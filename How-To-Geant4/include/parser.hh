#ifndef PARSER
#define PARSER

#include <span>
#include <Utils.hh>

namespace parser {
	using std::span, std::string;
	using util::Either, util::Option;

	enum cmd_type {
		place_geometry,
		make_ps,
		make_sd
	};

	struct command {
		cmd_type type;
		span<Either<string, double>> arguments;
	};

	span<command> parse_toml(string filename);
}

#endif
