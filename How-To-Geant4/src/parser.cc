#include "parser.hh"


parser::token parser::load_file(string filename) {
	auto file = std::ifstream(filename) >> std::noskipws;
	auto chunk_list = chunks(file);
	auto tokenized = tokenize(chunk_list.begin(), chunk_list.end());

	return tokenized;
}

std::vector<std::string> parser::chunks(std::basic_ifstream<char>& stream) {
	std::string current_tag = "";
	char c;
	std::vector<std::string> chunk_list = std::vector<std::string>();

	chunk_list.push_back("(");

	while (stream >> c) {
		if (c == '(') {
			chunk_list.push_back(current_tag);
			current_tag = "";
			chunk_list.push_back("(");
		}
		else if (c == ')') {
			chunk_list.push_back(current_tag);
			current_tag = "";
			chunk_list.push_back(")");
		}
		else if (c == ' ') {
			chunk_list.push_back(current_tag);
			current_tag = "";
		}
		else current_tag += c;
	}

	chunk_list.push_back(")");
	return chunk_list;
}

parser::token parser::tokenize(
	std::vector<std::string>::iterator current,
	std::vector<std::string>::iterator end,
	token partial_token
) {
	auto printer = current;
	//std::cout << "------------ tokenize call ------------" << std::endl << "\"";
	//for (;printer != end; ++printer) std::cout << *printer << " ";
	//std::cout << "\"" << std::endl << std::endl;

	auto next = current + 1;
	// singular tokens are converted and returned
	if (next == end) return token(*current);

	// brackets seek the end of the bracket and then
	else if (*current == "(") {
		auto eob = next;
		int bc = 1;
		for (; eob != end and bc > 0; ++ eob) {
			if (*eob == ")") --bc;
			if (*eob == "(") ++bc;
		}
		--eob;
		if (eob + 1 != end) return token() | (tokenize(next, eob) + tokenize(eob + 1, end));
		else return token() | tokenize(next, eob);
	} else {
		return (token() + tokenize(current, next)) + tokenize(next, end);
	}
}

double apply_unit(double value, std::string& unit) {
	return value; //TODO: functionality
}

parser::argtype collapse_to_argtype(parser::token tkn) {
	if (std::vector<parser::token>* inner = std::get_if<std::vector<parser::token>>(&tkn.body)) {
		// in this case, inner is not a singular token
		// could be a sequence or something nested.
		// if it's a keyword (a future todo. like loops, ifs, function defs, or other math stuff)
		// the token must be evaluated down to a value
		// if it is a value literal, the value is the result
		// or if it is of the form: (parameter name,) literal, unit
		// this needs to be evaluated in a simplyfied version

	} else {
		std::string inner = std::get<std::string>(&tkn.body);
		// check if it's a numerical literal or a string literal.
		// return that.
		try {
			return atof(inner);
 		} catch () {
			return inner;
		}
	}

	return 0.;
}

parser::command construct_place_cmd(parser::token input) {
	return {parser::cmd_type::place_geometry, {}};
}

parser::command construct_make_sd_cmd(parser::token input) {
	return {parser::cmd_type::make_sd, {}};
}

parser::command construct_make_ps_cmd(parser::token input) {
	return {parser::cmd_type::make_ps, {}};
}

parser::token unwrap(parser::token tkn) {
	return tkn.vec()[0];
}

bool contains_token_vec(parser::token tkn) {
	if (std::vector<parser::token>* inner = std::get_if<std::vector<parser::token>>(&tkn.body)) {
		return inner->size() > 1;
		// todo: make better check. this is merely improvised and could brake
		// with single line input files
	}

	return true; //this case should not happen but could technically
	// happen if an already too far unwrapped token is given.
	// too prevent endless looping this case should be true
}

std::vector<parser::command> parser::token_to_cmd(parser::token input) {
	static std::map<std::string, std::function<parser::command(parser::token)>> cmd_converter = {
		{"place", construct_place_cmd},
		{"make_sd", construct_make_sd_cmd},
		{"make_ps", construct_make_ps_cmd}
	};

	while (not contains_token_vec(input)) input = unwrap(input);

	//TODO: strip redundant token layers (vec with top level tokens contained)
	// these can occur an arbitrary number of times but at least once

	std::vector<parser::command> res = {};
	for (auto tkn : input.vec()) {
		std::string name = tkn.vec()[0].value();
		res.push_back(cmd_converter[name](tkn));
	}
	return res;
}
