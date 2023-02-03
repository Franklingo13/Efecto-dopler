

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "arduinoFFT.h"
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

const uint16_t samples = 2048 ; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 4000; //Hz, must be less than 10000 due to ADC

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
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
//INICIAR CONEXION WIFI //



  WiFi.begin(SSID,SSID_PASWORD);


  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Conectando...");

   }
   Serial.print("Conectado");


  /* INICIAR OLED */
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
  oled.clearDisplay();


    //THING.IO//
 thing.add_wifi(SSID, SSID_PASWORD);
  Serial.print(" ");
  Serial.println("Ready_thing");
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



  //Serial.println("Data:");
//  PrintVector(vReal, samples, SCL_TIME);
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  //Serial.println("Weighed data:");
 // PrintVector(vReal, samples, SCL_TIME);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
  //Serial.println("Computed Real values:");
 // PrintVector(vReal, samples, SCL_INDEX);
  //Serial.println("Computed Imaginary values:");
 // PrintVector(vImag, samples, SCL_INDEX);
  FFT.ComplexToMagnitude(vReal, vImag, samples);
  //Serial.println("Computed magnitudes:");
//  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);


  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  Serial.println("");
  Serial.println(x, 6); //Print out what frequency is the most dominant.
  fd=x;
  vel=fd/44;
  Serial.print(fd/44);Serial.print("km/h");




  // THING.IO //
      //PARAMETROS PARA PLATAFORMA IOT
  yaw = vel;
  thing["tempC"] >> [](pson& out){
      out = yaw;
  };
// thing.handle();


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
  delay(1000); /* Repeat after delay */
}
