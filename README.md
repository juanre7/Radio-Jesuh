# 📻 Radio Vintage LEGO - Proyecto con ESP32 y TEA5767

Bienvenido a nuestro proyecto de conversión de una radio vintage de LEGO en una radio completamente funcional usando un **ESP32** y el **módulo TEA5767**. 🚀

## 🎯 Objetivo del Proyecto
Queremos hacer funcional una radio de LEGO, manteniendo su estética original pero agregándole tecnología para que pueda **sintonizar emisoras FM reales**. La idea es documentar todo el proceso, desde la conexión del hardware hasta el desarrollo del software, para que cualquier persona pueda replicarlo, mejorarlo o por lo menos inspirarse.

## 📹 DevBlog Videos

### **10 de febrero: Conexión manual TEA5767 y primeras emisoras**  

https://github.com/user-attachments/assets/8bca59d5-34ca-4b3a-bc21-2371d2b4416f

### **15 de febrero: Primeras pruebas sonido sin TEA, directo del DAC del ESP**

https://github.com/user-attachments/assets/ec4be199-8b89-4f50-8597-f48a71e64a97

### **16 de febrero: Primeras pruebas amplificando TEA con PAM**  

https://github.com/user-attachments/assets/7099f065-9406-4ad9-b756-701b90826c55

### **22 de feberro: Amplificación «estéreo» del TEA + potenciometro**  
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


## 📝 Código y Funcionalidad
### 📡 Configurar la emisora en 89.7 FM
Para sintonizar la radio, el ESP32 envía comandos al TEA5767 a través de I2C, estableciendo la frecuencia deseada.
Cada reinicio, se establece la frecuencia sintonizada en Rock FM, un pequeño guiño a nuestros gustos personales.
Para cambiar la frecuencia, basta con girar la perilla del selector.
Además, pulsando el interuptor del lateral, se puede alternar entre el modo radio y el modo bluetooth.


