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
	std::cout << "------------ tokenize call ------------" << std::endl << "\"";
	for (;printer != end; ++printer) std::cout << *printer << " ";
	std::cout << "\"" << std::endl << std::endl;

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

std::vector<parser::command> parser::token_to_cmd(parser::token input) {
	return {};
}
