#ifndef SNOWFLAKE_STATE_HPP
#define SNOWFLAKE_STATE_HPP

#include <iostream>
#include <nlohmann/json.hpp>
#include <cmath>

//! Snowflake cell state
struct snowflakeState {
    double water;  // Accumulated water/vapor (0.0 to threshold 1.0)
    bool frozen;   // true if cell has frozen (ice), false otherwise

    //! Default constructor: initialize water to 0.0 and frozen to false.
    snowflakeState() : water(0.0), frozen(false) {}
};

//! Overload operator<< for logging (prints as: <water,frozen>)
inline std::ostream& operator<<(std::ostream& os, const snowflakeState& s) {
    os << "<" << s.water << "," << (s.frozen ? "1" : "0") << ">";
    return os;
}

//! Inequality operator to compare two states (with tolerance for water value)
inline bool operator!=(const snowflakeState& a, const snowflakeState& b) {
    return (a.frozen != b.frozen) || (std::fabs(a.water - b.water) > 1e-6);
}

//! from_json: initializes a snowflakeState from a JSON object.
inline void from_json(const nlohmann::json& j, snowflakeState& s) {
    j.at("water").get_to(s.water);
    j.at("frozen").get_to(s.frozen);
}

#endif // SNOWFLAKE_STATE_HPP
