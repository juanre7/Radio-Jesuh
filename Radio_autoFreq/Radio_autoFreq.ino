#include <Wire.h>

#define TEA5767_ADDRESS 0x60  // Dirección I2C del TEA5767

#define PIN_ANALOGICO 15

int valor = 0;
float frec = 0;

float frecuencia = 89.7;
float frecSintz = 0;

void sintonizarA(float frecuencia_MHz);
byte leerEstadoConexion();

void setup() {
  // Inicia la comunicación serial para depuración
  Serial.begin(115200);
  while (!Serial) {
    ; // Espera a que se abra el monitor serial (opcional en ESP32)
  }
  Serial.println();
  Serial.println("Inicializando TEA5767...");

  // Configura el bus I2C usando los pines 21 (SDA) y 22 (SCL)
  Wire.begin(21, 22);

  sintonizarA(frecuencia);
  frecSintz = frecuencia;
}

void loop() {
  //leer potenciometro y pasarlo a frec fm
  valor = analogRead(PIN_ANALOGICO);
  frecuencia =  87.5 + valor * (108 - 87.5)/4095;
  frecuencia = round(frecuencia * 10) / 10;
  Serial.println(frecuencia);
  
  //sintonizar si es diferente a la frecuencia ya sintonizada
  if(frecuencia != frecSintz){
    sintonizarA(frecuencia);
    frecSintz = frecuencia;
  }
  //leer estado y resincronizar si se ha interrumpido la conexion 
  byte estado = leerEstadoConexion();
  int bitState = estado & 128;
  if (bitState == 0){
    Serial.println("aqui"); 
    sintonizarA(frecuencia); 
    }
  
  delay(100);
}


void sintonizarA(float frecuencia_MHz){
  uint32_t frecuencia_Hz = (uint32_t)(frecuencia_MHz * 1000000UL);
  uint16_t pll = (uint16_t)((4UL * (frecuencia_Hz + 225000UL)) / 32768UL);

  // Prepara los 5 bytes de configuración según el protocolo del TEA5767
  // Los dos primeros bytes corresponden al valor PLL (14 bits)
  // Los siguientes bytes configuran opciones: mute, búsqueda, stereo, etc.
  byte config[5];
  config[0] = (pll >> 8) & 0xFF;  // Byte alto del PLL
  config[1] = pll & 0xFF;         // Byte bajo del PLL
  config[2] = 0xB0;               // Configuración: 
                                  //   - Bit 7: Search mode (1 = modo normal)
                                  //   - Bit 6: Mute (0 = sin mute)
                                  //   - Bits 5-0: Opciones varias (por defecto)
  config[3] = 0x10;               // Configuración adicional (por ejemplo, stereo blend)
  config[4] = 0x00;               // Byte reservado (usualmente 0)

  // Envía la configuración al TEA5767 vía I2C
  Wire.beginTransmission(TEA5767_ADDRESS);
  Wire.write(config, 5);
  byte error = Wire.endTransmission();

  if (error == 0) {
    Serial.println("Comunicación I2C con TEA5767 exitosa.");
  } else {
    Serial.print("Error en la comunicación I2C. Código de error: ");
    Serial.println(error);
  }

  Serial.println("Emisora configurada correctamente.");
}

byte leerEstadoConexion(){
    // Opcional: Leer los 5 bytes de estado que el TEA5767 envía de vuelta.
  // Estos bytes contienen información sobre la frecuencia, nivel de señal, etc.
  Wire.requestFrom(TEA5767_ADDRESS, 5);
  byte estado[5];
  if (Wire.available() == 5) {
    for (int i = 0; i < 5; i++) {
      estado[i] = Wire.read();
    }
    Serial.print("Datos leídos del TEA5767: ");
    for (int i = 0; i < 5; i++) {
      Serial.print("0x");
      if (estado[i] < 16) Serial.print("0");
      Serial.print(estado[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("No se pudieron leer los datos del TEA5767.");
  }  
  return estado[0];  
}
