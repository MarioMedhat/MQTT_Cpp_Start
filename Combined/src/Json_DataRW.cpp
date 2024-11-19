#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "Json_DataRW.hpp"

void writeJsonFile(const std::string& filepath) {
    nlohmann::json jsonData;
    // Simulate data
    jsonData["message"] = "Hello, Subscriber!";
    jsonData["timestamp"] = 5555555;
    jsonData["status"] = "active";

    // Write JSON to file
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << jsonData.dump(4); // Pretty print with 4 spaces
        file.close();
    } else {
        std::cerr << "Failed to write to " << filepath << std::endl;
    }
}

void updateJsonFile(const std::string& filepath, std::string field, int val)
{
    // Create or load the JSON file
    std::ifstream file(filepath);
    nlohmann::json jsonData;

    if (file.is_open() && file.peek() != std::ifstream::traits_type::eof()) {
        // If file exists and is not empty, load the content
        file >> jsonData;

        jsonData[field] = val;
    }
    file.close();
    std::ofstream outfile(filepath);
    outfile << jsonData.dump();
    outfile.close();
}

void readJsonFile(const std::string& filepath) {
    nlohmann::json jsonData;

    // Read JSON from file
    std::ifstream file(filepath);
    if (file.is_open()) {
        try {
            file >> jsonData;
            file.close();

            // Process the data
            //std::cout << jsonData.dump(4) << std::endl;

            // Access individual fields
            std::string message = jsonData["message"];
            int timestamp = jsonData["timestamp"];
            std::string status = jsonData["status"];

            std::cout << "Messags: " << message << "\n"
                      << "Timestamp: " << timestamp << "\n"
                      << "Status: " << status << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Failed to open " << filepath << std::endl;
    }
}

std::string JsonDataToString (const std::string filepath)
{
    nlohmann::json jsonData;
    // Read JSON from file
    std::ifstream file(filepath);
    if (file.is_open()) {
            file >> jsonData;
            file.close();
    }
    return jsonData.dump();
}