#include "ConfigStructs.hh"
#include <string>
#include <vector>

#ifndef GLOBALCONF
#define GLOBALCONF

ConfigStructs::GlobalConf global_conf;

#endif

std::vector<std::string> ConfigStructs::property_strings(std::vector<ConfigStructs::ParticleProperty> props) {
	std::vector<std::string> res = std::vector<std::string>();

	for (auto p : props) {
		if (p == ParticleProperty::Ekin) res.push_back("Ekin");
		if (p == ParticleProperty::local_pos_x) res.push_back("local_pos_x");
		if (p == ParticleProperty::local_pos_y) res.push_back("local_pos_y");
		if (p == ParticleProperty::local_pos_z) res.push_back("local_pos_z");
		if (p == ParticleProperty::global_pos_x) res.push_back("global_pos_x");
		if (p == ParticleProperty::global_pos_y) res.push_back("global_pos_y");
		if (p == ParticleProperty::global_pos_z) res.push_back("global_pos_z");
		if (p == ParticleProperty::time) res.push_back("time");
	}

	return res;
}
