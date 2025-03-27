#include "nlohmann/json.hpp"
#include <cadmium/modeling/celldevs/grid/coupled.hpp>
#include <cadmium/simulation/logger/csv.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <fstream>
#include <iostream>
#include "include/snowflakeCell.hpp"

using namespace cadmium::celldevs;
using namespace cadmium;

// Helper function to instantiate a snowflake cell.
std::shared_ptr<GridCell<snowflakeState, double>> addGridCell(
    const std::vector<int>& cellId,
    const std::shared_ptr<const GridCellConfig<snowflakeState, double>>& cellConfig)
{
    if(cellConfig->cellModel == "snowflake") {
        return std::make_shared<snowflake>(cellId, cellConfig);
    } else {
        throw std::bad_typeid();
    }
}

int main(int argc, char ** argv) {
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " config.json [simulation_time (default 1000)]" << std::endl;
        return -1;
    }
    std::string configFilePath = argv[1];
    double simTime = (argc > 2) ? std::stod(argv[2]) : 1000;  // Extended simulation time.

    // Create the coupled Cell-DEVS model using the provided configuration file.
    auto model = std::make_shared<GridCellDEVSCoupled<snowflakeState, double>>("snowflake", addGridCell, configFilePath);
    model->buildModel();

    // Set up the simulation with a CSV logger.
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<CSVLogger>("grid_log.csv", ";");

    // Start and run the simulation.
    rootCoordinator.start();
    rootCoordinator.simulate(simTime);
    rootCoordinator.stop();

    return 0;
}
