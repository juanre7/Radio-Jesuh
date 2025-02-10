# 📻 Radio Vintage LEGO - Proyecto con ESP32 y TEA5767

Bienvenido a nuestro proyecto de conversión de una radio vintage de LEGO en una radio completamente funcional usando un **ESP32** y el **módulo TEA5767**. 🚀

## 🎯 Objetivo del Proyecto
Queremos dar vida a una radio clásica de LEGO, manteniendo su estética original pero agregándole tecnología moderna para que pueda **sintonizar emisoras FM reales**. La idea es documentar todo el proceso, desde la conexión del hardware hasta el desarrollo del software, para que cualquier persona pueda replicarlo o mejorarlo.

## 🛠️ Componentes Utilizados
- **ESP32** → Microcontrolador principal
- **TEA5767** → Módulo de radio FM
- **LEGO Radio Vintage** → Carcasa del proyecto
- **Amplificador de audio (opcional)** → Para mejorar la salida de sonido
- **Altavoz pequeño** → Para la reproducción de audio
- **Potenciómetro o resistencias** → Para control de volumen
- **Cables y protoboard** → Para las conexiones iniciales

## 🔌 Conexión del TEA5767 con el ESP32
El TEA5767 se comunica con el ESP32 mediante **I2C**. La conexión es la siguiente:

| TEA5767  | ESP32 |
|----------|------|
| VCC      | 3.3V |
| GND      | GND  |
| SDA      | GPIO 21 |
| SCL      | GPIO 22 |

## 📝 Código y Funcionalidad
### 📡 Configurar la emisora en 92.0 FM
Para sintonizar la radio en 92.0 FM, el ESP32 envía comandos al TEA5767 a través de I2C, estableciendo la frecuencia deseada.

### 🔍 Leer la Frecuencia Actual
El TEA5767 devuelve datos que incluyen la frecuencia actual sintonizada. Utilizamos los primeros dos bytes de la respuesta para calcular la frecuencia con la fórmula:

```
Frecuencia (Hz) = (PLL * 32768) / 4 - 225000
```

### 🔊 Control de Volumen
El TEA5767 no tiene control de volumen incorporado, pero probamos diferentes formas de reducir la salida de audio:
- **Modo MUTE** (desactivando el audio completamente por software)
- **Divisor de voltaje** con resistencias
- **Uso de un potenciómetro** en la salida de audio
- **Control de volumen en el amplificador externo**

## 📂 Estructura del Proyecto
```
📦 RadioVintage-LEGO
 ┣ 📂 src          # Código fuente
 ┣ 📂 docs         # Documentación y esquemas
 ┣ 📂 images       # Fotos del proceso
 ┣ 📜 README.md    # Este documento
 ┗ 📜 LICENSE      # Licencia del proyecto
```

## 📸 Progreso y Actualizaciones
Todo el proceso será documentado con imágenes y avances en **GitHub**. ¡Síguenos y colabora si te interesa el proyecto! 🤖

## 🏗️ Próximos Pasos
✅ Probar la sintonización de más emisoras
✅ Mejorar la calidad de sonido
⬜ Integrar botones para cambiar de emisora
⬜ Diseñar una mejor interfaz de usuario

## 📢 Contribuciones
Si quieres aportar mejoras, ¡eres bienvenido! Puedes abrir un **Pull Request** o reportar errores en la sección de **Issues**.

🚀 ¡Vamos a revivir esta radio vintage con un toque moderno! 🎶

