#pragma once

#include "../misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

#include <fstream>
#include <string>

namespace FileManager
{
    struct DefaultConfig {
        const std::string data =
            "[Graphics]\n"
            "ResolutionX=800\n"
            "ResolutionY=600\n"
            "[Audio]\n"
            "MasterVolume=100\n";
    }defaultConfig;

	inline void initializeFileManager()
	{
		log("initializing FileManager", LogLevel::DEBUG);
	}

	// save inventory data to file. Use EntityID as filename. 
	inline int saveInventoryToFile(std::string& filename, const std::string& _inventoryData)
	{
		log("Saving data to file: " + filename);

            // Create an output file stream
            std::ofstream outFile("data/" + filename);

            // Check if the file is open
            if (outFile.is_open()) {
                //std::string dataToSave = "Hello, World!";
                outFile << _inventoryData; // Write the string to the file
                outFile.close(); // Close the file
                log("Data saved to file successfully.");
                return 0;
            }
            else {
                log("Error opening file.", LogLevel::ERROR);
                return -1;
            }
	}

	// read inventory data from file
    inline int readInventoryFromFile(const std::string& filename, std::string& out_inventoryData)
    {
        log("Reading data from file: " + filename);
            // Create an input file stream
            std::ifstream inFile(filename);
            // Check if the file is open
            if (inFile.is_open()) {
                std::string line;
                out_inventoryData.clear(); // Clear the output string before reading
                // Read the file line by line
                while (std::getline(inFile, line)) {
                    out_inventoryData += line + "\n"; // Append each line to the output string
                }
                inFile.close(); // Close the file
                log("Data read from file successfully.");
                return 0;
            }
            else {
                log("Error opening file.", LogLevel::ERROR);
                return -1;
            }
	}


    // --------------------------------------------------------------------------------

    // save any block of data to file in 
    inline int saveDataToFile(const std::string _filename, std::string _inventoryData)
    {
        //log("Saving data to file: " + _filename, LogLevel::DEBUG);

        // Create an output file stream
        std::ofstream outFile("data/" + _filename);

        // Check if the file is open
        if (outFile.is_open()) {
            //std::string dataToSave = "Hello, World!";
            outFile << _inventoryData; // Write the string to the file
            outFile.close(); // Close the file
            //log("Data saved to file successfully.");
            return 0;
        }
        else {
            log("Error opening file.", LogLevel::ERROR);
            return -1;
        }
    }

    // read data from file
    inline int readDataFromFile(const std::string& _filename, std::string& out_inventoryData)
    {
        //log("Trying to read data from file: " + _filename, LogLevel::DEBUG);
        // Create an input file stream
        std::ifstream inFile("data/" + _filename);
        // Check if the file is open
        if (inFile.is_open()) {
            std::string line;
            out_inventoryData.clear(); // Clear the output string before reading
            // Read the file line by line
            while (std::getline(inFile, line)) {
                out_inventoryData += line + "\n"; // Append each line to the output string
            }
            inFile.close(); // Close the file
            //log("Data read from file successfully.");
            return 0;
        }
        else {
            log("Error opening file. Does not exist.", LogLevel::ERROR);
            return -1;
        }
    }

    inline void processConfig(std::string& conf) 
    {
        log("Processing config file now...");
        log("Come back to this later...");

    }
}