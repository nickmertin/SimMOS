#include "circuit.hpp"
#include "nmos.hpp"
#include "pmos.hpp"
#include "global_output.hpp"
#include "merge_node.hpp"

#include <algorithm>
#include <utility>

simmos::Circuit::SynthesisContext::~SynthesisContext() = default;

simmos::Circuit::RootSynthesisContext::RootSynthesisContext() : _global_high(pipeline::create<GlobalInput>()), _global_low(pipeline::create<GlobalInput>()), _global_float(pipeline::create<GlobalInput>()) {}

void simmos::Circuit::RootSynthesisContext::add_local_in(std::string name, pipeline::sink<simmos::LogicLevel> signal) {
    _in_signals[name].push_back(signal);
}

void simmos::Circuit::RootSynthesisContext::add_local_out(std::string name, pipeline::source<simmos::LogicLevel> signal) {
    _out_signals[name].push_back(signal);
}

pipeline::source<simmos::LogicLevel> simmos::Circuit::RootSynthesisContext::get_port_in(std::string name) {
    auto it = _in_ports.find(name);
    if (it == _in_ports.end()) {
        auto node = pipeline::create<GlobalInput>();
        _in_ports.insert({name, node});
        return node;
    }
    return it->second;
}

pipeline::sink<simmos::LogicLevel> simmos::Circuit::RootSynthesisContext::get_port_out(std::string name) {
    auto it = _out_ports.find(name);
    if (it == _out_ports.end()) {
        auto node = pipeline::create<GlobalOutput>();
        _out_ports.insert({name, node});
        return node;
    }
    return it->second;
}

pipeline::source<simmos::LogicLevel> simmos::Circuit::RootSynthesisContext::get_global_high() {
    return _global_high;
}

pipeline::source<simmos::LogicLevel> simmos::Circuit::RootSynthesisContext::get_global_low() {
    return _global_low;
}

pipeline::source<simmos::LogicLevel> simmos::Circuit::RootSynthesisContext::get_global_float() {
    return _global_float;
}

void simmos::Circuit::RootSynthesisContext::finish_synthesis() {
    for (auto &&e : _in_signals) {
        auto it = _out_signals.find(e.first);
        auto src = it == _out_signals.end() ? _global_float : it->second.size() == 1 ? it->second[0] : pipeline::create<MergeNode>(it->second);
        for (auto &&in : e.second) {
            std::move(src) | in; // NOLINT
        }
    }

    _global_high.get<GlobalInput>()->set(LogicLevel::HIGH);
    _global_low.get<GlobalInput>()->set(LogicLevel::LOW);
    _global_float.get<GlobalInput>()->set(LogicLevel::FLOATING);
}

std::unordered_map<std::string, simmos::LogicLevel> simmos::Circuit::RootSynthesisContext::simulate(const std::unordered_map<std::string, LogicLevel> &inputs) {
    for (auto &&in : _in_ports) {
        auto it = inputs.find(in.first);
        if (it == inputs.end()) {
            throw std::range_error{"Missing input: " + in.first};
        }
        in.second.get<GlobalInput>()->set(it->second);
    }

    std::unordered_map<std::string, LogicLevel> result;
    for (auto &&out : _out_ports) {
        result[out.first] = out.second.get<GlobalOutput>()->value();
    }
    return result;
}

simmos::Circuit::RootSynthesisContext::~RootSynthesisContext() = default;

simmos::Circuit::Node::~Node() = default;

simmos::Circuit::PortNode::PortNode(bool is_output, std::string name) : _is_output(is_output), _name(std::move(name)) {}

void simmos::Circuit::PortNode::synthesize(SynthesisContext &context) {
    if (_is_output) {
        context.add_local_in(_name, context.get_port_out(_name));
    } else {
        context.add_local_out(_name, context.get_port_in(_name));
    }
}

simmos::Circuit::PortNode::~PortNode() = default;

simmos::Circuit::TransistorNode::TransistorNode(bool is_nmos, std::string gate, std::string drain, std::string source) : _is_nmos(is_nmos), _gate(std::move(gate)), _drain(std::move(drain)), _source(std::move(source)) {}

void simmos::Circuit::TransistorNode::synthesize(SynthesisContext &context) {
    if (_is_nmos) {
        auto transistor = pipeline::create<NMOS>();
        context.add_local_in(_gate, transistor.get<NMOS>()->gate());
        context.add_local_in(_source, transistor.get<NMOS>()->source());
        context.add_local_out(_drain, transistor);
    } else {
        auto transistor = pipeline::create<PMOS>();
        context.add_local_in(_gate, transistor.get<PMOS>()->gate());
        context.add_local_in(_drain, transistor.get<PMOS>()->drain());
        context.add_local_out(_source, transistor);
    }
}

simmos::Circuit::TransistorNode::~TransistorNode() = default;

simmos::Circuit::GlobalSourceNode::GlobalSourceNode(bool is_high, std::string name) : _is_high(is_high), _name(std::move(name)) {}

void simmos::Circuit::GlobalSourceNode::synthesize(simmos::Circuit::SynthesisContext &context) {
    context.add_local_out(_name, _is_high ? context.get_global_high() : context.get_global_low());
}

simmos::Circuit::GlobalSourceNode::~GlobalSourceNode() = default;

simmos::Circuit::Circuit() = default;

void simmos::Circuit::add_input(const std::string &name) {
    _nodes.emplace_back(new PortNode(false, name));
}

void simmos::Circuit::add_output(const std::string &name) {
    _nodes.emplace_back(new PortNode(true, name));
}

void simmos::Circuit::add_pmos(const std::string &gate_name, const std::string &drain_name, const std::string &source_name) {
    _nodes.emplace_back(new TransistorNode(false, gate_name, drain_name, source_name));
}

void simmos::Circuit::add_nmos(const std::string &gate_name, const std::string &drain_name, const std::string &source_name) {
    _nodes.emplace_back(new TransistorNode(true, gate_name, drain_name, source_name));
}

void simmos::Circuit::add_vdd(const std::string &name) {
    _nodes.emplace_back(new GlobalSourceNode(true, name));
}

void simmos::Circuit::add_vss(const std::string &name) {
    _nodes.emplace_back(new GlobalSourceNode(false, name));
}

std::vector<std::unordered_map<std::string, simmos::LogicLevel>> simmos::Circuit::simulate(const std::vector<std::unordered_map<std::string, LogicLevel>> &test_cases) {
    std::vector<std::unordered_map<std::string, LogicLevel>> result;
    std::transform(test_cases.begin(), test_cases.end(), std::back_inserter(result), [this] (auto &&inputs) {
        RootSynthesisContext ctx;
        for (auto &&n : _nodes) {
            n->synthesize(ctx);
        }
        ctx.finish_synthesis();
        return ctx.simulate(inputs);
    });
    return result;
}
