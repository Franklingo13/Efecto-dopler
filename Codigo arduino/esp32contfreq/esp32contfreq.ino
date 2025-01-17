
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Below: pin para la lectura de frecuencia
#define PIN_NUMBER A1
// numero de muestras para promediar
#define AVERAGE 4


#define ANCHO 128
#define ALTO 64


#define OLED_RESET -1 
Adafruit_SSD1306 oled(ANCHO,ALTO, &Wire, OLED_RESET);


#include "ThingSpeak.h"
#include "WiFi.h"

unsigned int doppler_div = 44;
unsigned int samples[AVERAGE];
unsigned int x;
float vel;


const char* ssid = "POCOX3";
const char* password = "pocox5597"; 

unsigned long channelId = 1915797;
const char* WriteAPIKey = "Z2WF768I7D3PWMIK";

WiFiClient Client; 




int b = 0;  // bandera para el display 
int retraso = 5000;
//const unsigned char prueba [] PROGMEM = {
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x7c, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xc0, 0x18, 0x00, 0x08, 0x8c, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xf0, 0x78, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xfd, 0xf8, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0x1f, 0xf8, 0x00, 0x3c, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xc7, 0xe6, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfd, 0xef, 0xc3, 0xef, 0x01, 0xe0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0x01, 0xcf, 0x80, 0x01, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfd, 0xfe, 0x00, 0xe7, 0xf8, 0x00, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfd, 0xfc, 0x00, 0x6f, 0xf8, 0x38, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0x00, 0x3f, 0xf8, 0x78, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0xf0, 0x04, 0x3f, 0xfc, 0x29, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x78, 0x02, 0x3f, 0xdf, 0x79, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x78, 0x0f, 0x3f, 0xfe, 0x0f, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x28, 0x1b, 0x7f, 0xbc, 0x3f, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x34, 0x3f, 0x79, 0x9c, 0x3f, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x3c, 0x3f, 0xdf, 0xf8, 0x3f, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x7c, 0x7f, 0x8f, 0xf8, 0x39, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x78, 0x7f, 0xcf, 0xf0, 0x3f, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x30, 0x3f, 0xcf, 0xb0, 0x3e, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x1f, 0xc4, 0x60, 0x7e, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x03, 0x83, 0xe0, 0x7e, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x79, 0x03, 0xe0, 0x3e, 0x60, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7c, 0x07, 0xf0, 0xc0, 0x70, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x5c, 0x03, 0xf5, 0xc0, 0x78, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x18, 0x00, 0xf3, 0xe6, 0x70, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x63, 0xe7, 0xd8, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x23, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xc0, 0x17, 0xff, 0x7b, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x02, 0x00, 0x0b, 0xf8, 0xfb, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xfd, 0xfb, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1e, 0xfb, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1e, 0x7e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0e, 0x7f, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x40, 0x0c, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0c, 0x3f, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0e, 0x1f, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xc0, 0x0f, 0x8f, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x06, 0x0f, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0c, 0x0f, 0xff, 0xd0, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0c, 0x07, 0xff, 0xb8, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x10, 0xff, 0x18, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x08, 0x3f, 0x3e, 0x1c, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x08, 0x7f, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0a, 0x7f, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3e, 0xff, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7d, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xfc, 0x70, 0x00, 0x7f, 0xd8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xbf, 0xcc, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x87, 0x98, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x13, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x03, 0xd0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x03, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xc2, 0x00, 0x03, 0xe0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xc4, 0x00, 0x03, 0xe1, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xc8, 0x00, 0x03, 0x81, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xd9, 0x80, 0x03, 0x85, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x81, 0x02, 0x07, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0x81, 0x03, 0x0b, 0x81, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x83, 0xff, 0x81, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff
//};




void setup() {
  
  Serial.begin(115200);
  pinMode(PIN_NUMBER, INPUT);

//// INICAIR CONEXION WIFI // 
//
//  WiFi.begin(ssid,password); 
//  while (WiFi.status() != WL_CONNECTED){
//    delay(500);
//    Serial.print("Conectando...");
//    
//   }
//   Serial.print("Conectado");
//   //Iniciar ThingSpeak
//   ThingSpeak.begin(Client);  
   


/* INICIAR OLED */
  Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
  oled.clearDisplay();

}




void loop() {


  // put your main code here, to run repeatedly:
//  if (b == 0){
//      oled.drawBitmap(0,0,prueba,128,64,BLACK,WHITE);
//      oled.display();
//      
//      delay(retraso); 
//      b = 1; 
//    }




  
  // deshabilita interrupciones, 
  //codigo sensible al tiempo:
  noInterrupts();
  //con "pulseIn" se obtiene el periodo en us
  pulseIn(PIN_NUMBER, HIGH);
  unsigned int pulse_length = 0;

  // el bucle registra y almacena 4 mediciones del periodo
  for (x = 0; x < AVERAGE; x++)
  {
    pulse_length = pulseIn(PIN_NUMBER, HIGH); 
    pulse_length += pulseIn(PIN_NUMBER, LOW);    
    samples[x] =  pulse_length;
  }
  interrupts();

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
      if (vel==4){
        
      }
      oled.print(vel);
      oled.print(" Km/h");
      oled.display();



      
  }

  // wait a 2 seconds between readings
  delay(50);
  /*
  ThingSpeak.writeFields(channelId,WriteAPIKey);
  Serial.println("Datos enviados a la nube");
  delay(14000);
*/

}
