/*
 * Main Arduino sketch for Blui Line follower.
   Updates should be posted on GitHub: https://github.com/bluecircuits/Seguidor-de-linea
   
   Bluecircuits
   July. 19, 2019
   Copyright (c) 2019 Bluecircuits.
   
   This sketch may also includes others' codes under MIT or other open source liscence.
   Check those liscences in corresponding module test folders.
   Feel free to contact us if you find any missing references.
   The MIT License
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 * 
 */
#include "Wire.h"
#include "sensorbar.h"

const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)
#define CASO1 1
#define CASO2 2
#define CASO3 3
#define CASO4 4
#define CASO5 5
#define CASO6 6
#define CASO7 7

SensorBar mySensorBar(SX1509_ADDRESS);

int v=200;
int v1=200;
int E1 = 6;
int M1 = 12;

int E2 =11;
int M2 = 13;
int cont=0;
uint8_t state;
//Funciones motor

void adelante() {
  v=110;
  digitalWrite(M1,HIGH);
  digitalWrite(M2,HIGH);
  analogWrite(E1, v);
  analogWrite(E2, v);
  delay(20);
}
void derecha() {
  v=100;//240
  v1=20;//50
  digitalWrite(M1,HIGH);
  digitalWrite(M2,HIGH);
  analogWrite(E1, v);
  analogWrite(E2, v1);
  delay(20);
}
void izquierda(){
  v=30;
  v1=110;
  digitalWrite(M1,HIGH);
  digitalWrite(M2,HIGH);
  analogWrite(E1, v);
  analogWrite(E2, v1);
  delay(20);
}
void parar() {
  v=0;
  digitalWrite(M1,HIGH);
  digitalWrite(M2,HIGH);
  analogWrite(E1, v);
  analogWrite(E2, v);
  delay(20);
}
void giro90() {
  v=100;
  v1=0;
  digitalWrite(M1,HIGH);
  digitalWrite(M2,HIGH);
  analogWrite(E1, v);
  analogWrite(E2, v1);
  delay(700);
}

//Funciones motor

void setup()
{
  Serial.begin(9600);  // start serial for output
  Serial.println("Program started.");
  Serial.println();
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT); 
  mySensorBar.clearBarStrobe();
  mySensorBar.setInvertBits();  
  uint8_t returnStatus = mySensorBar.begin();
  state=CASO1;
  if(returnStatus)Serial.println("sx1509 IC communication OK");
  Serial.println();
}

void loop()
{
  //4-9-13-18
  int pos=(mySensorBar.getPosition());
  int den=mySensorBar.getDensity();
  uint8_t data=mySensorBar.getRaw();
  switch (state) {
  case CASO1:
        if(den<=7){
          if(pos<0){
            izquierda();
            }
          if(pos>0){
            derecha();
            //Serial.println("Derecha");
            }
          if(pos==0){
            adelante();
            //Serial.println("Adelante");
            }
        }
        if(den<4){
          state=CASO2;
          }
        break;
  case CASO2:
        parar();
        delay(1000);
        giro90();
        adelante();
        delay(800);
        parar();
        delay(1000);
        state=CASO3;
        break;
  case CASO3:
        if(den<=7){
          if(pos<0){
            izquierda();
            }
          if(pos>0){
            derecha();
            //Serial.println("Derecha");
            }
          if(pos==0){
            adelante();
            //Serial.println("Adelante");
            }
        }
        if(cont>=50){
          if(data==0x21 || data==0x42 || data==0x84){
            state=CASO4;
            cont=0;
            }
        }else{
          cont++;
        }
        break;
  case CASO4:
        parar();
        break;
  default:
    parar();
    break;
  } 
}
