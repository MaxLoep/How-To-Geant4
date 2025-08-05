#include "parser.hh"
#include <cstdlib>
#include <string>
#ifdef _WIN32 || _WIN64
	#include <process.h>
#else
	#include <unistd.h>
#endif
#include <stdlib.h>

std::vector<std::vector<std::string>> parser::ssv_chunks(std::basic_ifstream<char>& stream) {
	std::string current_tag = "";
	char c;
	std::vector<std::vector<std::string>> chunk_list = std::vector<std::vector<std::string>>();
	std::vector<std::string> chunk = std::vector<std::string>();

	while (stream >> c) {
		if (c == ';') {
			chunk.push_back(current_tag);
			current_tag = "";
		}
		else if (c == '\n') {
			chunk_list.push_back(chunk);
			chunk = std::vector<std::string>();
		}
		else current_tag += c;
	}

	return chunk_list;
}

using cmd_tuple = parser::cmd_tuple;//std::vector<std::tuple<parser::cmd_type, std::map<std::string, string>, std::map<std::string, double>>>;

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

inline std::string trim(const std::string &s) {
	auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
	auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
	return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}

std::tuple<std::map<std::string, std::string>, std::map<std::string, double>>
primitive_args(std::vector<parser::string> input, int str_param_cutoff) {
	std::map<std::string, double> num_args = {};
	std::map<std::string, std::string> str_args = {};


	for (auto parameter : std::vector<parser::string>(input.begin(), input.begin() + str_param_cutoff)) {
		auto chunks = split(parameter, "=");
		str_args[trim(chunks[0])] = trim(chunks[1]);
	}

	for (auto parameter : std::vector<parser::string>(input.begin() + str_param_cutoff, input.end())) {
		auto chunks = split(parameter, "=");
		num_args[trim(chunks[0])] = std::stod(chunks[1]);
	}

	return {str_args, num_args};
}

cmd_tuple primitive_cmd(std::vector<parser::string>& line) {
	static std::map<parser::string, parser::cmd_type> cmd_lut = {
		{"place", parser::cmd_type::place_geometry},
		{"make_sd", parser::cmd_type::make_sd},
		{"make_ps", parser::cmd_type::make_ps},
		{"make_custom_material", parser::cmd_type::make_custom_material},
		{"particle_source", parser::cmd_type::particle_source},
		{"no_macro_f", parser::cmd_type::replace_macro_file},
		{"start_gui", parser::cmd_type::start_gui},
		{"prerun_macro", parser::cmd_type::prerun_macro},
	};

	int str_cnt = std::stoi(line[1]);
	auto [str_args, num_args] = primitive_args(std::vector<parser::string>(line.begin() + 2, line.end()), str_cnt);
	return {cmd_lut[trim(line[0])], str_args, num_args};
}

std::vector<cmd_tuple> parser::load_simple_file(std::string filename) {
	std::cout << "entering load file!" << std::endl;

	#ifdef _WIN64 || _WIN32
		auto temp_path = std::to_string(getpid()) + "geo_file.out";
	#else
		auto temp_path = "/tmp/" + std::to_string(getpid()) + "geo_file.out";
	#endif
	if (filename.ends_with(".py")) {
		std::cout << "running the python script, writing run file to: " << temp_path << std::endl;
		std::string exec_script = "python " + filename + " > " + temp_path;
		int res = std::system(exec_script.data());
		filename = temp_path;
	}

	auto file = std::ifstream(filename) >> std::noskipws;

	auto lines = ssv_chunks(file);
	auto res = std::vector<cmd_tuple>();
	for (auto line : lines) {
		res.push_back(primitive_cmd(line));
	}

	return res;
}
