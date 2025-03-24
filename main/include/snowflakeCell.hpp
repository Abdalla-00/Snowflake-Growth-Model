#ifndef SNOWFLAKE_CELL_HPP
#define SNOWFLAKE_CELL_HPP

#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include "include/snowflakeState.hpp"
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
        // If already frozen, no further computation is needed.
        if (state.frozen) {
            return state;
        }
        
        // Lower background water increment
        double background = 0.05;
        
        // Lower deposition rate per frozen neighbor
        double depositionPerNeighbor = 0.2;
        double deposition = 0.0;
        
        // Accumulate deposition from frozen neighbors.
        for (const auto& [neighborId, neighborData] : neighborhood) {
            if (neighborData.state->frozen) {
                deposition += depositionPerNeighbor;
            }
        }
        
        // Update water accumulation
        state.water += background + deposition;
        
        // Freeze the cell if the water threshold is reached.
        // You can keep the threshold at 1.0 or adjust it if needed.
        if (state.water >= 1.0) {
            state.frozen = true;
            state.water = 1.0;  // Cap the water value.
        }
        
        return state;
    }

    // The outputDelay function returns a constant delay (here, 1.0 time unit).
    [[nodiscard]] double outputDelay(const snowflakeState& state) const override {
        return 1.0;
    }
};

#endif // SNOWFLAKE_CELL_HPP
