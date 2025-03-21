#include "api.hh"
#include "ConfigStructs.hh"
#include "parser.hh"
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

#include "Collimator.hh"

void api::setup_sim(std::string arg) {
	add_placer("collimator", collimator);

	auto res = parser::parse_toml(arg);
}

/*
void api::setup_sim() {
	//place_geometry("SD1", "cube", {{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 10.*cm}, {"x_size", 15. * cm}, {"y_size", 15. * cm}, {"z_size", 0.02 * mm}}, Materials::Vacuum);
	//make_sd("SD1", "neutron", {property::Ekin, property::time});

	add_placer("collimator", collimator);

	//place_geometry("THEcollimator", "collimator", {{"x_pos", 2. * cm}, {"y_pos", 0.}, {"z_pos", 0.*cm}}, Materials::Vacuum);

	place_geometry(
		"PIXE", "sphere",
		{{"x_pos", 2. * cm}, {"y_pos", 0.}, {"z_pos", 0.*cm}, {"radius", 1. * cm}},
		Materials::Carbon
	);

	place_geometry(
		"PIXE_total", "sphere",
		{{"x_pos", 0. }, {"y_pos", 0.}, {"z_pos", 0. }, {"radius", 10. * cm}, {"inner_radius", 9.9 * cm}, {"transparency", 0.}},
		Materials::Carbon
	);

	place_geometry(
		"RBS", "sphere",
		{{"x_pos", 0. }, {"y_pos", 0.}, {"z_pos", 0. }, {"radius", 9.9 * cm}, {"inner_radius", 9.8 * cm}, {"transparency", 0.}},
		Materials::Vacuum
	);

	place_geometry(
		"target_base", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 5. * cm}, {"rot_x", 35. * degree}, {"x_size", 2. * cm}, {"y_size", 2. * cm}, {"z_size", 1. * mm}},
	 	Materials::Carbon
	);

	auto hist = ConfigStructs::Histogram{
		.name = "PIXE energy",
		.title = "PIXE energy",
		.nbins = 100,
		.xmin = 0.,
		.xmax = 10. * MeV
	};

	make_ps("PIXE", "energyDeposit", save_data{hist}, true, ConfigStructs::ParticleSpec("gamma"));
	make_sd("PIXE_total", "gamma", {property::Ekin, property::theta, property::phi});
	make_sd("RBS", "primary", {property::Ekin, property::time});
}
*/
