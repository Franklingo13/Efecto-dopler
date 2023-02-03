/*

  Example of use of the FFT libray to compute FFT for a signal sampled through the ADC.
        Copyright (C) 2018 Enrique Condés and Ragnar Ranøyen Homb

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "arduinoFFT.h"
//#include "ThingSpeak.h"
#include "WiFi.h"
#include <ThingerESP32.h>


/*
These values can be changed in order to evaluate the functions
*/

/* FOR FFT */
#define PIN_NUMBER 23
#define AVERAGE 4
// PANTALLA OLED 
#define ANCHO 128
#define ALTO 64
/* THING.IO */
#define THINGER_SERIAL_DEBUG
#define USERNAME "andresf5597"
#define DEVICE_ID "esp32"
#define DEVICE_CREDENTIAL "QyRdGD0eNm8#@pGP"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

float fd;
float vel;
unsigned int doppler_div = 44;
unsigned int samples[AVERAGE];
unsigned int x;

#define OLED_RESET -1 
Adafruit_SSD1306 oled(ANCHO,ALTO, &Wire, OLED_RESET);


// INICIALIZAR WIFI
const char* SSID = "POCOX3";
const char* SSID_PASWORD = "pocox5597"; 

unsigned long channelId = 1915797;
const char* WriteAPIKey = "Z2WF768I7D3PWMIK";
/* thing.io */ 

float yaw = 0; // defined as a global variable for thing.io

WiFiClient Client; 

void setup()
{
  
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
//INICIAR CONEXION WIFI // 

 /* WiFi.begin(SSID,SSID_PASWORD); 
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Conectando...");
    
   }
 
   Serial.print("Conectado");}
     */
  //THING.IO//
  thing.add_wifi(SSID, SSID_PASWORD);
  //========//

  /* INICIAR OLED */
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
  oled.clearDisplay();


 thing.handle();

}



void loop() {

  //noInterrupts();
  //con "pulseIn" se obtiene el periodo en us
 // pulseIn(PIN_NUMBER, HIGH);
  unsigned int pulse_length = 0;

  // el bucle registra y almacena 4 mediciones del periodo
//  for (x = 0; x < AVERAGE; x++)
//  {
//    pulse_length = pulseIn(PIN_NUMBER, HIGH); 
//    pulse_length += pulseIn(PIN_NUMBER, LOW);    
//    samples[x] =  pulse_length;
//  }
  //interrupts();

  // comprobacion de consistencia
  // comprueba que el promedio se haga sobre muestras con el mismo perido aproximado
  bool samples_ok = true;
  unsigned int nbPulsesTime = samples[0];
  for (x = 1; x < AVERAGE; x++)
  {
    nbPulsesTime += samples[x];
    if ((samples[x] > samples[0] * 2) || (samples[x] < samples[0] / 2))
    {
      samples_ok = false;
    }
  }


if (samples_ok)
  {
    Serial.print("samples ok");
    unsigned int Ttime = nbPulsesTime / AVERAGE; //periodo promedio
    unsigned int Freq = 1000000 / Ttime; 

      Serial.write(Freq/doppler_div); // obtencion de velocidad
      //Serial.print(Ttime);
      Serial.print("\r\n");
      Serial.print(Freq);
      Serial.print("Hz : ");
      Serial.print(Freq/doppler_div);
      Serial.print("km/h\r\n");
    
      vel =  Freq/doppler_div;      
//      ThingSpeak.setField(1,vel);
      

      oled.clearDisplay();
      oled.setTextColor(WHITE); //NO MULTIPLES COLORES
      oled.setCursor(0,0); //ELIGE COORDENADAS PARA COMENEZAR A ESCRIBIR
      oled.setTextSize(1); //TAMAÑO DEL TEXTO
      oled.print("Velocidad:");
      oled.setCursor(10,30); // NUEVAS COORDENADAS
      oled.setTextSize(2);   //TAMAÑO DE TEXTO
      oled.print(vel);
      oled.print(" Km/h");
      oled.display();


  // ENVIAR A LA NUBE 
  // THING.IO // 
      //PARAMETROS PARA PLATAFORMA IOT
  yaw = vel; 
  thing["tempC"] >> [](pson& out){
      out = yaw;
  };


      
  }

   //while(1); /* Run Once */
  delay(2000); /* Repeat after delay */
}
