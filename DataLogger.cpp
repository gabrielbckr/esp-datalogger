#include "DataLogger.hpp"
#include <sstream>


int pin;
File df;
char* label;

void dataLogger::setup(int pin = 2){
    pin = pin;
    df = SPIFFS.open(dataLogger::DF_FILENAME, "w+");
    label = "";
}

void dataLogger::writeRow(int v, unsigned long t){
    char row[1024] = "";
    snprintf(row, 1024, "%u , %d , %s", v, t, label);
    Serial.println("enter writerow"); // BEBUG
    // df.printf("%u , %d , %s\n", v, t, label);
    df.println(row);
    Serial.print("finish writerow: "); // BEBUG
    Serial.println(row); // BEBUG
}


void dataLogger::samplingTask(){

    int value = 1000; // analogRaed(A0);
    Serial.println(value); // BEBUG
    unsigned tstamp = 10;//millis();
    dataLogger::writeRow(value, tstamp);
}

void dataLogger::puseFile(){
    Serial.printf("File has stoped"); // BEBUG
    df.close();
}

void dataLogger::restartFile(){
    Serial.printf("File has restarted"); // BEBUG
    df = SPIFFS.open(dataLogger::DF_FILENAME, "w+");
}