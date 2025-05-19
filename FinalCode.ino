#include <Wire.h>
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

#define TEA5767_ADDRESS 0x60  // Dirección I2C del TEA5767
#define PIN_ANALOGICO 36      //Pin del potenciometro
#define N 20                  // Número de muestras para la mediana

int valor = 0;
int muestras[N];  // Buffer de muestras
float frec = 0;
float frecIni = 0;
bool ROCK = true;

float RockFM = 89.7;
float frecuencia = 89.7;
float frecSintz = 0;

AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

//#Lee N veces el pin analógico y devuelve la mediana # # # # # # # #
int readMedian() {
  // 1. Recoger N muestras
  for (int i = 0; i < N; i++) {
    muestras[i] = analogRead(PIN_ANALOGICO);
    delay(5);  // opcional: pequeña pausa entre lecturas
  }
  // 2. Ordenar el array de muestras (ordenación muy simple)
  for (int i = 0; i < N - 1; i++) {
    for (int j = i + 1; j < N; j++) {
      if (muestras[j] < muestras[i]) {
        int tmp = muestras[i];
        muestras[i] = muestras[j];
        muestras[j] = tmp;
      }
    }
  }
  // 3. Devolver la mediana (elemento central)
  return muestras[N / 2];
}
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
void sintonizarA(float frecuencia_MHz);
byte leerEstadoConexion();

void setup() {
  //Inicia Bluetooth
  a2dp_sink.start("RockYourLEGO by GV27 Electronics");
  // Inicia la comunicación serial para depuración
  Serial.begin(115200);
  while (!Serial) {
    ;  // Espera a que se abra el monitor serial (opcional en ESP32)
  }
  Serial.println();
  Serial.println("Inicializando TEA5767...");

  // Configura el bus I2C usando los pines 21 (SDA) y 22 (SCL)
  Wire.begin(21, 22);

  sintonizarA(frecuencia);
  frecSintz = frecuencia;

  valor = readMedian();
  frecuencia = 87.5 + ((float)valor) * (108.0 - 87.5) / 4095.0;
  frecuencia = round(frecuencia * 10.0) / 10.0;
  frecIni = frecuencia;
}

void loop() {
  //leer potenciometro y pasarlo a frec fm
  // 1. Leer el valor mediano del potenciómetro
  valor = readMedian();

  // 2. Mapear lectura [0..4095] a frecuencia [87.5..108]
  frecuencia = 87.5 + ((float)valor) * (108.0 - 87.5) / 4095.0;

  // 3. (Opcional) Redondear a un decimal
  frecuencia = round(frecuencia * 10.0) / 10.0;

  // 4. Mostrar por Serie
  Serial.print("Frecuencia potenciometro [87.5,108]: ");
  Serial.println(frecuencia);

  ROCK = ROCK && (abs(frecuencia - frecIni) < 2); // Si ROCK TRUE = RockFM. ROCK FALSE = 

  // 5. Sintonizar solo si la frecuencia ha cambiado desde la última vez
  //    Esto evita enviar comandos innecesarios al TEA5767
  if ((frecuencia != frecSintz) && (!ROCK)) {
    sintonizarA(frecuencia);
    frecSintz = frecuencia;  // Actualizar la frecuencia ya sintonizada
  }
  // 6. Verificar el estado de conexión con el chip y resintonizar si es necesario
  // byte estado = leerEstadoConexion();
  // int bitState = estado & 128;  // Extraer el bit más significativo (bit 7)

  // if (bitState == 0) {
  //   // Si la conexión se ha perdido o hay un problema, resintonizar
  //   Serial.println("########## Conexión interrumpida, resintonizando... ##########");
  //   sintonizarA(frecuencia);
  // }

  Serial.print("Frecuencia Sintonizada [87.5,108]: ");
  Serial.println(frecSintz);

  delay(500);
}

/**
 * Función para sintonizar el TEA5767 a una frecuencia específica
 * @param frecuencia_MHz Frecuencia en MHz a sintonizar (ej: 89.7)
 */
void sintonizarA(float frecuencia_MHz) {
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

  // Verificar si la comunicación fue exitosa y mostrar mensaje
  if (error == 0) {
    Serial.println("Comunicación I2C con TEA5767 exitosa.");
  } else {
    Serial.print("Error en la comunicación I2C. Código de error: ");
    Serial.println(error);
  }

  Serial.println("Emisora configurada correctamente.");
}

byte leerEstadoConexion() {
  // Opcional: Leer los 5 bytes de estado que el TEA5767 envía de vuelta.
  // Estos bytes contienen información sobre la frecuencia, nivel de señal, etc.
  Wire.requestFrom(TEA5767_ADDRESS, 5);
  byte estado[5];
  if (Wire.available() == 5) {  // Si hay datos disponibles, leerlos y mostrarlos en formato hexadecimal
    for (int i = 0; i < 5; i++) {
      estado[i] = Wire.read();
    }
    Serial.print("Datos leídos del TEA5767: ");
    for (int i = 0; i < 5; i++) {
      Serial.print("0x");
      if (estado[i] < 16) Serial.print("0");  // Asegura que los números hexadecimales tengan dos dígitos
      Serial.print(estado[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("No se pudieron leer los datos del TEA5767.");
  }
  // Devolver el primer byte que contiene el bit de estado principal
  return estado[0];
}
