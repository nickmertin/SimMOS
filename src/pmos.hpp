#ifndef SIMMOS_PMOS_HPP
#define SIMMOS_PMOS_HPP


#include <pipeline.h>
#include "node_base.hpp"

namespace simmos {
    class PMOS final : public NodeBase {
        pipeline::sink<LogicLevel> _gate;
        pipeline::sink<LogicLevel> _drain;
    protected:
        LogicLevel recalculate() override;

    public:
        PMOS();

        [[nodiscard]] const pipeline::sink<simmos::LogicLevel> &gate() const;

        [[nodiscard]] const pipeline::sink<simmos::LogicLevel> &drain() const;

        ~PMOS() override;
    };
}


#endif //SIMMOS_PMOS_HPP
