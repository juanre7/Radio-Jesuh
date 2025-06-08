# 📻 Radio Vintage LEGO - Proyecto con ESP32 y TEA5767

Bienvenido a nuestro proyecto de conversión de una radio vintage de LEGO en una radio completamente funcional usando un **ESP32** y el **módulo TEA5767**. 🚀

## 🎯 Objetivo del Proyecto
Queremos hacer funcional una radio de LEGO, manteniendo su estética original pero agregándole tecnología para que pueda **sintonizar emisoras FM reales**. La idea es documentar todo el proceso, desde la conexión del hardware hasta el desarrollo del software, para que cualquier persona pueda replicarlo, mejorarlo o por lo menos inspirarse.

## 📹 DevBlog

### **10 de febrero: Conexión manual TEA5767 y primeras emisoras**  

https://github.com/user-attachments/assets/8bca59d5-34ca-4b3a-bc21-2371d2b4416f

### **15 de febrero: Primeras pruebas sonido bluetooth, directo desde el DAC del ESP32**

https://github.com/user-attachments/assets/ec4be199-8b89-4f50-8597-f48a71e64a97

### **16 de febrero: Primeras pruebas amplificando la señal de radio**  

https://github.com/user-attachments/assets/7099f065-9406-4ad9-b756-701b90826c55

### **22 de feberro: Amplificación «estéreo» del TEA + potenciometro**  

https://github.com/user-attachments/assets/965b3410-e75a-4108-bfb5-50c1bd4986c7

### **10 de marzo: Debug del amplificador-radio (se quemó el amplificador)**  

https://github.com/user-attachments/assets/12592b68-5567-493f-8180-bc3df928a695

### **3 de abril: Vuelta a la pizarra**  

![pared](https://github.com/user-attachments/assets/71244fe2-3411-46b0-968e-14df1c93f227)


### **17 de mayo: Implementación de la mediana en la frecuencia**  

![mediana](https://github.com/user-attachments/assets/39810cc3-d18f-4ddd-a03a-4879e8b31f22)

### **2 de junio: Montaje final y últimas pruebas**  
 
https://github.com/user-attachments/assets/526c9167-5057-4ed5-96ba-6a7904426dba

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


