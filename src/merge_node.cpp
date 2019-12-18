#include <create.h>
#include "merge_node.hpp"
#include "node_base.hpp"


simmos::LogicLevel simmos::MergeNode::recalculate() {
    LogicLevel value = LogicLevel::FLOATING;
    for (auto &&i : _inputs) {
        value += i.get<Input>()->value();
    }
    return value;
}

simmos::MergeNode::MergeNode(size_t count) {
    _inputs.reserve(count);
    while (_inputs.size() < count) {
        _inputs.emplace_back(pipeline::create<Input>(this));
    }
}

simmos::MergeNode::MergeNode(const std::vector<pipeline::source<LogicLevel>> &sources) : MergeNode(sources.size()) {
    for (size_t i = 0; i < sources.size(); ++i) {
        sources[i] | _inputs[i];
    }
}

const pipeline::sink<simmos::LogicLevel> &simmos::MergeNode::operator[](size_t index) const {
    return _inputs[index];
}

simmos::MergeNode::~MergeNode() = default;
