# Radar de Velocidad Basado en el Efecto Doppler

Para este proyecto se emplea el efecto Doppler para calcular la velocidad de un objeto en base a la diferencia de frecuencias entre la enviada y la recibida.
El dispositivo empleado como transmisor y receptor es el sensor de movimiento CDM324, el cual funciona a una frecuecia de 24GHz. Este sensor también incorpora un mezclador de altas frecuencias y su salida es de unos pocos milivoltios, razón por la cual debe ser amplificada.

Se basó en el siguiente trabajo: https://github.com/limpkin/cdm324_backpack.




## _Primera Versión_
- Usa arduino Mega.
- Calcula la frecuencia recibida mediante usando la función _pulsein_.
- Presenta la velocidad en una pequeña pantalla OLED


### Amplificador 

Para el amplificador se usó el operacional LM358 alimentado a 5V y consta de dos etapas, la primera pre-amplifica la señal y en la segunda se obtiene una señal cuadrada. 
La simulación en Proteus de este circuito se encuentra en _Sim_Proteus/Circuito_amplificador.pdsprj_

<img alt="Esquema de la primera versión del amplificador" src="/Imágenes/esquema_amplificador_arduinoMega.png"/>


### Código arduino

Para calcular la frecuencia de un objeto se mide el ancho de cada pulso de la señal recibida en el Arduino para calcular el periodo de la señal y así tener su frecuencia Doppler, con la que ya se puede calcular la velocidad. El código para implementar este método está en _Codigo arduino/contador_frecuencia_ .


### Placa PCB
En la carpeta de _Esquemáticos_ se puede encontrar el gerber de la siguiente placa PCB.

<img alt="Priemra versión de placa PCB" src="/Imágenes/PCB_proteus.png"/>

### Soporte 3D

Las piezas para el soporte impreso en 3D mostrado a continuación se encuentran en la carpeta _Piezas_.
<img alt="Primera versión del soporte" src="/Imágenes/diseño_3D.png"/>


## _Segunda versión_
- Usa un ESP32.
- Calcula la frecuencia recibida mediante FFT.
- Presenta la velocidad en una pequeña pantalla OLED
- Envia los datos a un servicio en la nube para poder revisar la velocidad desde cualquier dispositivo con acceso a internet.


### Amplificador 

<img alt="Esquema de la segunda versión del amplificador" src="/Imágenes/esquema_amplificador_ESP32.png"/>

### Código arduino

Para encontrar la velocidad se calcula la FFT de la señal recibida empleando la librería arduinoFFT.h, además de presentar los datos en una pantalla OLED, también los envía al servicio en la nube de Thinger.io.  El código para implementar este método está en _Codigo arduino/final_esp32_FFT_ .


### Placa PCB


### Soporte 3D

Las piezas de la segunda versión del soporte se encuentran en la carpeta _Piezas radar final_ESP32_.
