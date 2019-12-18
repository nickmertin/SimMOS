#ifndef SIMMOS_GLOBAL_OUTPUT_HPP
#define SIMMOS_GLOBAL_OUTPUT_HPP


#include <pipeline.h>
#include "logic_level.hpp"

namespace simmos {
    class GlobalOutput final : public pipeline::sink_base<LogicLevel> {
        LogicLevel _value = LogicLevel::INDETERMINATE;

    public:
        [[nodiscard]] LogicLevel value() const;

        void accept(const LogicLevel &value) override;

        ~GlobalOutput() override;
    };
}


#endif //SIMMOS_GLOBAL_OUTPUT_HPP
