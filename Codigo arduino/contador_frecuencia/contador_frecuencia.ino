// Below: pin para la lectura de frecuencia
#define PIN_NUMBER 4
// numero de muestras para promediar
#define AVERAGE 4

unsigned int doppler_div = 44;
unsigned int samples[AVERAGE];
unsigned int x;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_NUMBER, INPUT);
}

void loop() {
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
  }

}
