#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<maf.h>
#define N 120
 
LiquidCrystal_I2C lcd(0x27,16,2);
// Declaracion de variables globales //
float buf[N];  //  buffer 1
float buf2[N]; //  buffer 2
float Ip; // Variable para almacenar el valor obtenido del sensor (0 a 1023)
float Ipa;     //  Variable
float Ipm;     //  Variable
int pin = A0; // Variable del pin de entrada del sensor (A0)
CircularBuffer cb(buf,N);
CircularBuffer cb2(buf2,N);

unsigned long t1_start, t1_stop;
unsigned long t2_start, t2_stop;
unsigned long t3_start, t3_stop;

void setup() {
  //Iniciar el LCD
  lcd.begin();
  //Encender la luz de fondo.
  lcd.backlight(); 
  // Escribimos el Mensaje en el LCD.
  lcd.print("Corriente");
  // Configuramos el puerto serial a 9600 bps
  Serial.begin(9600);
  t1_start = micros();
  t2_start = millis();
  t3_start = millis();
}

void loop() {
  float y, z;

  //t1_stop = micros();
  //if(t1_stop - t1_start > 1){
    // Con analogRead leemos el sensor, recuerda que es un valor de 0 a 1023
    Ip = analogRead(pin); 
    // Almacenamos la medida Ip en el buffer circular cb
    cb.push(Ip);
    // guardamos el promedio del valor del buffer cb en la variable y
    y = cb.mean();
    // Restamos el valor Ip-y y lo guardamos en z
    z = Ip-y;
    // Almacenamos la medida z*z en el buffer circular cb2
    cb2.push(z*z);
    // Calculamos  la raiz de cada valor promedio de z
    Ipa=sqrt(cb2.mean());
    // Hallamos el valor de cada corriente para valor de Ipa
    //  Ipm=1.4503*Ipa-3.7083;
    Ipm=1.263*Ipa-3.94;
    delay(1);
  //  t1_start = t1_stop;
  //}
  
   /////////////Dibujar en el puerto serial /////////////
   t2_stop = millis();
   if(t2_stop - t2_start > 30){
     //Serial.print(Ip);    Serial.print(" ");
     //Serial.print(y);       Serial.print(" ");
     Serial.print(abs(z));  Serial.print(" ");
     Serial.println(Ipm);
     t2_start = t2_stop;
   }  
  
   //////////////////////////////////////////////
   ///////////Mostrar valor en la pantalla LCD 16*2/////////////
   t3_stop = millis();
   if(t3_stop - t3_start > 450){
     lcd.setCursor(0, 1);
     lcd.print(Ipm);
     lcd.setCursor(7, 1);
     lcd.print(" mA");
     t3_start = t3_stop;
   }
  
}
