# Snowflake Growth Simulation Using Cell-DEVS and Cadmium
---
## 1. Introduction
This project demonstrates modeling and simulation using the Cell-DEVS formalism with the Cadmium tool. The simulation represents the formation of a snowflake; a natural pattern formation process using a cellular model. In this model, each cell maintains a water accumulation value and a frozen state. The evolution of the snowflake is driven by several factors including background vapor supply, deposition from neighboring frozen cells, evaporation, diffusion, and random noise. The final output is visualized using multiple color gradients to represent the water level (from low water to fully frozen).
---
## 2. Project Description
The simulation models a 50×50 grid where:
- **Cells** have two state variables: 
  - `water` (a value between 0 and 1) that represents the accumulated water.
  - `frozen` (a boolean) indicating whether the cell has frozen.
- **The seed cell** is placed at the center ([25, 25]) with a fully frozen state (water = 1.0).
- **Factors affecting water accumulation** include:
  - **Background vapor:** A constant water increment added to every cell.
  - **Deposition:** Additional water received from each frozen neighbor.
  - **Evaporation:** A small fraction of water is lost at each time step.
  - **Diffusion:** A cell's water level is adjusted toward the average water level of its neighbors.
  - **Random Noise:** Minor fluctuations to simulate environmental variability.

The simulation is run over an extended period (e.g., 1000 time units) to capture the gradual evolution of the snowflake pattern.
---
## 3. File Structure
```plaintext
├── CMakeLists.txt
├── bin
│   └── Snowflake_Growth_Model
├── build/
├── build_sim.sh
├── config
│   ├── snowflakeVisualization_config.json
│   └── snowflake_config.json
├── grid_log.csv
└── main
    ├── CMakeLists.txt
    ├── include
    │   ├── snowflakeCell.hpp
    │   └── snowflakeState.hpp
    └── main.cpp
```
---
## 4. How to Build and Run
### Prerequisites
- **Cadmium DEVS Simulator:** [Cadmium DEVS](https://devssim.carleton.ca)
- **C++ Compiler & CMake:** Ensure you have a working C++ development environment.
- **Git:** To clone the repository.

### Steps to Build
1. **Clone the Repository:**
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. **Build the Project:** Use the provided build script:
   ```bash
   source build_sim.sh
   ```

### Steps to Run the Simulation
Run the simulation using the configuration file:
```bash
./bin/Snowflake_Growth_Model config/snowflake_config.json
```
This command will generate a log file (e.g., grid_log.csv).

### Visualizing the Output
Open the generated CSV log with the Cell-DEVS Web Viewer to observe the evolving snowflake pattern.

The visualization uses a color gradient based on the water field:
- **Dark Blue:** Water < 0.25
- **Blue:** 0.25 ≤ Water < 0.5
- **Light Blue:** 0.5 ≤ Water < 0.75
- **White:** Water ≥ 0.75 (indicating frozen cells)
---
## 5. Experimental Framework
### Model Parameters
The parameters for background vapor, deposition, evaporation, diffusion, and noise are defined in `snowflakeCell.hpp` and can be adjusted to fine-tune the simulation.

### Visualization Configuration
The file `snowflake_config.json` defines the scenario and viewer settings, including color breaks for gradual transitions.
