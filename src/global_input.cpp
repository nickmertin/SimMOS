#include "global_input.hpp"

void simmos::GlobalInput::set(bool value) {
    push(value ? LogicLevel::HIGH : LogicLevel::LOW);
}

simmos::GlobalInput::~GlobalInput() = default;
