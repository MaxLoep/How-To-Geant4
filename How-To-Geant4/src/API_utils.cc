#include "api.hh"
#include "ConfigStructs.hh"

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

#include "Collimator.hh"

void api::setup_sim() {
	//place_geometry("SD1", "cube", {{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 10.*cm}, {"x_size", 15. * cm}, {"y_size", 15. * cm}, {"z_size", 0.02 * mm}}, Materials::Vacuum);
	//make_sd("SD1", "neutron", {property::Ekin, property::time});

	// NEUTRON COLLIMATOR GEOMETRY START
	// add_placer("collimator", collimator);

	// place_geometry("THEcollimator", "collimator", {{"x_pos", 0. * cm}, {"y_pos", 0.}, {"z_pos", 0.*cm}}, Materials::Vacuum);

	// place_geometry(
	// 	"Plane_150", "cube",
	// 	{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 150. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 1. * mm}},
	//  	Materials::Vacuum
	// );
	// make_sd("Plane_150", "neutron", {property::Ekin, property::local_pos_x, property::local_pos_y, property::time});

	// place_geometry(
	// 	"Plane_200", "cube",
	// 	{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 200. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 1. * mm}},
	//  	Materials::Vacuum
	// );
	// make_sd("Plane_200", "neutron", {property::Ekin, property::local_pos_x, property::local_pos_y, property::time});

	// place_geometry(
	// 	"Plane_300", "cube",
	// 	{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 300. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 1. * mm}},
	//  	Materials::Vacuum
	// );
	// make_sd("Plane_300", "neutron", {property::Ekin, property::local_pos_x, property::local_pos_y, property::time});

	// place_geometry(
	// 	"Plane_400", "cube",
	// 	{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 400. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 1. * mm}},
	//  	Materials::Vacuum
	// );
	// make_sd("Plane_400", "neutron", {property::Ekin, property::local_pos_x, property::local_pos_y, property::time});
	// NEUTRON COLLIMATOR GEOMETRY END

	// BEAM SPREAD GEOMETRY START
	place_geometry(//Plane00
		"Plane_00", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 0. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_00", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Plane0
		"Plane_0", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 10. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_0", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});
	
	place_geometry(//Air1
		"Air_1", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 15. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", (10.*cm)/2-0.1*mm}},
		Materials::Air
	);

	place_geometry(//Plane1
		"Plane_1", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 20. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_1", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Air2
		"Air_2", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 25. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", (10.*cm)/2-0.1*mm}},
		Materials::Air
	);

	place_geometry(//Plane2
		"Plane_2", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 30. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_2", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Air3
		"Air_3", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 35. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", (10.*cm)/2-0.1*mm}},
		Materials::Air
	);

	place_geometry(//Plane3
		"Plane_3", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 40. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_3", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Air4
		"Air_4", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 45. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", (10.*cm)/2-0.1*mm}},
		Materials::Air
	);

	place_geometry(//Plane4
		"Plane_4", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 50. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_4", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Air5
		"Air_5", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 55. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", (10.*cm)/2-0.1*mm}},
		Materials::Air
	);

	place_geometry(//Plane5
		"Plane_5", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 60. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_5", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Air6
		"Air_6", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 65. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", (10.*cm)/2-0.1*mm}},
		Materials::Air
	);

	place_geometry(//Plane6
		"Plane_6", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 70. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_6", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Plane7
		"Plane_7", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 80. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_7", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Plane8
		"Plane_8", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 90. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_8", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});

	place_geometry(//Plane9
		"Plane_9", "cube",
		{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 100. * cm}, {"rot_x", 0. * degree}, {"x_size", 50. * cm}, {"y_size", 50. * cm}, {"z_size", 0.1 * mm}},
		Materials::Vacuum
	);
	make_sd("Plane_9", "proton", {property::Ekin, property::local_pos_x, property::local_pos_y});
	// BEAM SPREAD GEOMETRY END

	// place_geometry(
	// 	"PIXE", "sphere",
	// 	{{"x_pos", - 5. * cm}, {"y_pos", 0.}, {"z_pos", 0.*cm}, {"radius", 1. * cm}},
	// 	Materials::Carbon
	// );

	// place_geometry(
	// 	"PIXE_total", "sphere",
	// 	{{"x_pos", 0. }, {"y_pos", 0.}, {"z_pos", 0. }, {"radius", 10. * cm}, {"inner_radius", 9.9 * cm}, {"transparency", 0.}},
	// 	Materials::Carbon
	// );

	// place_geometry(
	// 	"RBS", "sphere",
	// 	{{"x_pos", 0. }, {"y_pos", 0.}, {"z_pos", 0. }, {"radius", 9.9 * cm}, {"inner_radius", 9.8 * cm}, {"transparency", 0.}},
	// 	Materials::Vacuum
	// );

	// place_geometry(
	// 	"target_base", "cube",
	// 	{{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 0. * cm}, {"x_size", 0.5 * cm}, {"y_size", 0.5 * cm}, {"z_size", 0.5 * cm}},
	//  	Materials::Carbon
	// );

	auto hist = ConfigStructs::Histogram{
		.name = "PIXE energy",
		.title = "PIXE energy",
		.nbins = 100,
		.xmin = 0.,
		.xmax = 10. * MeV
	};

	// make_ps("PIXE", "energyDeposit", save_data{hist}, true, ConfigStructs::ParticleSpec("gamma"));
	// make_sd("PIXE_total", "gamma", {property::Ekin, property::theta, property::phi});
	// make_sd("RBS", "primary", {property::Ekin, property::time});
}
