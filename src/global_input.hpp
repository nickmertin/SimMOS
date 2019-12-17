#ifndef SIMMOS_GLOBAL_INPUT_HPP
#define SIMMOS_GLOBAL_INPUT_HPP


#include <pipeline.h>
#include "logic_level.hpp"

namespace simmos {
    class GlobalInput final : public pipeline::source_base<LogicLevel> {
    public:
        void set(bool value);

        ~GlobalInput() override;
    };
}


#endif //SIMMOS_GLOBAL_INPUT_HPP
