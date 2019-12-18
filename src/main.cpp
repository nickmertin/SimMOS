#include <iostream>
#include <sstream>
#include <iomanip>
#include <numeric>
#include "nmos.hpp"
#include "pmos.hpp"
#include "merge_node.hpp"
#include "global_input.hpp"
#include "circuit.hpp"

using namespace pipeline;
using namespace simmos;

std::string to_string(LogicLevel output) {
    switch (output) {
        case LogicLevel::LOW:
            return "LOW";
        case LogicLevel::HIGH:
            return "HIGH";
        case LogicLevel::FLOATING:
            return "FLOATING";
        case LogicLevel::SHORT:
            return "SHORT";
        case LogicLevel::INDETERMINATE:
            return "INDETERMINATE";
    }
}

void check_name(const std::string &name, bool declaration) {
    if (name.empty()) {
        throw std::runtime_error{"Name cannot be empty!"};
    }
    if (declaration && name[0] == '~') {
        throw std::runtime_error{"Sames beginning with a tilde are reserved: " + name};
    }
}

int main() {
    Circuit c;
    std::vector<std::string> inputs, outputs;

    try {
        c.add_vdd("~vdd");
        c.add_vss("~vss");

        std::string line, cmd;
        while (std::getline(std::cin, line)) {
            line = line.substr(0, line.find('#'));
            std::istringstream in{line};
            if (!(in >> cmd) || cmd.empty()) continue;

            if (cmd == "INPUT") {
                std::string name;
                while (in >> name) {
                    check_name(name, true);
                    inputs.push_back(name);
                    c.add_input(name);
                }
            } else if (cmd == "OUTPUT") {
                std::string name;
                while (in >> name) {
                    check_name(name, true);
                    outputs.push_back(name);
                    c.add_output(name);
                }
            } else if (cmd == "PMOS") {
                std::string gate, drain, source;
                in >> gate >> drain >> source;
                check_name(gate, false);
                check_name(drain, false);
                check_name(source, false);
                c.add_pmos(gate, drain, source);
            } else if (cmd == "NMOS") {
                std::string gate, drain, source;
                in >> gate >> drain >> source;
                check_name(gate, false);
                check_name(drain, false);
                check_name(source, false);
                c.add_nmos(gate, drain, source);
            } else if (cmd == "END") {
                break;
            } else {
                throw std::runtime_error{"Unknown command: " + cmd};
            }
        }

        std::vector<std::unordered_map<std::string, LogicLevel>> cases{1};
        for (auto &&name : inputs) {
            decltype(cases) old;
            cases.swap(old);
            for (auto &&row : old) {
                row[name] = LogicLevel::LOW;
                cases.push_back(row);
                row[name] = LogicLevel::HIGH;
                cases.push_back(row);
            }
        }
        auto r = c.simulate(cases);
        std::vector<std::vector<std::string>> results;
        std::vector<std::string> header;
        header.insert(header.end(), inputs.begin(), inputs.end());
        header.insert(header.end(), outputs.begin(), outputs.end());
        std::vector<size_t> widths(header.size());
        std::transform(cases.begin(), cases.end(), r.begin(), std::back_inserter(results), [&] (auto &&cases_row, auto &&results_row) {
            std::vector<std::string> row;
            std::transform(inputs.begin(), inputs.end(), std::back_inserter(row), [&] (auto &&name) {
                auto str = to_string(cases_row[name]);
                auto &w = widths[row.size()];
                if (str.size() > w) {
                    w = str.size();
                }
                return str;
            });
            std::transform(outputs.begin(), outputs.end(), std::back_inserter(row), [&] (auto &&name) {
                auto str = to_string(results_row[name]);
                auto &w = widths[row.size()];
                if (str.size() > w) {
                    w = str.size();
                }
                return str;
            });
            return row;
        });

        results.insert(results.begin(), {std::string(std::accumulate(widths.begin(), widths.end(), 3 * (header.size() - 1)), '-')});
        results.insert(results.begin(), header);

        for (auto &&row : results) {
            for (size_t i = 0; i < row.size(); ++i) {
                if (i) {
                    std::cout << " | ";
                }
                std::cout << std::setw(widths[i]) << row[i];
            }
            std::cout << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
