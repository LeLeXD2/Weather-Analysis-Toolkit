#include <iostream>
#include "PastData.h"
#include "CSVReader.h"
#include <map>
#include <functional>
#include <numeric>
#include <iomanip> //its for std::setw to control column width
#include <sstream>
#include <cmath> //using std::ceil to round up
#include <vector>
#include <limits> // Include this for std::numeric_limits
#include <deque> // store the data

//constuctor to read past data from csv
PastData::PastData(std::string filename){
    weather = CSVReader::readCSV(filename);
}

//TASK 1
//calculate the yearly mean
double PastData::calculateMean(const std::vector<std::vector<double>>& data){
    double yearTotal = 0;
    int count = 0;

    for (const auto& values : data) {
        double currentSum = 0;
        
        // Sum the values in the current vector
        for (const auto& val : values) {
            currentSum += val;
        }

        yearTotal += currentSum; // Add to the total for the year
        count++; // Increment the number of entries
    }

    //calculate and return the mean if there are any entries
    return (count > 0) ? yearTotal / count : 0;
}

//function to find the average temperature
double PastData::calculateAverageTemperature(const std::vector<double>& temps) {
    double total = std::accumulate(temps.begin(), temps.end(), 0.0);
    return total / temps.size();
}

//predict using average to predict the value for the first row open column
double PastData::predictWithAverage(const std::map<int, std::vector<std::vector<double>>>& candletick) {
    std::vector<double> temperatures;

    //collect temperature data from all years
    for (const auto& entry : candletick) {
        double temp = calculateMean(entry.second);
        temperatures.push_back(temp);
    }

    //calculate average temperature of all other years
    double averageTemp = calculateAverageTemperature(temperatures);

    std::cout << "Predicted temperature for the first Open row(using average): " << averageTemp << std::endl;

    return averageTemp;
}

//function to find the highest value in the data
double PastData::calculateHighest(const std::vector<std::vector<double>>& data){
    double highest_value = -100;
    for (const auto& e : data) {
        for (const auto& val : e){

            //replace the value in highest_value with val if the val is bigger
            if (highest_value < val){
                highest_value = val;
            }else{
                continue; //continue the loop if its not
            }
        }
    }

    return highest_value;
}

double PastData::calculateLowest(const std::vector<std::vector<double>>& data){
    //the value is = 100 the first value can be appended into it
    double lowest_value = 100;
    for (const auto& e : data){
        for (const auto& val: e){
            if (val < lowest_value){
                lowest_value = val;
            }else{
                continue;
            }
        }
    }

    return lowest_value;
}

double PastData::customRound(double num) {
    //multiply by 100 to move the second decimal place
    double temp = num * 100.0;

    //extract the fractional part and check the second decimal place
    if (std::fmod(temp, 1.0) >= 0.5) {
        //round up if >= 0.5
        return std::ceil(temp) / 100.0;
    } else {
        //round down if < 0.5
        return std::floor(temp) / 100.0;
    }
}

//append all the calculated data into a vector double double 
std::vector<std::vector<double>> PastData::TableData(const std::map<int, std::vector<std::vector<double>>>& candlestick, const std::string& dataOption){
    //Initialize data for the table
    std::vector<std::vector<double>> tabledata;
    std::vector<double> close_column, open_column, high_column, low_column;

    //append candlestick data into OpenMap
    std::map<int, std::vector<std::vector<double>>> Updatedcandlestick = candlestick;

    //get the first key 
    auto firstElement = Updatedcandlestick.begin();  // Iterator to the first element
    std::string firstKey = std::to_string(firstElement->first);

    //This removes the first data which is for the first entry Open
    if (dataOption == "1980") {
    }else if (dataOption == "1980-01") {
    }else if (firstKey == "1980") {
    }else{
        Updatedcandlestick.erase(Updatedcandlestick.begin());
    }

    //push all the keys into tabledata
    for (const auto& e: Updatedcandlestick){
        double key = e.first;

        tabledata.push_back({key});
    }

    //append date_column into tabledata
    //PRINTING CLOSE DATA FOR EACH YEAR
    // Loop through all years in the candlestick map 
    for (const auto& e : Updatedcandlestick) {
        double close = customRound(calculateMean(e.second));

        //append data of all years
        close_column.push_back(close);

    }

    //PRINTING OPEN DATA FOR EACH YEAR
    std::map<int, std::vector<std::vector<double>>> OpenData = candlestick;

    //check if the first key is the same as (1980, 1980-01,1980-01-01) 
    if (firstKey == "1980") {
        OpenData[0].push_back({predictWithAverage(OpenData)});
        //Remove the last data from OpenData
        OpenData.erase(std::prev(OpenData.end()));

    }else if (dataOption == "1980") {
        OpenData[0].push_back({predictWithAverage(OpenData)});
        //Remove the last data from OpenData
        OpenData.erase(std::prev(OpenData.end()));

    }else if (dataOption == "1980-01") {
        OpenData[0].push_back({predictWithAverage(OpenData)});
        //Remove the last data from OpenData
        OpenData.erase(std::prev(OpenData.end()));
    }else{
        OpenData.erase(std::prev(OpenData.end()));
    }

    // Loop through all years in the candlestick map
    for (const auto& e : OpenData) {
        // Calculate the "open" for the year
        double open = customRound(calculateMean(e.second));

        //append data of all years
        open_column.push_back(open);
    }

    //PRINTING HIGHEST VALUE IN TIME FRAME
    for (const auto& e: Updatedcandlestick){
        double highest_val = customRound(calculateHighest(e.second));
        

        //append data of all years
        high_column.push_back(highest_val);
    }

    //PRINTING LOWEST VALUE IN TIME FRAME
    for (const auto& e: Updatedcandlestick){
        double lowest_val = customRound(calculateLowest(e.second));

        //append data of all years
        low_column.push_back(lowest_val);
    }

    //this is to ensure that the number of rows in tabledata matches the 4 columns
    size_t num_rows = tabledata.size();

    //appened each column into tabledata
    for (size_t i = 0; i < num_rows; ++i) {
        tabledata[i].push_back(open_column[i]);
        tabledata[i].push_back(high_column[i]);
        tabledata[i].push_back(low_column[i]);
        tabledata[i].push_back(close_column[i]);
    }

    return tabledata;
}

void PastData::printTable(const std::vector<std::string>& headers, const std::vector<std::vector<double>>& data) {
    //print headers
    for (const auto& header: headers) {
        std::cout << std::setw(10) << header;
    }
    std::cout << std::endl;

    //print separator line
    for (size_t i = 0; i < headers.size(); ++i) {
        std::cout << "----------";
    }
    std::cout<< std::endl;

    //print the data
    for (const auto& row : data){
        for (const auto& cell : row){
            std::cout << std::setw(10) << cell;
        }
        std::cout << std::endl;
    }

}

//printing data
void PastData::printWeatherData(const std::map<int, std::vector<std::vector<double>>>& candlestick, const std::string& counutryOption, const std::string& periodOption, const std::string& dateOption){
    //check if the data goes through
    if (candlestick.empty()){
        std::cout << "You have entered an invalid input or the data range is not within our dataset" << std::endl;
    }else{
        std::cout << "Total entries: " << weather.size() << std::endl;

        std::cout << "CANDLESTICK DATA FOR " << counutryOption << " " << periodOption<< std::endl;

        if (periodOption == "1"){
            //header for table
            std::vector<std::string> headers = {"Year", "Open", "High", "Low", "Close"};
            std::vector<std::vector<double>> datatable = TableData(candlestick, dateOption);
            //prints the table
            printTable(headers, datatable);
        }else if (periodOption == "2"){
            //header for table
            std::vector<std::string> headers = {"Month", "Open", "High", "Low", "Close"};
            std::vector<std::vector<double>> datatable = TableData(candlestick, dateOption);
            //prints the table
            printTable(headers, datatable);
        }else{
            std::vector<std::string> headers = {"Day", "Open", "High", "Low", "Close"};
            std::vector<std::vector<double>> datatable = TableData(candlestick, dateOption);
            //prints the table
            printTable(headers, datatable);
        }
    }


} 

//TASK 2
//custom rounding function based on fractional part
auto round_based_on_fraction = [](double num) {
    double fractional_part = num - std::floor(num);
    if (fractional_part >= 0.5) {
        return std::ceil(num);  //round up
    } else {
        return std::floor(num);  //round down
    }
};

//find maxIn high and low column
double PastData::findMaxInColumn(const std::vector<std::vector<double>>& tabledata, int columnIndex) {
    double max = std::numeric_limits<double>::lowest(); // Initialize to the smallest possible value
    size_t index = columnIndex;

    for (const auto& row : tabledata) {
        if (index < row.size()) { // Check if the column index exists in the current row
            if (row[columnIndex] > max) {
                max = row[columnIndex];
            }
        }
    }

    return max;
}

//find the min value in a specific column
double PastData::findSmallestInColumn(const std::vector<std::vector<double>>& tabledata, int columnIndex) {
    double min = std::numeric_limits<double>::max(); // Initialize to the largest possible double value
    size_t index = columnIndex;

    for (const auto& row : tabledata) {
        if (index < row.size()) { //check if the column index is within bounds
            min = std::min(min, row[columnIndex]);
        }
    }

    return min;
}

//render the graph
void PastData::renderGraph(std::vector<std::vector<double>> tabledata, double maxHeight, double lowestHeight){
    std::cout << "Rendering graph......" << std::endl;

    //calculate the highest and lowest point of the axis
    double high = round_based_on_fraction(maxHeight);
    double low = round_based_on_fraction(lowestHeight);


    for (int i=high; i>=low; --i){
        //convert integer to string
        std::string numberStr = std::to_string(i);
        // Get the length of the string
        size_t length = numberStr.size();  // or numberStr.length()
        if (length == 3){
            std::cout << i << "|" ;
        }else if (length == 2){
            std::cout << i << " |";
        }else{
            std::cout << i << "  |";
        }
        //creating the candlestick
        //create the box
        for (const auto& values: tabledata){
            double roundedOpen = round_based_on_fraction(values[1]);
            double roundedHigh = round_based_on_fraction(values[2]);
            double roundedLow = round_based_on_fraction(values[3]);
            double roundedClose = round_based_on_fraction(values[4]);

            if (roundedClose > roundedOpen){
                //printing high
                for (int h=roundedClose + 1; h<=roundedHigh; h++){
                    if (h == i){
                        std::cout << "    " << "|" << "    ";
                    }
                }
                //print box
                for (int k=roundedOpen; k<=roundedClose; k++){
                    if (k == i){
                        std::cout << "   " << "###" << "   ";
                    }
                }
                //printing low
                for (int l=roundedLow; l<roundedOpen; l++){
                    if (l == i){
                        std::cout << "    " << "|" << "    ";
                    }
                }
            }else if (roundedOpen > roundedClose){
                //printing high
                for (int a=roundedOpen + 1; a<=roundedHigh; a++){
                    if (a == i){
                        std::cout << "    " << "|" << "    ";
                    }
                }
                //print box
                for (int j=roundedClose; j<=roundedOpen; j++){
                    if (j == i){
                        std::cout << "   " << "###" << "   ";
                    }
                }
                //printing low
                for (int p=roundedLow; p<roundedClose; p++){
                    if (p == i){
                        std::cout << "    " << "|" << "    ";
                    }
                }
            }else if (roundedClose == roundedOpen){
                //printing high
                for (int f=roundedOpen + 1; f<=roundedHigh; f++){
                    if (f == i){
                        std::cout << "    " << "|" << "    ";
                    }
                }
                //print box
                if (roundedOpen == i){
                    std::cout << "   " << "###" << "   ";
                }
                //printing low
                for (int d=roundedLow; d<roundedOpen; d++){
                    if (d == i){
                        std::cout << "    " << "|" << "    ";
                    }
                }
            }

            //fill in the rest of the empty space for that year
            if (roundedLow >= roundedClose){
                if (i < roundedClose) {
                    std::cout << "         ";
                }
            }else if (roundedLow >= roundedOpen){
                if (i < roundedOpen) {
                    std::cout << "         ";
                }
            }else{
                if (i < roundedLow) {
                    std::cout << "         ";
                }
            }

            if (roundedHigh <= roundedClose){
                if (i > roundedClose) {
                    std::cout << "         ";
                }
            }else if (roundedHigh <= roundedOpen){
                if (i > roundedOpen) {
                    std::cout << "         ";
                }
            }else{
                if (i > roundedHigh) {
                    std::cout << "         ";
                }
            }
        }
        std::cout << std::endl;
    }
}

void PastData::printGraph(const std::map<int, std::vector<std::vector<double>>>& candlestick, const std::string& dataOption, const std::string& periodOption, const std::string& countryOption){
    if(candlestick.empty()){
        std::cout << "You have entered an invalid input or the data range is not within our dataset" << std::endl;
    }else{
        std::cout << "CANDLESTICK DATA FOR " << countryOption << " " << dataOption<< std::endl;
        std::vector<std::vector<double>> datatable = TableData(candlestick, dataOption);
        renderGraph(datatable, findMaxInColumn(datatable, 2), findSmallestInColumn(datatable, 3));
        if (periodOption == "1"){
            for (size_t i=1; i<=datatable.size(); ++i) {
                std::cout << "----------";
            }
            std::cout << std::endl;
            std::cout << "   ";
            for (const auto& values: datatable){
                int year = values[0];
                std::cout << "   " <<  year << "  ";
            }
            std::cout << std::endl;
        }else{
            for (size_t i=1; i<=datatable.size(); ++i) {
                std::cout << "---------";
            }
            std::cout << std::endl;
            std::cout << "   ";
            for (const auto& values: datatable){
                int date = values[0];
                std::string str = std::to_string(date);
                if(str.length() == 1){
                    std::cout << "     0" <<  date << "  ";
                }else{
                    std::cout << "     " <<  date << "  ";
                }
            }
            std::cout << std::endl;
        }
    }

}

//TASK 3
std::map<std::string, std::function<double(const WeatherDataEntry&)>> PastData::CountryGetter(){
    //map country codes to their corresponding getter function
    std::map<std::string, std::function<double(const WeatherDataEntry&)>> countryToGetter = {
        {"AT", &WeatherDataEntry::getAT},
        {"BE", &WeatherDataEntry::getBE},
        {"BG", &WeatherDataEntry::getBG},
        {"CH", &WeatherDataEntry::getCH},
        {"CZ", &WeatherDataEntry::getCZ},
        {"DE", &WeatherDataEntry::getDE},
        {"DK", &WeatherDataEntry::getDK},
        {"EE", &WeatherDataEntry::getEE},
        {"ES", &WeatherDataEntry::getES},
        {"FI", &WeatherDataEntry::getFI},
        {"FR", &WeatherDataEntry::getFR},
        {"GB", &WeatherDataEntry::getGB},
        {"GR", &WeatherDataEntry::getGR},
        {"HR", &WeatherDataEntry::getHR},
        {"HU", &WeatherDataEntry::getHU},
        {"IE", &WeatherDataEntry::getIE},
        {"IT", &WeatherDataEntry::getIT},
        {"LT", &WeatherDataEntry::getLT},
        {"LU", &WeatherDataEntry::getLU},
        {"LV", &WeatherDataEntry::getLV},
        {"NL", &WeatherDataEntry::getNL},
        {"NO", &WeatherDataEntry::getNO},
        {"PL", &WeatherDataEntry::getPL},
        {"PT", &WeatherDataEntry::getPT},
        {"RO", &WeatherDataEntry::getRO},
        {"SE", &WeatherDataEntry::getSE},
        {"SI", &WeatherDataEntry::getSI},
        {"SK", &WeatherDataEntry::getSK},
    };

    return countryToGetter;
}

//get data and filter based on input
std::map<int, std::vector<std::vector<double>>> PastData::getCandlestickData(const std::string& country,const std::string& dateOption, std::string& periodOption){
    //map country codes to their corresponding getter function
    std::map<std::string, std::function<double(const WeatherDataEntry&)>> countryToGetter = CountryGetter();
    
    //initialize the map for candlestick data
    std::map<int, std::vector<std::vector<double>>> candlestick;

    //get the function for the selected country
    auto getter = countryToGetter.find(country)->second;

    int count = 0;
    std::deque<WeatherDataEntry> buffer;

    for (WeatherDataEntry& e: weather){
        if (periodOption == "1"){
            std::string date = e.getTimestamp().substr(0, 4);//cutting the timestamp to 0000
            int year = std::stoi(e.getTimestamp().substr(0, 4));
            candlestick[year].push_back({getter(e)});

        }else if (periodOption == "2"){
            std::string date = e.getTimestamp().substr(0, 4);//cutting the timestamp to 0000
            if (date == dateOption){
                int months = std::stoi(e.getTimestamp().substr(5, 2));
                if (months == 1){
                    count += 1;
                    if (count == 1){
                        for (WeatherDataEntry& e: buffer){
                            candlestick[0].push_back({getter(e)});
                        }
                        
                    }
                }
                candlestick[months].push_back({getter(e)});
            }
                
            //add the current entry to the buffer
            buffer.push_back(e);

            //remove the oldest entry if the buffer exceeds 31 entries
            if (buffer.size() > 31) {
                buffer.pop_front();
            }
        }else if (periodOption == "3"){
            std::string date = e.getTimestamp().substr(0, 7);//cutting the timestamp to 0000-00-00
            if (date == dateOption){
                int days = std::stoi(e.getTimestamp().substr(8, 2));
                if (days == 1) {
                    count += 1;
                    if (count == 1){
                        for (WeatherDataEntry& e: buffer){
                            candlestick[0].push_back({getter(e)});
                        }
                    }
                }
                candlestick[days].push_back({getter(e)});
            }

            //add the current entry to the buffer
            buffer.push_back(e);

            //remove the oldest entry if the buffer exceeds 12 entries
            if (buffer.size() > 24) {
                buffer.pop_front();
            }
        }
    }

    return candlestick;
}

//TASK 4
//function to calculate the mean of a vector for predictive model
double PastData::calMean(const std::vector<double>& data) {
    double total = std::accumulate(data.begin(), data.end(), 0.0);
    return total / data.size();
}

//function to calculate difference for the predictive model
std::vector<double> PastData::calDifference(const std::vector<double>& data) {
    std::vector<double> difference;
    for (size_t i = 1; i < data.size(); ++i) {
        difference.push_back(data[i] - data[i - 1]);
    }
    return difference;
}

//predict Future Value Using ARIMA
double PastData::predictNextYear(const std::vector<double>& data, int lag, double AR) {
    double mean = calMean(data);
    double prediction = mean; //atart with the mean as the base prediction

    //add autoregressive terms 
    for (int i = 1; i <= lag; ++i) {
        if (data.size() > static_cast<size_t>(i)) {
            prediction += AR * data[data.size() - i]; // Simplified AR coefficient
        }
    }

    return prediction;
}

//printing the graph after filtering by the year
void PastData::printPredictedGraph(std::vector<std::vector<double>> tabledata, const std::string& dateRange, const std::string& countryOption){
    if(tabledata.empty()){
        std::cout << "You have entered an invalid input or the data range is not within our dataset" << std::endl;
    }else{
        std::cout << "PREDICTED CANDLESTICK DATA FOR " << countryOption << " " << dateRange<< std::endl;
        renderGraph(tabledata, findMaxInColumn(tabledata, 2), findSmallestInColumn(tabledata, 3));
        for (size_t i=1; i<=tabledata.size(); ++i) {
            std::cout << "----------";
        }
        std::cout << std::endl;
        std::cout << "   ";
        for (const auto& values: tabledata){
            int year = values[0];
            std::cout << "   " <<  year << "  ";
        }
        std::cout << std::endl;
        
    }
}

std::vector<std::vector<double>> PastData::getPredictedData(const std::string& country,const std::string& dateRange){
    //map country codes to their corresponding getter function
    std::map<std::string, std::function<double(const WeatherDataEntry&)>> countryToGetter = CountryGetter();
    
    //initialize the map for candlestick data
    std::map<int, std::vector<std::vector<double>>> candlestick;

    //get the function for the selected country
    auto getter = countryToGetter.find(country)->second;

    for (WeatherDataEntry& e: weather){
        std::string date = e.getTimestamp().substr(0, 4);//cutting the timestamp to 0000
        int year = std::stoi(e.getTimestamp().substr(0, 4));
        candlestick[year].push_back({getter(e)});
    }   

    //extract and put all the data into individual vector<double>
    std::vector<std::vector<double>> tabledata = TableData(candlestick, dateRange); 
    std::vector<double>open, high, low, close;
    
    for (size_t col = 0; col < tabledata[0].size(); ++col){
        for (const auto& row: tabledata){
            if (col == 1){
                open.push_back(row[col]);
            }
            if (col == 2){
                high.push_back(row[col]);
            }
            if (col == 3){
                low.push_back(row[col]);
            }
            if (col == 4){
                close.push_back(row[col]);
            }
        }
    }
    
    //extract and convert first and last 4 characters to an integer
    int firstYear = std::stoi(dateRange.substr(0, 4)); 
    int lastYear = std::stoi(dateRange.substr(5, 4));

    //extract the latest year from the past dataset
    int latestYear = tabledata.back()[0];

    //difference between first year and latest year
    int diffYear = firstYear - latestYear - 1;

    //make data stationary by doing differencing
    std::vector<double> diffOpenPrices = calDifference(open);
    std::vector<double> diffHighPrices = calDifference(high);
    std::vector<double> diffLowPrices = calDifference(low);
    std::vector<double> diffClosePrices = calDifference(close);

    //predict future values
    int yearRange = lastYear - latestYear; //the highest year - the last year in the dataset (2019)
    int lag = 2; //predict based on the previous 2 year

    std::vector<double> predictedYear, openPredictions, highPredictions, lowPredictions, closePredictions;
    double lastOpen = open.back();
    double lastHigh = high.back();
    double lastLow = low.back();
    double lastClose = close.back();

    for (int i = 0; i < yearRange; ++i){
        //predicting for open
        double nextOpenDiffValue = predictNextYear(diffOpenPrices, lag, 0.3);
        double nextOpenValue = lastOpen + nextOpenDiffValue;

        openPredictions.push_back(nextOpenValue);

        // Update for the next iteration
        diffOpenPrices.push_back(nextOpenDiffValue);
        lastOpen = nextOpenValue;

        //predicting for high
        double nextHighDiffValue = predictNextYear(diffHighPrices, lag, 0.3);
        double nextHighValue = lastHigh + nextHighDiffValue;

        highPredictions.push_back(nextHighValue);

        // Update for the next iteration
        diffHighPrices.push_back(nextHighDiffValue);
        lastHigh = nextHighValue;

        //predicting for low
        double nextLowDiffValue = predictNextYear(diffLowPrices, 5, 0.1);
        double nextLowValue = lastLow + nextLowDiffValue;

        lowPredictions.push_back(nextLowValue);

        // Update for the next iteration
        diffLowPrices.push_back(nextLowDiffValue);
        lastLow = nextLowValue;

        //predicting for close
        double nextCloseDiffValue = predictNextYear(diffClosePrices, lag, 0.3);
        double nextCloseValue = lastClose + nextCloseDiffValue;

        closePredictions.push_back(nextCloseValue);

        // Update for the next iteration
        diffClosePrices.push_back(nextCloseDiffValue);
        lastClose = nextCloseValue;
    }

    //append the year range from the latest year
    for (int i = latestYear + 1; i <= lastYear; ++i) {
        predictedYear.push_back(i);
    }

    //remove the data from the gap between the first year of the user input and the end year of our dataset
    predictedYear.erase(predictedYear.begin(), predictedYear.begin() + diffYear);
    openPredictions.erase(openPredictions.begin(), openPredictions.begin() + diffYear);
    highPredictions.erase(highPredictions.begin(), highPredictions.begin() + diffYear);
    lowPredictions.erase(lowPredictions.begin(), lowPredictions.begin() + diffYear);
    closePredictions.erase(closePredictions.begin(), closePredictions.begin() + diffYear);

    //append into a predictedtable
    std::vector<std::vector<double>> predictedTable;

    for (const auto& year: predictedYear){
        predictedTable.push_back({year});
    }

    //append open
    size_t num_rows = predictedTable.size();
    for (size_t i = 0; i < num_rows; ++i) {
        predictedTable[i].push_back(openPredictions[i]);
        predictedTable[i].push_back(highPredictions[i]);
        predictedTable[i].push_back(lowPredictions[i]);
        predictedTable[i].push_back(closePredictions[i]);
    }

    return predictedTable;
}