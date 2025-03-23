#ifndef SNOWFLAKE_CELL_HPP
#define SNOWFLAKE_CELL_HPP

#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include "snowflakeState.hpp"
#include <cmath>

using namespace cadmium::celldevs;

class snowflake : public GridCell<snowflakeState, double> {
public:
    // Constructor: pass cell id and configuration to the base GridCell constructor.
    snowflake(const std::vector<int>& id,
              const std::shared_ptr<const GridCellConfig<snowflakeState, double>>& config)
      : GridCell<snowflakeState, double>(id, config) { }

    [[nodiscard]] snowflakeState localComputation(
        snowflakeState state,
        const std::unordered_map<std::vector<int>, NeighborData<snowflakeState, double>>& neighborhood
    ) const override {
        // If already frozen, remain frozen.
        if(state.frozen) {
            return state;
        }
        // Accumulate deposition from neighbors.
        double deposition = 0.0;
        for (const auto& [neighborId, neighborData] : neighborhood) {
            const snowflakeState* nState = neighborData.state;
            if(nState->frozen) {
                deposition += 0.15;  // Each frozen neighbor contributes 0.15
            }
        }
        // Add a constant background vapor supply.
        double background = 0.05;
        state.water += deposition + background;
        
        // If water accumulation reaches threshold, freeze the cell.
        if(state.water >= 1.0) {
            state.frozen = true;
            state.water = 1.0; // Cap water level to threshold.
        }
        return state;
    }

    // The outputDelay function returns a constant delay (here, 1.0 time unit).
    [[nodiscard]] double outputDelay(const snowflakeState& state) const override {
        return 1.0;
    }
};

#endif // SNOWFLAKE_CELL_HPP
