#ifndef SIMMOS_NMOS_HPP
#define SIMMOS_NMOS_HPP


#include <pipeline.h>
#include "node_base.hpp"

namespace simmos {
    class NMOS final : public NodeBase {
        pipeline::sink<LogicLevel> _gate;
        pipeline::sink<LogicLevel> _source;
    protected:
        LogicLevel recalculate() override;

    public:
        NMOS();

        [[nodiscard]] const pipeline::sink<simmos::LogicLevel> &gate() const;

        [[nodiscard]] const pipeline::sink<simmos::LogicLevel> &source() const;

        ~NMOS() override;
    };
}


#endif //SIMMOS_NMOS_HPP
