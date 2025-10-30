### Weather Analysis toolkit ###
### Product Description ###
This project is a high-performance Command-Line Interface (CLI) toolkit written in C++ for technical analysis of weather data. It supports end-to-end workflows: data ingestion (CSV), exploratory visualization (text-box plot), model training and evaluation (ARIMA), and exporting predictions and reports. The toolkit is built for reproducibility and speed, intended for researchers and engineers who want robust weather analytics without leaving the terminal.

### Features ###
- Menu with input validation
- Candlestick Table data filtered by country, yearly, monthly and daily
- Candlestick Text-Based Plot filtered by country, yearly, monthly and daily
- Candlestick Text-Based Plot for predicted data filtered by country and the date range the user input.

### Tech Stack ###
C++

### Download the dataset ###
Download the zip folder via the link - https://data.open-power-system-data.org/weather_data/2020-09-16

#### Run these liines in the terminal ####
* Run ```g++ -o main CSVReader.cpp MetozaMain.cpp WeatherDataEntry.cpp PastData.cpp main.cpp -std=c++17 -Wall -Wextra``` from the project directory to compile the code

* Run ```./main``` to run the code
