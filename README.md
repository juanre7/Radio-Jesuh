# 📻 Radio Vintage LEGO - Proyecto con ESP32 y TEA5767

Bienvenido a nuestro proyecto de conversión de una radio vintage de LEGO en una radio completamente funcional usando un **ESP32** y el **módulo TEA5767**. 🚀

## 🎯 Objetivo del Proyecto
Queremos hacer funcional una radio de LEGO, manteniendo su estética original pero agregándole tecnología para que pueda **sintonizar emisoras FM reales**. La idea es documentar todo el proceso, desde la conexión del hardware hasta el desarrollo del software, para que cualquier persona pueda replicarlo, mejorarlo o por lo menos inspirarse.

## 📹 DevBlog Videos
Mantente al tanto de nuestro proceso de desarrollo a través de nuestros videos en el DevBlog. ¡Pronto estarán disponibles!

1. **10 de febrero: Conexión manual TEA5767 y primeras emisoras**  
   [Placeholder para Video 1](#)

2. **15 de febrero: Primeras pruebas sonido sin TEA, directo del DAC del ESP**  
   [Placeholder para Video 2](#)

3. **16 de febrero: Primeras pruebas amplificando TEA con PAM**  
   [Placeholder para Video 3](#)

4. **22 de feberro: Amplificación «estéreo» del TEA + potenciometro**  
   [Placeholder para Video 4](#)

## 🛠️ Componentes Utilizados
- **ESP32** → Microcontrolador principal
- **TEA5767** → Módulo de radio FM
- **PAM...** → Amplificador de audio
- **LEGO Radio Vintage** → Carcasa del proyecto
- **2 altavoces pequeños** → Para la reproducción de audio
- **Potenciómetro deslizante/lineal** → Para control de volumen
- **Cables y protoboard** → Para las conexiones iniciales
- **USB-C PD board** → Placa que permite el uso inteligente de cualquier fuente usb
- **Cargador USB-C PD** → Fuente de alimentación USB genérica

## 🔌 Conexión del TEA5767 con el ESP32
El TEA5767 se comunica con el ESP32 mediante **I2C**. La conexión es la siguiente:

| TEA5767  | ESP32  |
|----------|--------|
| VCC      | 3.3V   |
| GND      | GND    |
| SDA      | GPIO 21|
| SCL      | GPIO 22|

## 📝 Código y Funcionalidad
### 📡 Configurar la emisora en 92.0 FM
Para sintonizar la radio, el ESP32 envía comandos al TEA5767 a través de I2C, estableciendo la frecuencia deseada.

### 🔍 Leer la Frecuencia Actual
El TEA5767 devuelve datos que incluyen la frecuencia actual sintonizada. Utilizamos los primeros dos bytes de la respuesta para calcular la frecuencia con la fórmula:

\[
\text{Frecuencia (Hz)} = \frac{\text{PLL} \times 32768}{4} - 225000
\]

## 📂 Estructura del Proyecto
```
📦 RadioVintage-LEGO
 ┣ 📂 Radio_autoFreq     # Pruebas sintonización TEA
 ┣ 📂 docs               # Documentación y esquemas
 ┣ 📂 DevBlog       # Fotos y videos del proceso
 ┣ 📜 README.md    # Este documento
 ┗ 📜 LICENSE      # Licencia del proyecto
```

## 🏗️ Próximos Pasos
✅ Probar la sintonización con potenciometro
✅ Amplificación con PAM
⬜ Conexión Bluetooth A2DP
⬜ Adaptación de impedancias

### 🔊 Adaptación de impedancias
El TEA5767 no tiene control de volumen incorporado, y el ESP32 tiene menos potencia de salida. Si no adaptamos de alguna manera, al cambiar de fuente de sonido, habrá una gran diferencia en el volumen percibido (después de pasar por el PAM).
Estas son las opciones que se barajan:
- **Divisor de tensión** con resistencias
- **Uso de un potenciómetro interno** en la salida del TEA

🚀 ¡Vamos a hacer funcional este modelo de radio vintage, con un toque moderno! 🎶

