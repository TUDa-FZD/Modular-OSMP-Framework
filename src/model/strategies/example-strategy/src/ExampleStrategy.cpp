//
// Copyright 2019 Technische Universität Darmstadt - Fahrzeugtechnik
//

#include "examplestrategy/ExampleStrategy.hpp"
#include <string>

using namespace model;
using namespace std;

// **main method of strategy**
void ExampleStrategy::apply(osi3::SensorData &sensor_data) {
    // sanity checks
	if (sensor_data.sensor_view_size() == 0) {
        alert("No valid SensorView.");
		return;
	}

    // debugging output
    log("-----------------------------------------------");
    log("number moving objects: " + std::to_string(sensor_data.sensor_view(0).global_ground_truth().moving_object_size()));
    log("number stationary objects: " + std::to_string(sensor_data.sensor_view(0).global_ground_truth().stationary_object_size()));
}
