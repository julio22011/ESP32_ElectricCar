
/*
    Archivo para aplicar los ajustes de la libreria de carro electrico
*/

// General settings
//--------------------------------------------------------------
#define ESP32                     //Comentar esta linea si se usa Arduino
//#define twoMotors               // Comentar esta linea si se usa un solo motor
#define resolutionSpeed 1023      // resolucion de velocidad para el PWM
#define servoDegreeLeft 115       // grados para girar a la izquierda
#define servoDegreeRight 65       // grados para girar a la derecha
#define servoDegreeCenter 90      // grados para centrar el servo
#define motorAccelMS_A 50         // tiempo de aceleracion: espera por cada 1% aumentado (milisegundos)
#define motorAccelMS_B 10         // tiempo de aceleracion: espera por cada 1% aumentado (milisegundos)
#define servoAccelMS 10           // tiempo de aceleracion: espera por cada 1% aumentado (milisegundos)

// Invertir la direccion de los motores
//#define invertMotorA  // invertir direccion del motor A
//#define invertMotorB  // invertir direccion del motor B

#define activateLog


// Settings connections
//--------------------------------------------------------------
#define defaultServoPin 17  // pin de servo
#define defaultIn1 15   // pin de direccion 1
#define defaultIn2 32   // pin de direccion 2
#define defaultSpA 0    // pin de velocidad por PWM
#define defaultSpeedA 50  // velocidad por defecto al hacer forward, backward, left, right, etc
#define defaultInitialSpeedA 0 // velocidad

#ifdef twoMotors
// Motor B
#define defaultIn3 22    // pin de direccion 1
#define defaultIn4 23    // pin de direccion 2
#define defaultSpB 21    // pin de velocidad por PWM
#define defaultSpeedB 50  // velocidad por defecto al hacer forward, backward, left, right, etc
#define defaultInitialSpeedB 0  // velocidad
#endif // twoMotors


// Other settings
//--------------------------------------------------------------
// ...
