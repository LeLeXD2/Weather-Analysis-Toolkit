#include "CSVReader.h"
#include <fstream>
#include <iostream>

CSVReader::CSVReader() {}

std::vector<WeatherDataEntry> CSVReader::readCSV(std::string csvFile){
    std::vector<WeatherDataEntry> entries;

    std::ifstream csvStream{csvFile};
    std::string line;
    std::vector<std::string> tokens;


    if (csvStream.is_open()){
        //iterate through the first line which is the header
        if (std::getline(csvStream, line)){
            //This is the first line which gets skipped
        }

        while (std::getline(csvStream, line)){
            try{
                WeatherDataEntry obe = stringToOBE(tokenize(line, ','));
                entries.push_back(obe);
            }
            catch(std::exception& e){
                std::cout << "Bad data" << std::endl;
                continue;
            }
        }
    }
    else{
        std::cout << "Unable to open file: " << csvFile << std::endl;
    }

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries." << std::endl;

    //check if the data has been appended
    /* for (const auto& entry : entries) {
        std::cout << "Timestamp: " << entry.getTimestamp() << ", AT: " << entry.getAT() << ", BE: " << entry.getBE() << std::endl;
        // Print other fields similarly as needed
    } */
    return entries;
}

std::vector<std::string> CSVReader::tokenize(std::string csvLine, char sep){
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(sep, 0);

    do{
        end = csvLine.find_first_of(sep, start);
        if (static_cast<std::string::size_type>(start) == csvLine.length() || start == end)
            break;
        if (end >= 0)
        {
            token = csvLine.substr(start, end - start);
        }
        else
        {
            token = csvLine.substr(start, csvLine.length() - start);
        }
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);
    return tokens;
}

WeatherDataEntry CSVReader::stringToOBE(std::vector<std::string> tokens){
    double AT, BE, BG, CH, CZ, DE, DK, EE, ES, FI, FR, GB, GR, HR, HU, IE, IT, LT, LU, LV, NL, NO, PL, PT, RO, SE, SI, SK;
    std::string timestamp;
    
    //check if there is any line with no values
    if (tokens.size() != 29){
        std::cout << "Bad line" << std::endl;
        throw std::exception{};
    }

     try {
        timestamp = tokens[0];  // assuming first token is the timestamp
        AT = std::stod(tokens[1]);  // Convert to double
        BE = std::stod(tokens[2]);
        BG = std::stod(tokens[3]);
        CH = std::stod(tokens[4]);
        CZ = std::stod(tokens[5]);
        DE = std::stod(tokens[6]);
        DK = std::stod(tokens[7]);
        EE = std::stod(tokens[8]);
        ES = std::stod(tokens[9]);
        FI = std::stod(tokens[10]);
        FR = std::stod(tokens[11]);
        GB = std::stod(tokens[12]);
        GR = std::stod(tokens[13]);
        HR = std::stod(tokens[14]);
        HU = std::stod(tokens[15]);
        IE = std::stod(tokens[16]);
        IT = std::stod(tokens[17]);
        LT = std::stod(tokens[18]);
        LU = std::stod(tokens[19]);
        LV = std::stod(tokens[20]);
        NL = std::stod(tokens[21]);
        NO = std::stod(tokens[22]);
        PL = std::stod(tokens[23]);
        PT = std::stod(tokens[24]);
        RO = std::stod(tokens[25]);
        SE = std::stod(tokens[26]);
        SI = std::stod(tokens[27]);
        SK = std::stod(tokens[28]);
        // Print the parsed values
        /* std::cout << "Parsed data: " << std::endl;
        std::cout << "Timestamp: " << timestamp << std::endl;
        std::cout << "AT: " << AT << ", BE: " << BE << ", BG: " << BG << std::endl; */
        // Print other fields as needed
    } catch (const std::exception& e) {
        std::cout << "Error parsing data: " << e.what() << std::endl;
        throw;  // rethrow or handle error
    }

    return WeatherDataEntry(timestamp, AT, BE, BG, CH, CZ, DE, DK, EE, ES, FI, FR, GB, GR, HR, HU, IE, IT, LT, LU, LV, NL, NO, PL, PT, RO, SE, SI, SK);

}
