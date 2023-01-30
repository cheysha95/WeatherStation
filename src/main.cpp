#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

//resets the controller
void(* resetFunc) = 0;

char[] ssid = "ssid";
char[] password = "password";
char[] url = "https://api.open-meteo.com/v1/forecast?latitude=35.47&longitude=-97.52&hourly=temperature_2m";


void setup() {
    // begins by attemping to connect to wifi
    ConnectToWifi(ssid, password);
}

void loop() {
    // if connection secussfull then it will attempt to get the data from the api
    if (WiFi.status() == WL_CONNECTED) {
        getWeatherData(url);
    }

}
// function that connects to wifi, called in setup
void connectToWifi(char* ssid, char* password) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    // wait for connection before moving on
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
}

// function that prints the response to the display, called when http request is successful
void printoDisplay(String response) {
    LiquidCrystal_I2C lcd(0x27, 16, 2);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print(response);
}

// function that gets the data from the api, called in loop
void getWeatherData(char* url) {
    // creates a http client
    HttpClient http;
    // sets the url to the one passed in
    http.begin(url);
    // gets the response from the api
    int httpResponseCode = http.GET();
    // if the response is successful then it will print the response to the display
    if (httpResponseCode > 0) {
        String response = http.getString(); // needs parsing
        printoDisplay(response);
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        resetFunc();
    }
    // ends the http client
    http.end();
}







