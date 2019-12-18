#ifndef SIMMOS_MERGE_NODE_HPP
#define SIMMOS_MERGE_NODE_HPP


#include <vector>
#include <component.h>
#include <component_base.h>
#include "logic_level.hpp"
#include "node_base.hpp"

namespace simmos {
    class MergeNode final : public NodeBase {

        std::vector<pipeline::sink<LogicLevel>> _inputs;

        LogicLevel recalculate() override;

    public:
        explicit MergeNode(size_t count);

        explicit MergeNode(const std::vector<pipeline::source<LogicLevel>> &sources);

        const pipeline::sink<LogicLevel> &operator[](size_t index) const;

        ~MergeNode() override;
    };
}


#endif //SIMMOS_MERGE_NODE_HPP
