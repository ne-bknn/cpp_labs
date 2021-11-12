#pragma once

#include <limits>
#include <iostream>

// I've implemented this one in previous lab, so I can use it
// I guess
#include <vector>

namespace ui {
	template<typename T>
	void get(T& a) {
		while (! (std::cin >> a)) {
			if (std::cin.eof()) {
				throw std::runtime_error("EOF");
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	std::string get_str();
	std::string get_str_custom(std::istream& is);
	
	/*
	namespace {
		struct AbstractOp {
			std::string op_name;
			std::string args_type;
			std::string return_type;
			std::string description;

		};

		template<typename F>
		struct Operation: AbstractOp {
			F callback;
		};
	}
	
	template<typename T>
	class TextApp {
		private:
			std::vector<AbstractOp*> ops;
			T structure;
		public:
			explicit TextApp(std::vector<AbstractOp*> ops, T structure) {
				this->ops = ops;
				this->structure = structure;
			}

			void display_help() {
				for (int i = 0; i < this->ops.size(); ++i) {
					std::cout << ops[i]->op_name << ": " << ops[i]->description << std::endl;
				}
			}

			void prompt() {
				std::cout << "$ ";
				std::string cmd = get_str();

				if (cmd.compare(std::string{"help"})) {
					this->display_help();
					return;
				}

				bool found = false;
				for (int i = 0; i < this->ops.size(); ++i) {
					if (cmd.compare(ops[i]->op_name) == 0) {
						found = true;
						
						// I do not need to parse args for now
						// TODO: implement this I guess?
						for (char const &c: ops[i]->args_type) {
							throw std::runtime_error("Not implemented");
							if (c == 'd') {
								// int	
							} else if (c == 'f') {
								// double
							} else if (c == 's') {
								// string
							}
						}

						std::cout << ops[i]->callback() << std::endl;
					}
				}

				if (!found) {
					std::cout << "[-] There is no such command. Use help to view available commands" << std::endl;
				}
			}
	};
	*/
}
