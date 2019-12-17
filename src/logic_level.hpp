#ifndef SIMMOS_LOGIC_LEVEL_HPP
#define SIMMOS_LOGIC_LEVEL_HPP


namespace simmos {
    enum class LogicLevel {
        LOW,
        HIGH,
        FLOATING,
        SHORT,
        INDETERMINATE,
    };

    constexpr LogicLevel operator+(LogicLevel lhs, LogicLevel rhs) {
        if (lhs == LogicLevel::FLOATING) return rhs;
        if (rhs == LogicLevel::FLOATING) return lhs;
        if (lhs == LogicLevel::INDETERMINATE || rhs == LogicLevel::INDETERMINATE) return LogicLevel::INDETERMINATE;
        if (lhs == rhs) return lhs;
        return LogicLevel::SHORT;
    }

    constexpr LogicLevel &operator+=(LogicLevel &lhs, LogicLevel rhs) {
        return lhs = lhs + rhs;
    }
}


#endif //SIMMOS_LOGIC_LEVEL_HPP
