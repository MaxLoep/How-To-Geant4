#include "api.hh"
#include "ConfigStructs.hh"
#include "parser.hh"
#include <string>
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
}

// should primitive scorers even be implemented in code??
// they can also be constructed in macro files so maybe that is better??
void api::make_ps(
		std::string name,
		std::string quantity,
		util::Either<ConfigStructs::Histogram, ConfigStructs::NTuple> save_to,
		bool filter,
		ConfigStructs::ParticleSpec particle_filter,
		std::string l_volume
){
	auto _ = global_conf.lock();

	if (save_to.has_a) {
		global_conf.ra_conf.add_analysis(save_to.option_a);
		int hist_id = global_conf.ra_conf.histograms.size() - 1;
		global_conf.eoe_conf.ps_hist_targets.push_back({name + "/" + quantity, hist_id});
	} else {
		global_conf.ra_conf.add_analysis(save_to.option_b);
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


std::vector<std::string> api::setup_sim(std::string arg) {
	add_placer("collimator", collimator);

	auto commands = parser::load_simple_file(arg);
	std::vector<std::string> macro_commands = {};

	for (auto [command_type, string_args, numerical_args] : commands) {
		std::cout << string_args["command"] << std::endl;
		// actually run the commands lol!
		if (command_type == parser::cmd_type::place_geometry) {
			std::cout << "running place_geometry command" << std::endl;
			auto mat = Materials::get_mat(string_args["material"]);
			place_geometry(string_args["name"], string_args["object"], numerical_args, mat);
		} else if (command_type == parser::cmd_type::make_sd) {
			std::cout << "running make_sd command" << std::endl;
			auto properties = std::vector<api::property>();
			for (int i = 0; i < (int) numerical_args["attrib_count"]; ++ i) {
				std::string attrib_name = "attrib_" + std::to_string(i);
				properties.push_back(property_from_string(string_args[attrib_name]));
				std::cout << string_args[attrib_name] << std::endl;
			}
			make_sd(string_args["name"], string_args["particle"], properties, string_args["attach"]);
		} else if (command_type == parser::cmd_type::make_ps) {
			std::cout << "not implemented yet!" << std::endl;
		} else if (command_type == parser::cmd_type::make_custom_material) {
			std::cout << "running make_custom_mat command" << std::endl;
			std::vector<std::tuple<std::string, double>> parts = {};
			for (auto [mat, amount] : numerical_args) {
				if (mat == "density") continue;
				parts.push_back({mat, amount});
			}
			Materials::add_custom_mat(string_args["name"], parts, numerical_args["density"]);
		} else if (command_type == parser::cmd_type::particle_source) {
			std::cout << "making particle source" << std::endl;
			auto pos = macro_commands.size() > 3? macro_commands.begin(): macro_commands.begin();
			pos = macro_commands.insert(pos, "/gps/particle " + string_args["particle"]);
			pos = macro_commands.insert(pos, "/gps/position "
				+ std::to_string(numerical_args["x_pos"]) + " "
				+ std::to_string(numerical_args["y_pos"]) + " "
				+ std::to_string(numerical_args["x_pos"])
			);
			pos = macro_commands.insert(pos, "/gps/direction "
				+ std::to_string(numerical_args["x_facing"]) + " "
				+ std::to_string(numerical_args["y_facing"]) + " "
				+ std::to_string(numerical_args["x_facing"])
			);
			if (string_args["mono_e"] == "true"){
				pos = macro_commands.insert(pos, "/gps/ene/type Mono");
				pos = macro_commands.insert(pos, "/gps/ene/mono " + std::to_string(numerical_args["energy"]) + " MeV");
			} else {
				exit(-1);
			}

			macro_commands.insert(pos, "/gps/pos/type Point");
		} else if (command_type == parser::cmd_type::replace_macro_file) {
			std::cout << "making run macros" << std::endl;
			macro_commands.insert(macro_commands.begin(), "/run/initialize");
			macro_commands.insert(macro_commands.begin(), "/run/numberOfThreads " + std::to_string((int) (numerical_args["thread_count"])));
			macro_commands.insert(macro_commands.end(), "/run/printProgress " + std::to_string((int) (numerical_args["event_count"] / 1e3)));
			macro_commands.insert(macro_commands.end(), "/run/beamOn " + std::to_string((int) (numerical_args["event_count"])));
		}
	}


	return macro_commands;
}
