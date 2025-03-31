#ifndef SNOWFLAKE_CELL_HPP
#define SNOWFLAKE_CELL_HPP

#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include "snowflakeState.hpp"
#include <cmath>

using namespace cadmium::celldevs;

/**
 * Symmetrical hex-based snowflake model:
 * - 6 neighbors in a hex pattern
 * - If exactly 1 neighbor is frozen => strong deposit (branch tip)
 * - Otherwise deposit scales gently with number of frozen neighbors
 * - Very small background
 * - Minimal diffusion/evaporation
 * - No random noise
 */
class snowflake : public GridCell<snowflakeState, double> {
public:
    snowflake(const std::vector<int>& id,
              const std::shared_ptr<const GridCellConfig<snowflakeState, double>>& config)
      : GridCell<snowflakeState, double>(id, config) { }

    [[nodiscard]] snowflakeState localComputation(
        snowflakeState state,
        const std::unordered_map<std::vector<int>, NeighborData<snowflakeState, double>>& neighborhood
    ) const override {
        // If cell is already frozen, no changes.
        if (state.frozen) {
            return state;
        }

        // 1. Very small background vapor.
        double background = 0.005;

        // 2. Compute how many neighbors are frozen.
        int frozenNeighbors = 0;
        for (const auto& [offset, neighborData] : neighborhood) {
            if (neighborData.state->frozen) {
                frozenNeighbors++;
            }
        }

        // 3. Strong "tip" effect if exactly 1 neighbor is frozen; otherwise gentle deposit.
        double deposit = 0.0;
        if (frozenNeighbors == 1) {
            // Tip: big deposit for strong branching
            deposit = 0.2;
        } else {
            // Gentle deposit scaling with number of frozen neighbors
            deposit = 0.02 * frozenNeighbors;
        }

        // Add background + deposit
        state.water += background + deposit;

        // 4. Minimal diffusion: let's do a tiny push toward neighbors.
        double diffusionCoeff = 0.005;
        double neighborWaterSum = 0.0;
        int neighborCount = 0;
        for (const auto& [offset, neighborData] : neighborhood) {
            neighborWaterSum += neighborData.state->water;
            neighborCount++;
        }
        if (neighborCount > 0) {
            double avgWater = neighborWaterSum / neighborCount;
            double diffTerm = diffusionCoeff * (avgWater - state.water);
            state.water += diffTerm;
        }

        // 5. Minimal evaporation
        double evaporationRate = 0.0005;
        state.water *= (1.0 - evaporationRate);

        // 6. No random noise => maintain symmetry

        // 7. Freeze if water >= 1.0
        if (state.water >= 1.0) {
            state.frozen = true;
            state.water = 1.0;
        }

        return state;
    }

    // Output delay is 1 time unit per update.
    [[nodiscard]] double outputDelay(const snowflakeState& state) const override {
        return 1.0;
    }
};

#endif // SNOWFLAKE_CELL_HPP
