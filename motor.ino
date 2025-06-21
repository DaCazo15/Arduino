#include "AccelStepper.h"

#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  // Establece la velocidad máxima en pasos por segundo
  stepper.setMaxSpeed(1000);
}

void loop() {
  // Establece la velocidad en pasos por segundo
  stepper.setSpeed(400);
  // Mueve el motor con una velocidad constante 
  // Establecida por setSpeed()
  stepper.runSpeed();
}
