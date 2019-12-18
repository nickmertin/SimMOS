#include "global_input.hpp"

void simmos::GlobalInput::set(LogicLevel value) {
    push(value);
}

simmos::GlobalInput::~GlobalInput() = default;
