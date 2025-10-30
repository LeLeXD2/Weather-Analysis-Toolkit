#pragma once

#include "WeatherDataEntry.h"
#include "PastData.h"
#include <vector>
#include <unordered_set>

class MetozaMain
{
    public:
        MetozaMain();
        void init();

    private:
        std::vector<WeatherDataEntry> WeatherData;
        PastData pastData{"weather_data_EU_1980-2019_temp_only.csv"}; 
        void printMenu();
        void printHelp();
        void printCandlestickData();
        void printPlot();
        void printPredict();
        int getMainOption();
        void processMainOption(int mainOption);
        std::unordered_set<std::string> CountryChecker();
        bool DateRangeChecker(const std::string& dateRange);
};