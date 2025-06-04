#include <Wire.h>
#include <Arduino.h>
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

#define TEA5767_ADDRESS 0x60  // Dirección I2C del TEA5767
#define PIN_ANALOGICO 36      // Pin del potenciómetro
#define N 20                  // Número de muestras para la mediana

// ——————————————————————————————————————————————————————————
// 1) Variables globales relacionadas con el timer
// ——————————————————————————————————————————————————————————
hw_timer_t *timer = nullptr;
volatile bool bloqHist = false;  // Flag que marca que el timer ya disparó
bool timerActive = false;        // Indica si el timer está corriendo

// ——————————————————————————————————————————————————————————
// 2) Variables para manejar el potenciómetro y la frecuencia FM
// ——————————————————————————————————————————————————————————
int valor = 0;
int muestras[N];  // Buffer de muestras
float frec = 0;
float frecIni = 0;
float frecSintz = 0;
float frecAnt = 0;
bool ROCK = true;
float frecuencia = 89.7;

// ——————————————————————————————————————————————————————————
// 3) Objetos de audio / Bluetooth
// ——————————————————————————————————————————————————————————
AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

// ——————————————————————————————————————————————————————————
// 4) Función para leer la mediana de N muestras del ADC
// ——————————————————————————————————————————————————————————
int readMedian() {
  for (int i = 0; i < N; i++) {
    muestras[i] = analogRead(PIN_ANALOGICO);
    delay(5);
  }
  // Ordenamos de menor a mayor (algoritmo de burbuja sencillo)
  for (int i = 0; i < N - 1; i++) {
    for (int j = i + 1; j < N; j++) {
      if (muestras[j] < muestras[i]) {
        int tmp = muestras[i];
        muestras[i] = muestras[j];
        muestras[j] = tmp;
      }
    }
  }
  return muestras[N / 2];
}

// ——————————————————————————————————————————————————————————
// 5) Prototipos de funciones para sintonizar y leer estado
// ——————————————————————————————————————————————————————————
void sintonizarA(float frecuencia_MHz);
byte leerEstadoConexion();

// ——————————————————————————————————————————————————————————
// 6) ISR del timer
//    Se ejecuta cuando el timer alcanza 1 s sin reiniciarse
//    Marca bloqHist=true y detiene el timer (one-shot).
// ——————————————————————————————————————————————————————————
void IRAM_ATTR onTimer() {
  bloqHist = true;
  timerActive = false;
  timerStop(timer);  // Aseguramos que no vuelva a disparar hasta que lo reiniciemos
}

// ——————————————————————————————————————————————————————————
// 7) setup()
// ——————————————————————————————————————————————————————————
void setup() {
  // Inicia Bluetooth
  a2dp_sink.start("RockYourLEGO by GV27 Electronics");
  
  // Inicia Serial para debug
  Serial.begin(115200);
  while (!Serial) { ; }  // Espera a que abra el monitor (opcional en ESP32)
  Serial.println();
  Serial.println("Inicializando TEA5767...");

  // Configura el bus I2C (SDA=21, SCL=22)
  Wire.begin(21, 22);

  // Estado inicial de variables
  valor = readMedian();
  frecuencia = 87.5 + ((float)valor) * (108.0 - 87.5) / 4095.0;
  frecuencia = round(frecuencia * 10.0) / 10.0;
  frecIni = frecuencia;
  frecSintz = frecuencia;

  // Sintoniza la emisora inicial
  sintonizarA(frecuencia);

  // ————————————————————————————————————————————————————————
  // 7.1) Inicializar el timer DE FORMA GLOBAL (solo una vez)
  //       → Lo configuramos a 1 MHz (1 tick = 1 μs)
  //       → Lo asociamos a nuestra ISR onTimer()
  //       → NO le programamos alarma todavía (lo haremos dinámicamente)
  // ————————————————————————————————————————————————————————
  timer = timerBegin(1000000);              // frequency = 1 MHz
  timerAttachInterrupt(timer, &onTimer);     // solo dos parámetros en v3.x
  // NOTA: no llamamos aquí a timerAlarm(...). Dejamos el timer “a la espera” hasta que lo iniciemos en loop().
}

// ——————————————————————————————————————————————————————————
// 8) loop()
// ——————————————————————————————————————————————————————————
void loop() {
  // 8.1) Leer el potenciómetro (mediana) y calcular la frecuencia FM
  valor = readMedian();
  frec = 87.5 + ((float)valor) * (108.0 - 87.5) / 4095.0;
  frec = round(frec * 10.0) / 10.0;

  Serial.print("Frecuencia potenciómetro [87.5,108]: ");
  Serial.println(frec);

  // 8.2) Determinar si seguimos en “modo ROCK” (proximidad a frecIni)
  ROCK = ROCK && (abs(frec - frecIni) < 2);

  // 8.3) bloqHist solo se mantiene false si la diferencia es menor a 0.3
  //      (frecAnt viene de la iteración anterior)
  bloqHist = bloqHist && (abs(frec - frecAnt) < 0.3);

  // 8.4) Sintonizar solo si cambió la frecuencia Y NO estamos en modo ROCK Y No tenemos bloqueado historial
  if ((frec != frecSintz) && (!ROCK) && (!bloqHist)) {
    sintonizarA(frec);
    frecSintz = frec;  // Actualizamos la frecuencia ya sintonizada

    // 8.5) Si la frecuencia no cambió desde la última lectura (frec == frecAnt),
    //       y NO tenemos el timer activo, lo iniciamos para que bloquee historial en 1 s.
    if ((frec == frecAnt) && (!timerActive)) {
      // Reiniciamos el timer desde 0 y lo programamos a 1 000 000 ticks → 1 s, one-shot
      timerRestart(timer);                  // pone contador a 0 y arranca
      timerAlarm(timer, 1000000ULL, false, 0); 
      timerActive = true;
    }
    // 8.6) Si la frecuencia cambió (frec != frecAnt) y el timer estaba activo, lo detenemos
    else if ((frec != frecAnt) && timerActive) {
      timerStop(timer);
      timerActive = false;
      bloqHist = false;  // si cambiamos antes de que expire el timer, borramos bloqueo
    }
  }

  // 8.7) Mostrar frecuencia ya sintonizada
  Serial.print("Frecuencia sintonizada [87.5,108]: ");
  Serial.println(frecSintz);

  // 8.8) Guardamos la frecuencia anterior para la próxima iteración
  frecAnt = frec;

  delay(500);  // Pausa entre iteraciones
}

// ——————————————————————————————————————————————————————————
// 9) Función para sintonizar el TEA5767 a una frecuencia dada
// ——————————————————————————————————————————————————————————
void sintonizarA(float frecuencia_MHz) {
  uint32_t frecuencia_Hz = (uint32_t)(frecuencia_MHz * 1000000UL);
  uint16_t pll = (uint16_t)((4UL * (frecuencia_Hz + 225000UL)) / 32768UL);

  byte config[5];
  config[0] = (pll >> 8) & 0xFF;
  config[1] = pll & 0xFF;
  config[2] = 0xB0;  // Modo normal, sin mute, stereo
  config[3] = 0x10;  // Config adicional (por ejemplo, stereo blend)
  config[4] = 0x00;  // Reservado

  Wire.beginTransmission(TEA5767_ADDRESS);
  Wire.write(config, 5);
  byte error = Wire.endTransmission();

  if (error == 0) {
    Serial.println("Comunicación I2C con TEA5767 exitosa.");
  } else {
    Serial.print("Error I2C al sintonizar. Código: ");
    Serial.println(error);
  }

  Serial.println("Emisora configurada correctamente.");
}

// ——————————————————————————————————————————————————————————
// 10) (Opcional) Leer estado de conexión del TEA5767
// ——————————————————————————————————————————————————————————
byte leerEstadoConexion() {
  Wire.requestFrom(TEA5767_ADDRESS, 5);
  byte estado[5];
  if (Wire.available() == 5) {
    for (int i = 0; i < 5; i++) {
      estado[i] = Wire.read();
    }
    Serial.print("Datos leídos TEA5767: ");
    for (int i = 0; i < 5; i++) {
      Serial.print("0x");
      if (estado[i] < 16) Serial.print("0");
      Serial.print(estado[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("No se pudieron leer datos TEA5767.");
  }
  return estado[0];
}
