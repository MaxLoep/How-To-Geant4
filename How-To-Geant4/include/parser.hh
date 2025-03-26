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

namespace parser {
	using std::span, std::string;

	enum cmd_type {
		place_geometry,
		make_ps,
		make_sd
	};

	typedef std::variant<std::string, double> argtype;

	struct command {
		cmd_type type;
		std::vector<argtype> args;
	};

	std::vector<std::string> chunks(std::basic_ifstream<char>& stream);

	struct token{
		token(std::string literal) {
			this->body = literal;
		}

		token() {
			this->body = std::vector<token>();
		}

		token(std::vector<token> vt) {
			this->body = vt;
		}

		token operator+(token other) {
			token new_tk = *this;
			if (std::vector<token>* inner = std::get_if<1>(&new_tk.body)) {
				if (std::vector<token>* addition = std::get_if<1>(&other.body)) {
					std::copy(addition->begin(), addition->end(), std::back_inserter(*inner));
				} else {
					inner->push_back(other);
				}
				return new_tk;
			} else {
				return token({*this, other});
			}
		}

		token operator|(token other) {
			token new_tk = *this;
			if (std::vector<token>* inner = std::get_if<std::vector<token>>(&new_tk.body)) {
				std::cout << "inserting" << std::endl;
				inner->push_back(other);
				return new_tk;
			} else {
				return token({*this, other});
			}
		}

		std::variant<std::string, std::vector<token>> body;
	};

	struct token_print {
		void operator()(std::string& s) {
			std::cout << s << " ";
		};

		void operator()(std::vector<token>& vt)  {
			std::cout << "{ ";
			for (auto e : vt) std::visit(token_print(), e.body);
			std::cout << " }";
		};
	};

	token tokenize(
		std::vector<std::string>::iterator current,
		std::vector<std::string>::iterator end,
		token partial_token = token(std::vector<token>({}))
	);


	token load_file(string filename);
	std::vector<command> token_to_cmd(token input);
}

#endif
