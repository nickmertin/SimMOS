#include <iostream>
#include "nmos.hpp"
#include "pmos.hpp"
#include "merge_node.hpp"
#include "global_input.hpp"

using namespace pipeline;
using namespace simmos;

void print(LogicLevel output) {
    switch (output) {
        case LogicLevel::LOW:
            std::cout << "LOW" << std::endl;
            break;
        case LogicLevel::HIGH:
            std::cout << "HIGH" << std::endl;
            break;
        case LogicLevel::FLOATING:
            std::cout << "FLOATING" << std::endl;
            break;
        case LogicLevel::SHORT:
            std::cout << "SHORT" << std::endl;
            break;
        case LogicLevel::INDETERMINATE:
            std::cout << "INDETERMINATE" << std::endl;
            break;
    }
}

int main(int argc, char **argv) {
    auto pmos = create<PMOS>();
    auto nmos = create<NMOS>();
    auto merge = create<MergeNode>(2);
    auto input = create<GlobalInput>();
    LogicLevel output;

    input | pmos.get<PMOS>()->gate();
    LogicLevel::HIGH | pmos.get<PMOS>()->drain();
    input | nmos.get<NMOS>()->gate();
    LogicLevel::LOW | nmos.get<NMOS>()->source();
    pmos | (*merge.get<MergeNode>())[0];
    nmos | (*merge.get<MergeNode>())[1];
    merge | create<variable_sink<LogicLevel>>(output);

    input.get<GlobalInput>()->set(false);
    print(output);
    input.get<GlobalInput>()->set(true);
    print(output);
}
