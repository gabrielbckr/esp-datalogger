#include "App.hpp"
#include "DataLogger.hpp"


const char* ssid = "Becker";
const char* password = "theovoador";


AsyncWebServer server(80);

const int ledPin = 2;
String ledState;
unsigned long previous_t;
bool shouldRecord = true;

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  };
}

void app::setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

    if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);    
            Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP());

    app::setEndpoints();
    dataLogger::setup(2);

    server.begin();

}
void app::setEndpoints(){

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    }); 

    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
        digitalWrite(ledPin, HIGH);    
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
        digitalWrite(ledPin, LOW);    
        request->send(SPIFFS, "/index.html", String(), false, processor);
    }); 

    server.on("/table", HTTP_GET, [](AsyncWebServerRequest *request){
      dataLogger::puseFile();
      shouldRecord = false;
      /* ResponseStrean */
      // AsyncResponseStream* response = request->beginResponseStream("text/csv");
      // File file = SPIFFS.open(dataLogger::DF_FILENAME, "r");
      // response->addHeader("Server","ESP Async Web Server");
      // response->println(file.readString().c_str());
      // request->send(response);
      // file.close();
      /* ChunkedStrean */
      File file = SPIFFS.open(dataLogger::DF_FILENAME, "r");
      AsyncWebServerResponse* response = request->beginChunkedResponse(
        "text/csv", 
        [&](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
        return file.read(buffer, maxLen);
      });
      request->send(response);
      file.close();
      /* DefaultFromFS */

      // request->send(SPIFFS, dataLogger::DF_FILENAME , "text/csv", true);
      dataLogger::restartFile();
      shouldRecord = true;
    }); 
}

void app::loopTask(){
  auto current_t = millis();
  if(shouldRecord && (current_t-previous_t>=2000) )
  {
    dataLogger::samplingTask();
    previous_t = current_t;

  }
}