#include <SPI.h>

//Configuaracion de pantalla
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  

int PWM_pin = 3;  //PWM
int clk = 8;      int data = 9;     


//Variables del progragama
float set_temperature = 0;
float temperature_read = 0.0;
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time, timePrev;
float PID_value = 0;
int button_pressed = 0;
int menu_activated=0;
float last_set_temperature = 0;

//Varaiables 
int clk_State;
int Last_State;  
bool dt_State;  

//PID 
//////////////////////////////////////////////////////////
int kp = 150;   // antes 90
int ki = 60;    // antes 30
int kd = 80;    // puedes dejar igual o ajustar si oscila mucho
//////////////////////////////////////////////////////////

int PID_p = 0;    int PID_i = 0;    int PID_d = 0;
float last_kp = 0;
float last_ki = 0;
float last_kd = 0;
int PID_values_fixed =0;

//Pines del sensor de temperatura
#define MAX6675_CS   10
#define MAX6675_SO   12
#define MAX6675_SCK  13

void setup() {
  pinMode(PWM_pin,OUTPUT);
  TCCR2B = TCCR2B & B11111000 | 0x03;    
  Time = millis();
  
  Last_State = (PINB & B00000001);    

  PCICR |= (1 << PCIE0);                                                    
  PCMSK0 |= (1 << PCINT0);  
  PCMSK0 |= (1 << PCINT1); 
  PCMSK0 |= (1 << PCINT3);    
                           
  pinMode(11,INPUT);
  pinMode(9,INPUT);
  pinMode(8,INPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {

if(menu_activated==0)
{
  temperature_read = readThermocouple();
  PID_error = set_temperature - temperature_read;
  PID_p = 0.01*kp * PID_error;
  PID_i = 0.01*PID_i + (ki * PID_error);
  

  timePrev = Time;                     
  Time = millis();     
  elapsedTime = (Time - timePrev) / 1000; 
  PID_d = 0.01*kd*((PID_error - previous_error)/elapsedTime);
  PID_value = PID_p + PID_i + PID_d;

  if(PID_value < 0)
  {    PID_value = 0;    }
  if(PID_value > 255)  
  {    PID_value = 255;  }
  analogWrite(PWM_pin,255-PID_value);
  previous_error = PID_error;     

  delay(250); 
  
  lcd.setCursor(0,0);
  lcd.print("PID TEMP control");
  lcd.setCursor(0,1);
  lcd.print("S:");
  lcd.setCursor(2,1);
  lcd.print(set_temperature,1);
  lcd.setCursor(9,1);
  lcd.print("R:");
  lcd.setCursor(11,1);
  lcd.print(temperature_read,1);
}
if(menu_activated == 1)
{
   analogWrite(PWM_pin,255);
  if(set_temperature != last_set_temperature)
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set  temperature");    
  lcd.setCursor(0,1);
  lcd.print(set_temperature);  
  }
  last_set_temperature = set_temperature;
}
if(menu_activated == 2)
{
  if(kp != last_kp)
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set   P  value  ");    
  lcd.setCursor(0,1);
  lcd.print(kp);  
  }
  last_kp = kp;
}if(menu_activated == 3)
{
  if(ki != last_ki)
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set   I  value  ");    
  lcd.setCursor(0,1);
  lcd.print(ki);  
  }
  last_ki = ki;
}
if(menu_activated == 4)
{
  
  if(kd != last_kd)
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set   D  value  ");    
  lcd.setCursor(0,1);
  lcd.print(kd);  
  }
  last_kd = kd;
}
  
}
double readThermocouple() {

  uint16_t v;
  pinMode(MAX6675_CS, OUTPUT);
  pinMode(MAX6675_SO, INPUT);
  pinMode(MAX6675_SCK, OUTPUT);
  
  digitalWrite(MAX6675_CS, LOW);
  delay(1);

  v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  v <<= 8;
  v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  
  digitalWrite(MAX6675_CS, HIGH);
  if (v & 0x4) 
  {    
    return NAN;     
  }
  v >>= 3;
  return v*0.25;
}
ISR(PCINT0_vect){
if(menu_activated==1)
   {
  clk_State =   (PINB & B00000001); 
  dt_State  =   (PINB & B00000010); 
  if (clk_State != Last_State){     
     if (dt_State != clk_State) { 
       set_temperature = set_temperature + 0.5;
     }
     else {
       set_temperature = set_temperature - 0.5;
     } 
     if(set_temperature > 250) set_temperature = 250;
     if(set_temperature < 0) set_temperature = 0;
  }
  Last_State = clk_State; 
}
if(menu_activated==2)
   {
  clk_State =   (PINB & B00000001);
  dt_State  =   (PINB & B00000010); 
  if (clk_State != Last_State){     
     if (dt_State != clk_State) { 
       kp = kp+1 ;
     }
     else {
       kp = kp-1;
     } 
  }
  Last_State = clk_State; 
} 
if(menu_activated==3)
   {
  clk_State =   (PINB & B00000001); 
  dt_State  =   (PINB & B00000010); 
  if (clk_State != Last_State){     
     if (dt_State != clk_State) { 
       ki = ki+1 ;
     }
     else {
       ki = ki-1;
     } 
  }
  Last_State = clk_State; 
}
 if(menu_activated==4)
   {
  clk_State =   (PINB & B00000001);
  dt_State  =   (PINB & B00000010); 
  if (clk_State != Last_State){     
     if (dt_State != clk_State) { 
       kd = kd+1 ;
     }
     else {
       kd = kd-1;
     } 
  }
  Last_State = clk_State; 
}
  if (PINB & B00001000) 
  {       
    button_pressed = 1;
  } 
  else if(button_pressed == 1)
  {
   if(menu_activated==4)
   {
    menu_activated = 0;  
    PID_values_fixed=1;
    button_pressed=0; 
    delay(1000);
   }
   if(menu_activated==3)
   {
    menu_activated = menu_activated + 1;  
    button_pressed=0; 
    kd = kd + 1; 
    delay(1000);
   }
   if(menu_activated==2)
   {
    menu_activated = menu_activated + 1;  
    button_pressed=0; 
    ki = ki + 1; 
    delay(1000);
   }
   if(menu_activated==1)
   {
    menu_activated = menu_activated + 1;  
    button_pressed=0; 
    kp = kp + 1; 
    delay(1000);
   }
   if(menu_activated==0 && PID_values_fixed != 1)
   {
    menu_activated = menu_activated + 1;  
    button_pressed=0;
    set_temperature = set_temperature+1;   
    delay(1000);
   }
   PID_values_fixed = 0;
  }  
}
