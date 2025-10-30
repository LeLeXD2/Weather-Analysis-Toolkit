#pragma once

#include "WeatherDataEntry.h"
#include <vector>
#include <string>

class CSVReader
{
    public:
        //constructors
        CSVReader();
        static std::vector<WeatherDataEntry> readCSV(std::string csvFile);

    private:
        static std::vector<std::string> tokenize(std::string csvLine, char sep);
        static WeatherDataEntry stringToOBE(std::vector<std::string> tokens);
};