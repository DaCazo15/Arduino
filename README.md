<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://i.postimg.cc/PrRfhrLh/Diseno-sin-titulo-removebg-preview.png" alt="Project logo"></a>
</p>

<h1 align="center">E-28</h1>

# Extrusora de Filamento para la WRO

Este repositorio contiene el firmware para el proyecto **E-28**, una extrusora de filamento de plástico PET desarrollada para participar en las regionales de la **World Robot Olympiad (WRO)** en la categoría de **Future Innovators**.

El objetivo del proyecto es demostrar una solución innovadora y sostenible para el reciclaje de plástico, transformándolo en filamento útil para impresoras 3D.

El sistema se compone de dos módulos de control principales que funcionan en conjunto: un control de temperatura PID y un control para el motor de extrusión.

---

## Módulos del Firmware

### 1. Control de Temperatura PID (`PID_temperature_control.ino`)

Este componente es el cerebro de la operación de fusión. Se encarga de mantener el plástico a una temperatura precisa y constante, lo cual es crítico para obtener un filamento de diámetro y calidad uniformes.

**Función:**
- Utiliza un algoritmo PID (Proporcional-Integral-Derivativo) para regular un elemento calefactor.
- Lee la temperatura de un sensor y ajusta la potencia para alcanzar y mantener la temperatura objetivo (setpoint) que se le indique.

**Características:**
- Control PID para una gestión precisa de la temperatura.
- Visualización de la temperatura en tiempo real y del setpoint en una pantalla LCD 20x4 I2C.
- Menú interactivo para ajustar la temperatura objetivo y las constantes del PID (Kp, Ki, Kd) sobre la marcha.
- Utiliza un amplificador de termopar MAX6675 para una detección de temperatura fiable.
- Salida PWM para controlar el elemento calefactor (a través de un MOSFET o relé).

### 2. Control del Motor de Extrusión (`motorPasoPaso.ino`)

Una vez que el plástico está a la temperatura correcta, este módulo se encarga de empujarlo a través de la boquilla a una velocidad constante para formar el filamento.

**Función:**
- Controla un motor a pasos que acciona el tornillo de extrusión.
- La velocidad constante del motor es esencial para asegurar que el diámetro del filamento sea consistente a lo largo de toda su longitud.

**Características:**
- Movimiento del motor suave y fiable.
- Fácil de configurar para diferentes drivers de motor a pasos.
- Utiliza la librería `AccelStepper` para un control preciso de la velocidad.

---

## 🏁 Puesta en Marcha

### Prerrequisitos

- [Arduino IDE](https://www.arduino.cc/en/software) instalado en tu ordenador.
- El hardware completo de la extrusora ensamblado.

**Componentes Electrónicos Principales:**
- Placa Arduino (ej. Uno, Nano)
- **Para el control de temperatura:**
    - Amplificador de Termopar MAX6675 y Termopar tipo K
    - Pantalla LCD I2C 20x4
    - Encoder Rotativo y Pulsador
    - Elemento calefactor y su interfaz de potencia (MOSFET, relé de estado sólido, etc.)
- **Para el control del motor:**
    - Motor a pasos
    - Driver para motor a pasos (ej. A4988, DRV8825)

### Instalación de Librerías

1.  Abre el Arduino IDE.
2.  Ve a `Programa` > `Incluir Librería` > `Administrar Bibliotecas...`.
3.  Busca e instala las siguientes librerías:
    - `LiquidCrystal_I2C` por Frank de Brabander
    - `AccelStepper` por Mike McCauley

---

## 🎈 Uso

Debido a que son dos componentes de un mismo sistema, la integración final dependerá del diseño de tu placa o de cómo los conectes. Puedes:
- **Opción 1 (Recomendado):** Combinar ambos códigos en un único sketch de Arduino. Deberás gestionar los `setup()` y `loop()` de forma conjunta y asegurarte de que no haya conflictos de pines.
- **Opción 2:** Usar dos Arduinos separados que se comuniquen entre sí (por ejemplo, mediante I2C o UART), aunque esto añade complejidad.

**Para cargar el código:**
1.  Abre el sketch deseado (o el sketch combinado) en el Arduino IDE.
2.  Conecta tu placa Arduino al ordenador.
3.  Selecciona la Placa y el Puerto correctos en el menú `Herramientas`.
4.  Haz clic en el botón `Subir`.

---

## ✍️ Autores

- **David C.A** 
- **Arberto Medina** 
- **Daniel Cazorla** 

---

## 📄 Licencia

Este proyecto está bajo la Licencia MIT.