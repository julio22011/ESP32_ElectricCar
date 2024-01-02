/* Library developped by Victor Julio Monge Rojas
    * Descripcion: libreria para controlar funciones
    básicas del carro eléctrico.
*/

// Settings
#include "EV-settings.h"

//--------------------------------------------------------------------
#ifndef ElectricCar_h  // prevents executing this file more than once
#define ElectricCar_h

// Aditional libraries needed
#ifdef ESP32  // ESP32
  #include <ESP32Servo.h>
#else  // Arduino
  #include <Servo.h>
#endif


class ElectricCar{
    public:
        ElectricCar();
        bool begin(uint8_t servoPin,
              uint8_t in1, uint8_t in2, uint8_t spA
              #ifdef twoMotors
              , uint8_t in3, uint8_t in4, uint8_t spB
              #endif
        );

        // Servo
        Servo servo;

        void left();
        void right();
        void center();
        int currentServoDegree = servoDegreeCenter;

        // Set pins for control
        void setPinsMotorA(uint8_t in1, uint8_t in2, uint8_t spA);
        void setPinsMotorB(uint8_t in3, uint8_t in4, uint8_t spB);

        // Funtions of momevent
        void forwardMotorA(int speed, int milliseconds);
        void backwardMotorA(int speed, int milliseconds);
        void stopMotorA();
        void setSpeedMotorA(int speed);   // speed between 0 and 100%
        void changeSlowAccelerationA(bool activate);
        #ifdef twoMotors
        void forwardMotorB(int speed, int milliseconds);
        void backwardMotorB(int speed, int milliseconds);
        void stopMotorB();
        void setSpeedMotorB(int speed);   // speed between 0 and 4095
        void changeSlowAccelerationB(bool activate);
        #endif

        // General settings
        //----------------------------------
        bool slowAccelerationA = false;  // slow acceleration
        bool slowAccelerationB = false;  // slow acceleration

        // Ajustes por defecto para el esp32
        //----------------------------------
        #ifdef ESP32
        // Conexition to the microcontroller ESP32

        // Servo: direccion del carro
        uint8_t servoPin;   // pin de servo

        // Motor A
        uint8_t in1;   // pin de direccion 1
        uint8_t in2;   // pin de direccion 2
        uint8_t spA;    // pin de velocidad por PWM
        int speedA = defaultInitialSpeedA; // velocidad

        #ifdef twoMotors
        // Motor B
        uint8_t in3;    // pin de direccion 1
        uint8_t in4;    // pin de direccion 2
        uint8_t spB;    // pin de velocidad por PWM   
        int speedB = defaultInitialSpeedB;  // velocidad
        #endif // twoMotors

        #endif // ESP32

};

ElectricCar::ElectricCar(){
  // Constructor
  // ...
}

// Ajustab/e settings during execution
//----------------------------------
void ElectricCar::changeSlowAccelerationA(bool activate = true){
    slowAccelerationA = activate;
}

#ifdef twoMotors
void ElectricCar::changeSlowAccelerationB(bool activate = true){
    slowAccelerationB = activate;
}
#endif

// Control del servo motor
//-------------------------------
void ElectricCar::left(){
    #ifdef activateLog
    Serial.println("Log: left");
    #endif

    // cambiar poco a poco la posicion del servo a servoDegreeLeft
    if (currentServoDegree > servoDegreeLeft){
        for (int i = currentServoDegree; i >= servoDegreeLeft; i--){
            servo.write(i);
            delay(servoAccelMS);
        }
    }else{
        for (int i = currentServoDegree; i <= servoDegreeLeft; i++){
            servo.write(i);
            delay(servoAccelMS);
        }
    }
    currentServoDegree = servoDegreeLeft;
}

void ElectricCar::right(){
    #ifdef activateLog
    Serial.println("Log: right");
    #endif

    // cambiar poco a poco la posicion del servo a servoDegreeRight
    if (currentServoDegree < servoDegreeRight){
        for (int i = currentServoDegree; i <= servoDegreeRight; i++){
            servo.write(i);
            delay(10);
        }
    }else{
        for (int i = currentServoDegree; i >= servoDegreeRight; i--){
            servo.write(i);
            delay(10);
        }
    }
    currentServoDegree = servoDegreeRight;
}

void ElectricCar::center(){
    #ifdef activateLog
    Serial.println("Log: center");
    #endif

    // cambiar poco a poco la posicion del servo a servoDegreeCenter
    if (currentServoDegree > servoDegreeCenter){
        for (int i = currentServoDegree; i >= servoDegreeCenter; i--){
            servo.write(i);
            delay(10);
        }
    }else{
        for (int i = currentServoDegree; i <= servoDegreeCenter; i++){
            servo.write(i);
            delay(10);
        }
    }
    currentServoDegree = servoDegreeCenter;
}

// Asignacion de pines
//-------------------------------
void ElectricCar::setPinsMotorA(uint8_t in1_n, uint8_t in2_n, uint8_t spA_n){
    in1 = in1_n;
    in2 = in2_n;
    spA = spA_n;

    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(spA, OUTPUT);
    stopMotorA();
}


void ElectricCar::setPinsMotorB(uint8_t in3_n, uint8_t in4_n, uint8_t spB_n){
    #ifdef twoMotors
    in3 = in3_n;
    in4 = in4_n;
    spB = spB_n;
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(spB, OUTPUT);
    stopMotorB();
    #else
    Serial.println("Error: twoMotors is not defined in settings.h");
    #endif
}

// Control de movimiento de motores
//-------------------------------
void ElectricCar::stopMotorA(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
}

void ElectricCar::setSpeedMotorA(int speed = defaultSpeedA){
    int valor = speed*resolutionSpeed/100;
    analogWrite(spA, valor);

    #ifdef activateLog
    Serial.print("Log: setSpeedMotorA: ");
    Serial.println(valor);
    #endif
}

void ElectricCar::forwardMotorA(int speed = defaultSpeedA, int milliseconds = 0){
    #ifdef activateLog
    Serial.println("Log: forwardMotorA");
    #endif

    // Selección de dirección
    #ifdef invertMotorA
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    #else    
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    #endif

    // Aceleracion progresiva o inmediata
    if(slowAccelerationA){
        for (int i = 0; i <= speed; i++){
            setSpeedMotorA(i);
            delay(motorAccelMS_A);
        }
    }else{  
        setSpeedMotorA(speed);
    }

    // Esperar el tiempo especificado
    delay(milliseconds);

    // No detener el motor si no se especifica un tiempo
    if (milliseconds > 0){
        stopMotorA();
    }
}

void ElectricCar::backwardMotorA(int speed = defaultSpeedA, int milliseconds = 0){
    #ifdef activateLog
    Serial.println("Log: backwardMotorA");
    #endif

    #ifdef invertMotorA
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    #else
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    #endif

    // Aceleracion progresiva o inmediata
    if(slowAccelerationA){
        for (int i = 0; i <= speed; i++){
            setSpeedMotorA(i);
            delay(motorAccelMS_A);
        }
    }else{
        setSpeedMotorA(speed);
    }

    // Esperar el tiempo especificado
    delay(milliseconds);

    // No detener el motor si no se especifica un tiempo
    if (milliseconds > 0){
        stopMotorA();
    }
}

#ifdef twoMotors
void ElectricCar::stopMotorB(){
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void ElectricCar::setSpeedMotorB(int speed = defaultSpeedB){
    analogWrite(spB, speed*resolutionSpeed/100);
}

void ElectricCar::forwardMotorB(int speed = defaultSpeedB, int milliseconds = 0){

    // Selección de dirección
    #ifdef invertMotorB
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    #else
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    #endif

    // Aceleracion progresiva o inmediata
    if(slowAccelerationB){
        for (int i = 0; i <= speed; i++){
            setSpeedMotorB(i);
            delay(motorAccelMS_B);
        }
    }else{
        setSpeedMotorB(speed);
    }

    // Esperar el tiempo especificado
    delay(milliseconds);

    // No detener el motor si no se especifica un tiempo
    if (milliseconds > 0){
        stopMotorB();
    }
}

void ElectricCar::backwardMotorB(int speed = defaultSpeedB, int milliseconds = 0){

    // Selección de dirección
    #ifdef invertMotorB
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    #else
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    #endif

    // Aceleracion progresiva o inmediata
    if(slowAccelerationB){
        for (int i = 0; i <= speed; i++){
            setSpeedMotorB(i);
            delay(motorAccelMS_B);
        }
    }else{
        setSpeedMotorB(speed);
    }

    // Esperar el tiempo especificado
    delay(milliseconds);

    // No detener el motor si no se especifica un tiempo
    if (milliseconds > 0){
        stopMotorB();
    }
}
#endif


// Inicialzacion de la libreria
//-------------------------------
bool ElectricCar::begin(uint8_t servoPin = defaultServoPin,
              uint8_t in1 = defaultIn1, uint8_t in2 = defaultIn2, uint8_t spA = defaultSpA
              #ifdef twoMotors
              , uint8_t in3 = defaultIn3, uint8_t in4 = defaultIn4, uint8_t spB = defaultSpB
              #endif
        ){
    // Save and init pins
    servoPin = servoPin;
    setPinsMotorA(in1, in2, spA);
    #ifdef twoMotors
    setPinsMotorB(in3, in4, spB);
    #endif

    #ifdef activateLog 
    Serial.println("Log: Pins saved");
    #endif

    // Init servo
    servo.setPeriodHertz(50); 
    servo.attach(servoPin);
    //center();

    return true;
}


#endif // ElectricCar_h
