#include "pmos.hpp"

simmos::LogicLevel simmos::PMOS::recalculate() {
    auto gate = _gate.get<Input>()->value();
    auto drain = _drain.get<Input>()->value();
    if (gate == LogicLevel::SHORT || drain == LogicLevel::SHORT) return LogicLevel::SHORT;
    if (gate == LogicLevel::LOW) return drain;
    if (gate == LogicLevel::HIGH) return LogicLevel::FLOATING;
    return LogicLevel::INDETERMINATE;
}

simmos::PMOS::PMOS() : _gate(pipeline::create<Input>(this)), _drain(pipeline::create<Input>(this)) {}

const pipeline::sink<simmos::LogicLevel> &simmos::PMOS::gate() const {
    return _gate;
}

const pipeline::sink<simmos::LogicLevel> &simmos::PMOS::drain() const {
    return _drain;
}

simmos::PMOS::~PMOS() = default;
