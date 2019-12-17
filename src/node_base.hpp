#ifndef SIMMOS_NODE_BASE_HPP
#define SIMMOS_NODE_BASE_HPP


#include <component_base.h>
#include "logic_level.hpp"

namespace simmos {
    class NodeBase : public pipeline::source_base<LogicLevel> {
        void update();

    protected:
        class Input final : public pipeline::sink_base<LogicLevel> {
            NodeBase *_node;
            LogicLevel _value = LogicLevel::FLOATING;

        public:
            explicit Input(NodeBase *node);

            void accept(const LogicLevel &value) override;

            [[nodiscard]] LogicLevel value() const;

            ~Input() override;
        };

        virtual LogicLevel recalculate() = 0;

    public:
        ~NodeBase() override;
    };
}


#endif //SIMMOS_NODE_BASE_HPP
