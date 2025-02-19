#ifndef SIM_API
#define SIM_API

#include <string>
#include <vector>
#include "ConfigStructs.hh"

namespace api {
	typedef ConfigStructs::ParticleProperty property;

	void generate_conf();
	void make_sd(
		std::string name,
		std::string particle,
		std::vector<property> properties,
		std::string l_volume = ""
	);
}

#endif
