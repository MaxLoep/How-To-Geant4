#ifndef SIM_API
#define SIM_API

#include <string>
#include <vector>
#include "ConfigStructs.hh"
#include "Geometries.hh"
#include "Utils.hh"

namespace api {
	typedef ConfigStructs::ParticleProperty property;
	typedef util::Either<ConfigStructs::Histogram, ConfigStructs::NTuple> save_data;

	std::vector<std::string> setup_sim(std::string);
	void make_sd(
		std::string name,
		std::string particle,
		std::vector<property> properties,
		std::string l_volume = ""
	);

	void make_ps(
		std::string name,
		std::string quantity,
		save_data save_to,
		bool filter,
		ConfigStructs::ParticleSpec particle_filter,
		std::string l_volume = ""
	);

	void place_geometry(
		std::string l_volume_name,
		std::string geometry_name,
		std::map<std::string, double> placement_params = {},
		Materials::MaterialMaker material = Materials::Vacuum
	);

	void add_placer(std::string name, placer_func func);
}

#endif
