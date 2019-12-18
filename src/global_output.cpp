#include "global_output.hpp"

simmos::LogicLevel simmos::GlobalOutput::value() const {
    return _value;
}

void simmos::GlobalOutput::accept(const simmos::LogicLevel &value) {
    _value = value;
}

simmos::GlobalOutput::~GlobalOutput() = default;
