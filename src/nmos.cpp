#include "nmos.hpp"

simmos::LogicLevel simmos::NMOS::recalculate() {
    auto gate = _gate.get<Input>()->value();
    auto source = _source.get<Input>()->value();
    if (gate == LogicLevel::SHORT || source == LogicLevel::SHORT) return LogicLevel::SHORT;
    if (gate == LogicLevel::HIGH) return source;
    if (gate == LogicLevel::LOW) return LogicLevel::FLOATING;
    return LogicLevel::INDETERMINATE;
}

simmos::NMOS::NMOS() : _gate(pipeline::create<Input>(this)), _source(pipeline::create<Input>(this)) {}

const pipeline::sink<simmos::LogicLevel> &simmos::NMOS::gate() const {
    return _gate;
}

const pipeline::sink<simmos::LogicLevel> &simmos::NMOS::source() const {
    return _source;
}

simmos::NMOS::~NMOS() = default;
