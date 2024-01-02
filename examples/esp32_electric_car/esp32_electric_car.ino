/*
 * Devveloped by Victor Julio Monge Rojas

 * Description: Test of the basic funtions of the library for
   controlling the electric car.

   Default connections for esp32

   Servo: 17

   Motor A:
    in1: 15
    in2: 21
    pinPWM (velocidad): 0

   Motor B:
    in1: 22
    in2: 23
    pinPWM (velocidad): 21

   Change configurations in the EV-settings.h file inside the library.
   
*/

// Include the library
#include <ElectricCar.h>

// Create electric car intance
ElectricCar EV;

void setup(){
  Serial.begin(115200);

  if(EV.begin()){ // Pins: servo, in1, in2, spA (speed pin), in3, in4, spB
    Serial.println("Electric car initialized");
  } else{
    Serial.println("Error during initialization of electric car");
  }

  // Activate progresive aceleration
  //------------------------
  EV.changeSlowAccelerationA(true);


  // Probar motor A
  //------------------------
  EV.forwardMotorA(10, 2000);  // Go foward at 10% speed during 1 second (when time is not defined, the motor does not stop)
  //EV.stopMotorA();           // Stop the motor manually: when a time is not specified.
  delay(2000);                 // wait with the motor stopped
  EV.backwardMotorA(10, 2000);
}

void loop(){
  // Probar servo
  //-------------------------
  EV.center();
  delay(1000);
  EV.left();
  delay(1000);
  EV.right();
  delay(1000);
  EV.center();
}

