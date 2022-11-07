/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-temperature-humidity-sensor
 */


// Channel ID: 1915797 
// API KEY: Z2WF768I7D3PWMIK

#include "ThingSpeak.h"
#include "WiFi.h"

#include <DHT.h>
#define DHT_SENSOR_PIN  21 // ESP32 pin GIOP21 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11



// Wifi 

const char* ssid = "POCOX3";
const char* password = "pocox5597"; 

unsigned long channelId = 1915797;
const char* WriteAPIKey = "Z2WF768I7D3PWMIK";

WiFiClient Client; 

//

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
  Serial.begin(9600);
  dht_sensor.begin(); // initialize the DHT sensor

   // INICAIR CONEXION WIFI // 

  WiFi.begin(ssid,password); 
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Conectando...");
    
   }
   Serial.print("Conectado");
   //Iniciar ThingSpeak
   ThingSpeak.begin(Client);

}



void loop() {
  // read humidity
  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht_sensor.readTemperature(true);



  // check whether the reading is successful or not
  if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
    Serial.print(tempF);
    Serial.println("°F");


   // THINGSPEAK
   ThingSpeak.setField(1,tempC);



  //
    
  }

  // wait a 2 seconds between readings
  delay(2000);

  ThingSpeak.writeFields(channelId,WriteAPIKey);
  Serial.println("Datos enviados a la nube");
  delay(14000);
  
}
