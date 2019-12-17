#include "node_base.hpp"

void simmos::NodeBase::update() {
    push(recalculate());
}

simmos::NodeBase::Input::Input(simmos::NodeBase *node) : _node(node) {}

void simmos::NodeBase::Input::accept(const simmos::LogicLevel &value) {
    _value = value;
    _node->update();
}

simmos::LogicLevel simmos::NodeBase::Input::value() const {
    return _value;
}

simmos::NodeBase::Input::~Input() = default;

simmos::NodeBase::~NodeBase() = default;
