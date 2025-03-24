#include "nlohmann/json.hpp"
#include <cadmium/modeling/celldevs/grid/coupled.hpp>
#include <cadmium/simulation/logger/csv.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <fstream>
#include <iostream>
#include "include/snowflakeCell.hpp"

using namespace cadmium::celldevs;
using namespace cadmium;

// Helper function to create a snowflake cell from JSON configuration.
std::shared_ptr<GridCell<snowflakeState, double>> addGridCell(
    const std::vector<int>& cellId,
    const std::shared_ptr<const GridCellConfig<snowflakeState, double>>& cellConfig) {
    std::string cellModel = cellConfig->cellModel;
    if(cellModel == "snowflake") {
        return std::make_shared<snowflake>(cellId, cellConfig);
    } else {
        throw std::bad_typeid();
    }
}

int main(int argc, char ** argv) {
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " config_file.json [simulation_time]" << std::endl;
        return -1;
    }
    std::string configFilePath = argv[1];
    double simTime = (argc > 2) ? std::stod(argv[2]) : 100.0;

    // Create the coupled Cell-DEVS model using the configuration file.
    auto model = std::make_shared<GridCellDEVSCoupled<snowflakeState, double>>("snowflake", addGridCell, configFilePath);
    model->buildModel();

    // Initialize the RootCoordinator and set the CSV logger.
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<CSVLogger>("grid_log.csv", ";");

    // Start the simulation.
    rootCoordinator.start();
    rootCoordinator.simulate(simTime);
    rootCoordinator.stop();

    return 0;
}