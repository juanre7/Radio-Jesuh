#include <Wire.h>

#define TEA5767_ADDRESS 0x60  // Dirección I2C del TEA5767

float frecuencia = 89.7; //Rock FM (unica radio necesaria)

void sintonizarA(float frecuencia_MHz);

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
}

void loop() {
  // Opcional: Leer los 5 bytes de estado que el TEA5767 envía de vuelta.
  // Estos bytes contienen información sobre la frecuencia, nivel de señal, etc.
  Wire.requestFrom(TEA5767_ADDRESS, 5);
  byte status[5];
  if (Wire.available() == 5) {
    for (int i = 0; i < 5; i++) {
      status[i] = Wire.read();
    }
    Serial.print("Datos leídos del TEA5767: ");
    for (int i = 0; i < 5; i++) {
      Serial.print("0x");
      if (status[i] < 16) Serial.print("0");
      Serial.print(status[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("No se pudieron leer los datos del TEA5767.");
  }  
  int bitState = status[0] & 128;
  Serial.println(bitState);
  if (bitState == 0){
    Serial.println("aqui"); 
    sintonizarA(frecuencia); 
    }

  
  // Espera 5 segundos antes de volver a leer
  delay(1000);
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
