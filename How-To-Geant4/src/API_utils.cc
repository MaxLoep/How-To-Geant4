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


void api::setup_sim() {
	place_geometry("SD1", "cube", {{"x_pos", 0.}, {"y_pos", 0.}, {"z_pos", 10.*cm}, {"x_size", 15. * cm}, {"y_size", 15. * cm}, {"z_size", 0.02 * mm}}, Materials::Carbon);

	make_sd("SD1", "N14", {property::Ekin, property::time});
//	make_sd("SD2", "neutron", {property::Ekin, property::time});
//	make_sd("SD3", "proton", {property::Ekin, property::time});
//	make_sd("SD4", "proton", {property::Ekin, property::time});
//	make_sd("SD5", "proton", {property::Ekin, property::time});
//	make_sd("Sphere", "neutron", {property::Ekin, property::time});
}


void api::place_geometry(
	std::string l_volume_name,
	std::string geometry_name,
	std::map<std::string, double> placement_params,
	Materials::MaterialMaker material
) {
	geometries::register_placement(geometry_name, l_volume_name, placement_params, material);
}
