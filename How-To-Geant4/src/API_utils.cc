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


void api::generate_conf() {
	make_sd("SD1", "neutron", {property::Ekin, property::time});
	make_sd("SD2", "neutron", {property::Ekin, property::time});
	make_sd("SD3", "proton", {property::Ekin, property::time});
	make_sd("SD4", "proton", {property::Ekin, property::time});
	make_sd("SD5", "proton", {property::Ekin, property::time});
	make_sd("Sphere", "neutron", {property::Ekin, property::time});
}
