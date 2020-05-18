#ifndef DATALOGGER_APP
#define DATALOGGER_APP


#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



namespace app
{
    void setup();

    void setEndpoints();

    void loopTask();
} 




#endif