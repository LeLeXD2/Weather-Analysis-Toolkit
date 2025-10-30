#include <iostream>
#include "MetozaMain.h"
#include "CSVReader.h"
#include <unordered_set>

MetozaMain::MetozaMain() {}

void MetozaMain::init() {
    int mainOption;
    while (true) {
        printMenu();
        mainOption = getMainOption();
        processMainOption(mainOption);
    }
}

void MetozaMain::printMenu() {
    std::cout << "Welcome to Metoza's Weather forecast!" << std::endl;
    //print help (description for each option)
    std::cout << "1: Print help" << std::endl;
    //print cadlestick data for a particular location in europe
    std::cout << "2: Print past data on table."  << std::endl;
    //Show text-based plot of the candlestick data for a particular location in europe
    std::cout << "3: Print past data on text-based plot"  << std::endl;
    //Predicting data and plotting of my choice
    std::cout << "4: Predict data and plot."  << std::endl;

    //Spacing
    std::cout << "=================================" << std::endl;
}

void MetozaMain::printHelp() {
    std::cout << "Our aim is to show past weather data and predict future forecast." << std::endl;
    std::cout << "Description for each option." << std::endl;
    std::cout << "2 - Printing table yearly past data based on country " << std::endl;
    std::cout << "3 - Printing text-based plot graph based on each country, daily, monthly, yearly" << std::endl;
    std::cout << "4 - Pedicting the weather forecast and plot the graph based on daily." << std::endl;
}

//Check if country exist in the database
std::unordered_set<std::string> MetozaMain::CountryChecker(){
    std::unordered_set<std::string> validCountries = {
        "AT", "BE", "BG", "CH", "CZ", "DE", "DK", "EE", // Add all 28 country codes
        "ES", "FI", "FR", "GB", "GR", "HR", "HU", "IE",
        "IT", "LT", "LU", "LV", "NL", "NO", "PL", "PT",
        "RO", "SE", "SI", "SK",
    };

    return validCountries;
}

bool MetozaMain::DateRangeChecker(const std::string& dateRange) {
    // Check if the string is exactly 9 characters long
    if (dateRange.length() != 9) {
        return false;
    }

    // Check if the 5th character is a hyphen '-'
    if (dateRange[4] != '-') {
        return false;
    }

    //check if the first 4 characters are digits and larger than 2019
    try {
        //extract and convert first and last 4 characters to an integer
        int firstYear = std::stoi(dateRange.substr(0, 4)); 
        int lastYear = std::stoi(dateRange.substr(5, 4));

        if (firstYear <= 2019) {
            return false;
        }

        if (lastYear < firstYear){
            return false;
        }
    } catch (const std::invalid_argument& e) {
        return false;  // Handle invalid conversion if the first 4 characters aren't digits
    }



    // Check if the first 4 characters and last 4 characters are digits
    for (int i = 0; i < 4; ++i) {
        if (!std::isdigit(dateRange[i])) {
            return false;
        }
    }

    for (int i = 5; i < 9; ++i) {
        if (!std::isdigit(dateRange[i])) {
            return false;
        }
    }

    return true;
}


void MetozaMain::printCandlestickData() {
    std::cout << "Candlestick Data." << std::endl;
    while (true){
        std::string countryOption;
        std::string periodOption;
        std::string dateOption;
        std::cout << "Select a country (e.g., AT, GB, LV): ";

        std::cin >> countryOption;
        std::unordered_set<std::string> validCountries = CountryChecker();
        //to break the loop and go back to the start
        if (countryOption == "q"){
            break;
        }else if (validCountries.find(countryOption) == validCountries.end()) {
            std::cout << "You have entered an invalid data" << countryOption << std::endl;
        } 
        else{
            // Display the options to the user
            std::cout << "=================================" << std::endl;
            std::cout << "Choose a time period"<< std::endl;
            std::cout << "1. Yearly" << std::endl;
            std::cout << "2. Monthly" << std::endl;
            std::cout << "3. Daily" << std::endl;
            std::cout << "Enter your choice (1, 2, or 3): ";

            // Get the user's choice
            std::cin >> periodOption;     
            std::cout << "=================================" << std::endl;

            if (periodOption == "1"){
                pastData.printWeatherData(pastData.getCandlestickData(countryOption, dateOption, periodOption), countryOption, periodOption, dateOption);
            }else if (periodOption == "2"){
                std::cout << "Enter in year (e.g., 1980): ";
                std::cin >> dateOption;
                if (dateOption == "q" || dateOption == "Q"){
                    break;
                }else{
                    pastData.printWeatherData(pastData.getCandlestickData(countryOption, dateOption, periodOption), countryOption, periodOption, dateOption);
                }
            }else if (periodOption == "3"){
                std::cout << "Enter in year-month(e.g., 1980-08): ";
                std::cin >> dateOption;
                pastData.printWeatherData(pastData.getCandlestickData(countryOption, dateOption, periodOption), countryOption, periodOption, dateOption);
            }else if (periodOption == "q"){
                break;
            }else{
                std::cout << "You have chosen an invalid option" << std::endl;
            }
        }
    }
}

void MetozaMain::printPlot() {
    std::cout << "Text-base plot graph." << std::endl;
    while (true){
        std::string countryOption;
        std::string periodOption;
        std::string dateOption;
        std::cout << "Select a country (e.g, AT, GB, LV): ";
        std::cin >> countryOption;
        //to break the loop and go back to the start
        std::unordered_set<std::string> validCountries = CountryChecker();
        if (countryOption == "q" ){
            break;
        }else if (validCountries.find(countryOption) == validCountries.end()) {
            std::cout << "You have entered an invalid data" << countryOption << std::endl;
        } 
        else{
            
            // Display the options to the user
            std::cout << "=================================" << std::endl;
            std::cout << "Choose a time period"<< std::endl;
            std::cout << "1. Yearly" << std::endl;
            std::cout << "2. Monthly" << std::endl;
            std::cout << "3. Daily" << std::endl;
            std::cout << "Enter your choice (1, 2, or 3): ";

            // Get the user's choice
            std::cin >> periodOption;     
            std::cout << "=================================" << std::endl;

            if (periodOption == "1"){
                pastData.printGraph(pastData.getCandlestickData(countryOption, dateOption, periodOption), dateOption, periodOption, countryOption);
            }else if (periodOption == "2"){
                std::cout << "Enter in year (e.g., 1980): ";
                std::cin >> dateOption;
                pastData.printGraph(pastData.getCandlestickData(countryOption, dateOption, periodOption), dateOption, periodOption, countryOption);
            }else if (periodOption == "3"){
                std::cout << "Enter in year-month(e.g., 1980-08): ";
                std::cin >> dateOption;
                pastData.printGraph(pastData.getCandlestickData(countryOption, dateOption, periodOption), dateOption, periodOption, countryOption);
            }else if (periodOption == "q"){
                break;
            }else{
                std::cout << "You have chosen an invalid option" << std::endl;
            }
        }
    }
}

void MetozaMain::printPredict() {
    std::cout << "Predict data" << std::endl;
    while (true){
        std::string countryOption;
        std::string dateRange;
        std::cout << "Select a country (e.g., AT, GB, LV): ";
        std::cin >> countryOption;
        //to break the loop and go back to the start
        std::unordered_set<std::string> validCountries = CountryChecker();
        if (countryOption == "q" ){
            break;
        }else if (validCountries.find(countryOption) == validCountries.end()) {
            std::cout << "You have entered an invalid data" << countryOption << std::endl;
        } 
        else{
            // Display the options to the user
            std::cout << "=================================" << std::endl;
            std::cout << "Select the date range (e.g., 2020-2025): ";
            // Get the user's choice
            std::cin >> dateRange;     
            std::cout << "=================================" << std::endl;
             if (DateRangeChecker(dateRange)) {
                pastData.printPredictedGraph(pastData.getPredictedData(countryOption, dateRange), dateRange, countryOption);
            } else {
                std::cout << "Invalid input. Please enter a valid date range in the format YYYY-YYYY/the first year is larger than the last year/first year is larger than 2019" << std::endl;
            }

        }
    }
}

int MetozaMain::getMainOption() {
    int mainOption;

    std::cout << "Select (1 - 4): ";
    std::cin >> mainOption;
    return mainOption;
}

void MetozaMain::processMainOption(int mainOption) {
    switch (mainOption) {
        case 1:
            printHelp();
            break;
        case 2:
            printCandlestickData();
            break;
        case 3:
            printPlot();
            break;
        case 4:
            printPredict();
            break;
        default:
            std::cout << "You have entered an invalid option!" << std::endl;
            break;
    
    }
    //add a spacing
    std::cout << std::endl;
}