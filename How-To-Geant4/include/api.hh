#ifndef SIM_API
#define SIM_API

#include <string>
#include <vector>
#include "ConfigStructs.hh"
#include "Geometries.hh"

namespace api {
	typedef ConfigStructs::ParticleProperty property;

	void setup_sim();
	void make_sd(
		std::string name,
		std::string particle,
		std::vector<property> properties,
		std::string l_volume = ""
	);

	void place_geometry(
		std::string l_volume_name,
		std::string geometry_name,
		std::map<std::string, double> placement_params = {},
		Materials::MaterialMaker material = Materials::Vacuum
	);
}

#endif
