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
#define CHANNEL 35
// PANTALLA OLED 
#define ANCHO 128
#define ALTO 64
/* THING.IO */
#define THINGER_SERIAL_DEBUG
#define USERNAME "andresf5597"
#define DEVICE_ID "esp32"
#define DEVICE_CREDENTIAL "QyRdGD0eNm8#@pGP"


arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

const uint16_t samples = 256; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 15000; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/



double vReal[samples];
double vImag[samples];
float fd;
float vel;
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03
#define OLED_RESET -1 
//Adafruit_SSD1306 oled(ANCHO,ALTO, &Wire, OLED_RESET);


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
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  Serial.begin(115200);
  while(!Serial);
  //Serial.println("Ready");
//INICIAR CONEXION WIFI // 

  WiFi.begin(SSID,SSID_PASWORD); 
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Conectando...");
    
   }
   Serial.print("Conectado");
   //Iniciar ThingSpeak
   //ThingSpeak.begin(Client); 

  /* INICIAR OLED */
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
  oled.clearDisplay();


    //THING.IO//
  thing.add_wifi(SSID, SSID_PASWORD);
  //========//

}



void loop()
{
  /*SAMPLING*/
  microseconds = micros();
  for(int i=0; i<samples; i++)
  {
      vReal[i] = analogRead(CHANNEL);
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;



      
  }


  /* Print the results of the sampling according to time */
  //Serial.println("Data:");
  PrintVector(vReal, samples, SCL_TIME);
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  //Serial.println("Weighed data:");
  PrintVector(vReal, samples, SCL_TIME);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  //Serial.println("Computed Real values:");
  PrintVector(vReal, samples, SCL_INDEX);
  //Serial.println("Computed Imaginary values:");
  PrintVector(vImag, samples, SCL_INDEX);
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
  //Serial.println("Computed magnitudes:");
  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
  double x = FFT.MajorPeak(vReal, samples, samplingFrequency); 
  Serial.println("");
  Serial.println(x, 6); //Print out what frequency is the most dominant.
  fd=x;
  vel=fd/44;
  Serial.print(fd/44);Serial.print("km/h");
  
  
  // ENVIAR A LA NUBE 
  /*
    ThingSpeak.setField(1,vel);  
    ThingSpeak.writeFields(channelId,WriteAPIKey);
    Serial.println("Datos enviados a la nube");
    delay(14000);
  */


  // THING.IO // 
      //PARAMETROS PARA PLATAFORMA IOT
  yaw = vel; 
  thing["tempC"] >> [](pson& out){
      out = yaw;
  };
 thing.handle();

  
  //IMPRIMIR VALORES EN OLED//
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




  
  //while(1); /* Run Once */
  delay(2000); /* Repeat after delay */
}

  

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
  break;
    }
    //Serial.println(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
    //Serial.println("Hz");
    Serial.print(" ");
    //Serial.println(vData[i], 4);
  }
}
