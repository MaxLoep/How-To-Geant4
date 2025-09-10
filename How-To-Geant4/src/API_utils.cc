#include "api.hh"
#include "ConfigStructs.hh"
#include "config.hh"
#include "parser.hh"
#include <cstddef>
#include <string>
#include <vector>

extern ConfigStructs::GlobalConf global_conf;

#include "G4ios.hh"


void api::make_sd(
	std::string name,
	std::string particle,
	std::vector<property> properties,
	std::string l_volume
) {
	auto _ = global_conf.lock();

	if (l_volume == "") l_volume = "l" + name;
	ConfigStructs::DetectionInfo sd_detection_info = global_conf.ra_conf.add_detection_info(
		name, particle, properties
	);

	ConfigStructs::SDConfig sd_conf {
		.name = name,
		.logical_volume = l_volume,
		.log_properties = {sd_detection_info}
	};

	global_conf.sd_conf.push_back(sd_conf);

	#ifdef DEBUG
	std::cout << "make_sd ran succesfully" << std::endl;
	#endif
}

// should primitive scorers even be implemented in code??
// they can also be constructed in macro files so maybe that is better??
void api::make_ps(
		std::string name,
		std::string quantity,
		save_data save_to,
		bool filter,
		ConfigStructs::ParticleSpec particle_filter,
		std::string l_volume
){
	auto _ = global_conf.lock();

	if (auto save_variant = std::get_if<ConfigStructs::Histogram>(&save_to)) {
		global_conf.ra_conf.add_analysis(*save_variant);
		int hist_id = global_conf.ra_conf.histograms.size() - 1;
		global_conf.eoe_conf.ps_hist_targets.push_back({name + "/" + quantity, hist_id});
	} else if (auto save_variant = std::get_if<ConfigStructs::NTuple>(&save_to)) {
		global_conf.ra_conf.add_analysis(*save_variant);
		int tuple_id = global_conf.ra_conf.tuples.size() - 1;
		global_conf.eoe_conf.ps_tuple_targets.push_back({name + "/" + quantity, tuple_id, 0});
	}

	if (l_volume == "") l_volume = "l" + name;

	ConfigStructs::PSConf ps_conf{
		.name = name,
		.logical_volume = l_volume,
		.quantity = quantity,
		.filtered = filter,
		.filter_for = particle_filter,
	};

	global_conf.ps_conf.push_back(ps_conf);
}

void api::place_geometry(
	std::string l_volume_name,
	std::string geometry_name,
	std::map<std::string, double> placement_params,
	Materials::MaterialMaker material
) {
	geometries::register_placement(geometry_name, l_volume_name, placement_params, material);
}

void api::add_placer(std::string name, placer_func func) {
	geometries::add_placer(name, func);
}


api::property property_from_string(std::string property_string) {
	// Ekin, local_pos_x,  local_pos_y,  local_pos_z, time, theta, phi
	std::map<std::string, api::property> lut = {
		{"ekin", api::property::Ekin},
		{"pos_x", api::property::local_pos_x},
		{"pos_y", api::property::local_pos_y},
		{"pos_z", api::property::local_pos_z},
		{"theta", api::property::theta},
		{"phi", api::property::phi}
	};

	return lut[property_string];
}


#include "Collimator.hh"


std::tuple<std::vector<std::string>, bool> api::setup_sim(std::string arg) {
	add_placer("collimator", collimator);

	auto commands = parser::load_simple_file(arg);
	std::vector<std::string> macro_commands = {};
	bool interactive = false;

	for (auto [command_type, string_args, numerical_args] : commands) {
		std::cout << string_args["command"] << std::endl;
		// actually run the commands lol!
		if (command_type == parser::cmd_type::place_geometry) {
			auto mat = Materials::get_mat(string_args["material"]);
			place_geometry(string_args["name"], string_args["object"], numerical_args, mat);
		} else if (command_type == parser::cmd_type::make_sd) {
			#ifdef DEBUG
			std::cout << "running make_sd command" << std::endl;
			#endif

			auto properties = std::vector<api::property>();
			for (int i = 0; i < (int) numerical_args["attrib_count"]; ++ i) {
				std::string attrib_name = "attrib_" + std::to_string(i);
				properties.push_back(property_from_string(string_args[attrib_name]));
				std::cout << string_args[attrib_name] << std::endl;
			}

			#ifdef DEBUG
			std::cout << "\tparticle is: " << string_args["particle"] << std::endl;
			#endif

			make_sd(string_args["name"], string_args["particle"], properties, string_args["attach"]);
		} else if (command_type == parser::cmd_type::make_ps) {
			std::cout << "not implemented yet!" << std::endl;
		} else if (command_type == parser::cmd_type::make_custom_material) {
			std::vector<std::tuple<std::string, double>> parts = {};
			for (auto [mat, amount] : numerical_args) {
				if (mat == "density") continue;
				parts.push_back({mat, amount});
			}
			Materials::add_custom_mat(string_args["name"], parts, numerical_args["density"]);
		} else if (command_type == parser::cmd_type::particle_source) {
			auto pos = macro_commands.size() > 2? macro_commands.begin() + 2: macro_commands.begin();
			if (string_args["shape"] == "point") pos = macro_commands.insert(pos, "/gps/pos/type Point");
			else if (string_args["shape"] == "beam") {
				pos = macro_commands.insert(pos, "/gps/pos/type Beam");
				pos = macro_commands.insert(pos, "/gps/pos/sigma_r " + std::to_string(numerical_args["sigma_r"]) + " mm");
				std::cout << "sigma r is: " << std::to_string(numerical_args["sigma_r"]) << std::endl;
			}

			if (string_args["particle"] == "ion") {
				pos = macro_commands.insert(pos,
					std::format("/gps/ion {} {} {} {}",
						(int) numerical_args["atomic_number"],
						(int) numerical_args["atomic_mass"],
						(int) numerical_args["charge"],
						(int) numerical_args["excitation"]
					)
				);
			}
			pos = macro_commands.insert(pos, "/gps/particle " + string_args["particle"]);

			pos = macro_commands.insert(pos, std::format("/gps/position {} {} {} mm", numerical_args["x_pos"], numerical_args["y_pos"], numerical_args["z_pos"]));
			pos = macro_commands.insert(pos, std::format("/gps/direction {} {} {}", numerical_args["x_facing"], numerical_args["y_facing"], numerical_args["z_facing"]));

			if (string_args["mono_e"] == "true"){
				pos = macro_commands.insert(pos, "/gps/ene/type Mono");
				pos = macro_commands.insert(pos, "/gps/ene/mono " + std::to_string(numerical_args["energy"]) + " MeV");
			} else {

				std::vector<std::string> amplitudes = parser::to_vec(string_args["amplitudes"]);
				std::vector<std::string> energies = parser::to_vec(string_args["energies"]);
				for (int i = amplitudes.size() - 1; i >= 0; --i) {
					std::string s = "/gps/hist/point ";
					s.append(energies[i]);
					s.append(" ");
					s.append(amplitudes[i]);
					//std::cout << s << std::endl;
					pos = macro_commands.insert(pos, s);
				}
				pos = macro_commands.insert(pos, "/gps/hist/type energy");
				pos = macro_commands.insert(pos, "/gps/ene/type User");
			}

		} else if (command_type == parser::cmd_type::replace_macro_file) {
			macro_commands.insert(macro_commands.begin(), "/run/initialize");
			macro_commands.insert(macro_commands.begin(), "/run/numberOfThreads " + std::to_string((int) (numerical_args["thread_count"])));
			macro_commands.insert(macro_commands.end(), "/run/printProgress " + std::to_string((int) (numerical_args["event_count"] / 10)));
			while (numerical_args["event_count"] > 2e9) {
				macro_commands.insert(macro_commands.end(), "/run/beamOn " + std::to_string((int) 2e9));
				numerical_args["event_count"] -= 2e9;
			}
			macro_commands.insert(macro_commands.end(), "/run/beamOn " + std::to_string((int) (numerical_args["event_count"])));
		} else if (command_type == parser::cmd_type::start_gui) {
			interactive = true;
		} else if (command_type == parser::cmd_type::prerun_macro) {
			macro_commands.insert(macro_commands.begin(), string_args["content"]);
		}
	}


	return {macro_commands, interactive};
}
