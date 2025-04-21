#ifndef PARSER
#define PARSER

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <functional>
#include <iterator>
#include <string>
#include <variant>
#include <vector>
#include <span>
#include <sstream>

namespace parser {
	using std::span, std::string;

	enum cmd_type {
		place_geometry,
		make_ps,
		make_sd,
		make_custom_material,
		particle_source,
		replace_macro_file,
		start_gui
	};


	std::vector<std::vector<std::string>> ssv_chunks(std::basic_ifstream<char>& stream);
	using cmd_tuple = std::tuple<parser::cmd_type, std::map<std::string, string>, std::map<std::string, double>>;
	std::vector<cmd_tuple> load_simple_file(string filename);
}

#endif
