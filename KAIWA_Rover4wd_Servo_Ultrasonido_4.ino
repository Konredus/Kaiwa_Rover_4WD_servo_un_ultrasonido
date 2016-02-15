//-----------------------------------------------------------------
//          Programa hecho por: Konrad Peschka
//          Entidad: Kaiwa Technology
//          Fecha: Enero 2016
//-----------------------------------------------------------------


//--------------defino librerias y variables para Ultrasonidos------------------


#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

#include <NewPing.h>

#define MAX_DISTANCE 100 // Maximum distance (in cm) to ping.

NewPing ultrasonido_frente(A0,A1, MAX_DISTANCE);

#include <Servo.h>

Servo servo_frente; 

//-----------------------------------------------------------------------
//                Definicion Variables del Programa
//------------------------------------------------------------------

//#define debug                  //comentando o descomentando esta linea activo el codigo para debug


const int distancia_critica = 15;
const int distancia_deteccion=40;
const int distancia_deteccion_lejano=70;

int velocidad=150;
const int velocidad_maxima=150;
const int velocidad_medio=100;
const int velocidad_giro=180;
const int velocidad_atras=180;

const int tiempo_stop=50;
const int tiempo_giro=200;
const int tiempo_atras=200;
const int tiempo_frente=10;
const int tiempo_servo=90;

const int servo_medio=100;
const int servo_der=50;
const int servo_izq=150;

int distancia_der=0;
int distancia_der_anterior=30;
int distancia_centro=0;
int distancia_centro_anterior=30;
int distancia_izq=0;
int distancia_izq_anterior=30;

int aux_matriz=0;

//-----------------------------------------------------
//                  Funcion SETUP
//-------------------------------------------------------

void setup()
{

 servo_frente.attach(9);
 servo_frente.write(100);
 
#ifdef debug
 Serial.begin(9600);
#endif

  pinMode(MOTORLATCH, OUTPUT);
  pinMode(MOTORENABLE, OUTPUT);
  pinMode(MOTORDATA, OUTPUT);
  pinMode(MOTORCLK, OUTPUT);

  pinMode(10, INPUT);
 
  movimiento_stop();

}

//--------------------------------------------------------
//              Funcion principal del codigo
//------------------------------------------------------


void loop()
{
while(digitalRead(10)==LOW)
{
  
  #ifdef debug
  Serial.print("Boton Arranque\n");
  #endif
  delay(50);
  
}


//--------------UNA VEZ PULSADO ENTRA AL WHILE INFINITO----------------------


while(1)   //se queda en este loop luego de haber pulsado el pulsador de arranque
  {
    lectura_ultrasonido();
    if ((distancia_centro == 0) || ( distancia_centro > distancia_deteccion_lejano)) 
    {
      movimiento_frente();
      delay(tiempo_frente);
      velocidad=velocidad_maxima;
    }
    if ( (distancia_centro <= distancia_deteccion_lejano) && ( distancia_centro >= distancia_deteccion))
    {
      movimiento_frente();
      delay(tiempo_frente);
      velocidad=velocidad_medio;
    } 
    
    else
    {
      if ( (distancia_centro <= distancia_deteccion) && ( distancia_centro >= distancia_critica))
      {        
        mira_costados();
        decide_giro();
      } 
      else 
      {
        if ( (distancia_centro < distancia_critica) && (distancia_centro !=0))
        {
          movimiento_stop();
          delay(tiempo_stop);
          movimiento_atras();
          delay(tiempo_atras);
       }
     }
   }
  }
}


//---------------------------------------------------------------------------------------
//                    FUNCION LECTURA ULTRASONIDO
//---------------------------------------------------------------------------------------

void lectura_ultrasonido(void)
{
   servo_frente.write(servo_medio);
   delay(tiempo_servo);
   distancia_centro=ultrasonido_frente.ping_cm();

   #ifdef debug
   Serial.print("Centro= ");
   Serial.print(distancia_centro);
   #endif
  
  
}

//---------------------------------------------------------------------------------------
//                    FUNCION MIRA Y LEE PARA LOS COSTADOS
//---------------------------------------------------------------------------------------

void mira_costados(void)
{
   movimiento_stop();
   delay(tiempo_stop);
    
   servo_frente.write(servo_der);
   delay(tiempo_servo);
   distancia_der=ultrasonido_frente.ping_cm();

   servo_frente.write(servo_medio);
   delay(tiempo_servo);
   
   servo_frente.write(servo_izq);
   delay(tiempo_servo);
   distancia_izq=ultrasonido_frente.ping_cm();

   servo_frente.write(servo_medio);
   delay(tiempo_servo);
   distancia_centro=ultrasonido_frente.ping_cm();

   #ifdef debug
   Serial.print("Der= ");
   Serial.print(distancia_der);
   Serial.print("Izq= ");
   Serial.print(distancia_izq);
   #endif

}

//---------------------------------------------------------------------------------------
//                    FUNCION PARA DECIDIR HACIA QUE COSTADO GIRAR
//---------------------------------------------------------------------------------------

void decide_giro(void)
{
  if(distancia_der>distancia_izq)
  {
    movimiento_derecha();
    delay(tiempo_giro);
  }
  else
  {
    if(distancia_izq>distancia_der)
    {
    movimiento_izquierda();
    delay(tiempo_giro);
    }
  }
}
// -------------------------------------------------------------
//               Definimos librerias de movimiento
//--------------------------------------------------------------

void movimiento_stop(void)
{
 motor1.setSpeed(1);  
 motor2.setSpeed(1); 
 motor3.setSpeed(1); 
 motor4.setSpeed(1); 
}

void movimiento_frente(void)
{

 motor1.setSpeed(velocidad);  
 motor2.setSpeed(velocidad); 
 motor3.setSpeed(velocidad); 
 motor4.setSpeed(velocidad); 

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
}

void movimiento_atras(void)
{
 motor1.setSpeed(velocidad_atras);  
 motor2.setSpeed(velocidad_atras); 
 motor3.setSpeed(velocidad_atras); 
 motor4.setSpeed(velocidad_atras); 

 
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
}

void movimiento_derecha(void)
{
 motor1.setSpeed(velocidad_giro);  
 motor2.setSpeed(velocidad_giro); 
 motor3.setSpeed(velocidad_giro); 
 motor4.setSpeed(velocidad_giro); 

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

}

void movimiento_izquierda(void)
{
 motor1.setSpeed(velocidad_giro);  
 motor2.setSpeed(velocidad_giro); 
 motor3.setSpeed(velocidad_giro); 
 motor4.setSpeed(velocidad_giro); 

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);

}


