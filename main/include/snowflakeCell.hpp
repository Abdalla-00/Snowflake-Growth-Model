#ifndef SNOWFLAKE_CELL_HPP
#define SNOWFLAKE_CELL_HPP

#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include "snowflakeState.hpp"
#include <cmath>
#include <random>

using namespace cadmium::celldevs;

class snowflake : public GridCell<snowflakeState, double> {
public:
    // Constructor: simply pass the cell id and configuration to the base class.
    snowflake(const std::vector<int>& id,
              const std::shared_ptr<const GridCellConfig<snowflakeState, double>>& config)
      : GridCell<snowflakeState, double>(id, config) { }

    [[nodiscard]] snowflakeState localComputation(
        snowflakeState state,
        const std::unordered_map<std::vector<int>, NeighborData<snowflakeState, double>>& neighborhood
    ) const override {
        // If the cell is already frozen, no further updates are made.
        if (state.frozen) {
            return state;
        }
        
        // 1. Background and Deposition: Very gradual increments.
        double background = 0.02;             // Base water added each time step.
        double depositionPerNeighbor = 0.05;    // Additional water from each frozen neighbor.
        double deposition = 0.0;
        
        for (const auto& [neighborId, neighborData] : neighborhood) {
            if (neighborData.state->frozen) {
                deposition += depositionPerNeighbor;
            }
        }
        
        state.water += background + deposition;
        
        // 2. Evaporation: Remove a small fraction of water each step.
        double evaporationRate = 0.005;  // 0.5% evaporation per step.
        state.water *= (1.0 - evaporationRate);
        
        // 3. Diffusion: Adjust water toward the average water level of neighbors.
        double diffusionCoefficient = 0.01;  
        double neighborWaterSum = 0.0;
        int neighborCount = 0;
        for (const auto& [neighborId, neighborData] : neighborhood) {
            neighborWaterSum += neighborData.state->water;
            neighborCount++;
        }
        if (neighborCount > 0) {
            double neighborWaterAvg = neighborWaterSum / neighborCount;
            double diffusionTerm = diffusionCoefficient * (neighborWaterAvg - state.water);
            state.water += diffusionTerm;
        }
        
        // 4. Random Noise: Add a small random fluctuation.
        // Note: static variables ensure the generator is initialized only once.
        static std::default_random_engine generator;
        static std::uniform_real_distribution<double> distribution(-0.005, 0.005);
        double noise = distribution(generator);
        state.water += noise;
        
        // 5. Freezing Threshold: Once water reaches or exceeds 1.0, the cell freezes.
        if (state.water >= 1.0) {
            state.frozen = true;
            state.water = 1.0;  // Cap the water level.
        }
        
        return state;
    }

    // Each update happens after a constant delay (1 time unit).
    [[nodiscard]] double outputDelay(const snowflakeState& state) const override {
        return 1.0;
    }
};

#endif // SNOWFLAKE_CELL_HPP
