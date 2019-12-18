#ifndef SIMMOS_CIRCUIT_HPP
#define SIMMOS_CIRCUIT_HPP


#include <pipeline.h>
#include "logic_level.hpp"
#include "global_input.hpp"

namespace simmos {
    class Circuit final {
        class SynthesisContext {
        public:
            virtual void add_local_in(std::string name, pipeline::sink<LogicLevel> signal) = 0;
            virtual void add_local_out(std::string name, pipeline::source<LogicLevel> signal) = 0;
            virtual pipeline::source<LogicLevel> get_port_in(std::string name) = 0;
            virtual pipeline::sink<LogicLevel> get_port_out(std::string name) = 0;
            virtual pipeline::source<LogicLevel> get_global_high() = 0;
            virtual pipeline::source<LogicLevel> get_global_low() = 0;
            virtual pipeline::source<LogicLevel> get_global_float() = 0;

            virtual ~SynthesisContext();
        };

        class RootSynthesisContext final : public SynthesisContext {
            std::unordered_map<std::string, std::vector<pipeline::sink<LogicLevel>>> _in_signals;
            std::unordered_map<std::string, std::vector<pipeline::source<LogicLevel>>> _out_signals;
            std::unordered_map<std::string, pipeline::source<LogicLevel>> _in_ports;
            std::unordered_map<std::string, pipeline::sink<LogicLevel>> _out_ports;
            pipeline::source<simmos::LogicLevel> _global_high;
            pipeline::source<simmos::LogicLevel> _global_low;
            pipeline::source<simmos::LogicLevel> _global_float;

        public:
            RootSynthesisContext();

            void add_local_in(std::string name, pipeline::sink<LogicLevel> signal) override;

            void add_local_out(std::string name, pipeline::source<LogicLevel> signal) override;

            pipeline::source<simmos::LogicLevel> get_port_in(std::string name) override;

            pipeline::sink<simmos::LogicLevel> get_port_out(std::string name) override;

            pipeline::source<simmos::LogicLevel> get_global_high() override;

            pipeline::source<simmos::LogicLevel> get_global_low() override;

            pipeline::source<LogicLevel> get_global_float() override;

            void finish_synthesis();

            std::unordered_map<std::string, LogicLevel> simulate(const std::unordered_map<std::string, LogicLevel> &inputs);

            ~RootSynthesisContext() override;
        };

        class Node {
        public:
            virtual void synthesize(SynthesisContext &context) = 0;

            virtual ~Node();
        };

        class PortNode final : public Node {
            bool _is_output;
            std::string _name;

        public:
            PortNode(bool is_output, std::string name);

            void synthesize(SynthesisContext &context) override;

            ~PortNode() override;
        };

        class GlobalSourceNode final : public Node {
            bool _is_high;
            std::string _name;

        public:
            GlobalSourceNode(bool is_high, std::string name);

            void synthesize(SynthesisContext &context) override;

            ~GlobalSourceNode() override;
        };

        class TransistorNode final : public Node {
            bool _is_nmos;
            std::string _gate;
            std::string _drain;
            std::string _source;

        public:
            TransistorNode(bool is_nmos, std::string gate, std::string drain, std::string source);

            void synthesize(SynthesisContext &context) override;

            ~TransistorNode() override;
        };

        std::vector<std::unique_ptr<Node>> _nodes;
    public:
        Circuit();

        void add_input(const std::string &name);

        void add_output(const std::string &name);

        void add_pmos(const std::string &gate_name, const std::string &drain_name, const std::string &source_name);

        void add_nmos(const std::string &gate_name, const std::string &drain_name, const std::string &source_name);

        void add_vdd(const std::string &name);

        void add_vss(const std::string &name);

        std::vector<std::unordered_map<std::string, LogicLevel>> simulate(const std::vector<std::unordered_map<std::string, LogicLevel>> &test_cases);
    };
}


#endif //SIMMOS_CIRCUIT_HPP
