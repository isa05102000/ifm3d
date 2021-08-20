/*
 * Copyright 2021-present ifm electronic, gmbh
 * SPDX-License-Identifier: Apache-2.0
 */

#include <iostream>
#include <iomanip>
#include <memory>
#include <fstream>
#include <ifm3d/camera.h>

// Path to config files are assuming you are running this example from the /build folder.

int main(){

    // Create the camera object
    auto cam = ifm3d::O3RCamera::MakeShared();

    // Get the current configuration of the camera in JSON format
    json conf = cam->ToJSON();
    // Display and then write to file
    std::cout << std::setw(4) << conf << std::endl;
    std::ofstream file_get("conf_get.json");
    file_get << std::setw(4) << conf;

    // Configure the device from a configuration file
    std::ifstream file_set("/home/usmasslo/O3R/ifm3d/examples/tutorials/configuration/conf_set.json");
    file_set >> conf;
    cam->FromJSON(conf);

    // Check that the configuration worked
    conf = cam->ToJSON();
    std::cout << std::setw(4) << conf << std::endl;

    return 0;
}