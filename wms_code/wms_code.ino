//il ne faut pas changer la quantité  d eau d un reservoir au moment du traitement d eau

#include "RTClib.h"
RTC_DS3231 rtc;

#include <Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);

#include "DHT.h"
#define DHTTYPE DHT22//(AM2302)
int maxHum = 60;
int maxTemp = 40;
int DHTPIN=31;
DHT dht(DHTPIN, DHTTYPE);

int pump_24v = 23;//in1
int pump_grey_water1 = 24;//in2
int pump_grey_water2 = 25;//in3


int electrovalve3 = 26;//in4
int electrovalve5 = 27;//in5
int electrovalve2 = 28;//in6
int electrovalve4 = 29;//in7
int electrovalve1 = 30;//in8

int Back_led=13;

int trigPin_drinking_water = 40;
int echoPin_drinking_water = 41;

int trigPin_grey_water = 36;
int echoPin_grey_water = 37;

int trigPin_Primary_supply_water = 38;
int echoPin_Primary_supply_water = 39;

long duration;
double level;
double lev_b;
double lev_g;
double lev_d;
double lev_b_2;
double lev_g_2;
double lev_d_2;
int rest=0;
int i;

int cmpt=0;
int lcd_button=42;
int nb=2;
int nb_fois=0;
int permission_b=1;
int permission_g=1;
int permission_d=1;
int sink_but=A7;

int sensorPin1 = A1; 
int sensorPin2 = A2;   
int sensorValue1 = 0;  
int percent1 = 0;
int sensorValue2 = 0;  
int percent2 = 0;


void setup()
{
    pinMode(pump_grey_water1,OUTPUT);
    pinMode(pump_grey_water2,OUTPUT);

    digitalWrite(pump_grey_water1,HIGH);
    digitalWrite(pump_grey_water2,HIGH);

    digitalWrite(pump_24v,HIGH);
    pinMode(pump_24v,OUTPUT);

    pinMode(electrovalve1,OUTPUT);
    pinMode(electrovalve2,OUTPUT);
    pinMode(electrovalve3,OUTPUT);
    pinMode(electrovalve4,OUTPUT);
    pinMode(electrovalve5,OUTPUT);

    digitalWrite(electrovalve1,HIGH);
    digitalWrite(electrovalve2,HIGH);
    digitalWrite(electrovalve3,HIGH);
    digitalWrite(electrovalve4,HIGH);
    digitalWrite(electrovalve5,HIGH);

    pinMode(sink_but,INPUT);

    rtc.begin();
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //pour ajuster le temps et la date avec ceux de l'ordinateur
    //rtc.adjust(DateTime(2016,9,29,17,16,50));//pour ajuster le temps et la date avec notre propre choix
    DateTime now = rtc.now();

    dht.begin();

    Serial.begin(9600);

    pinMode(Back_led,OUTPUT);
    digitalWrite(Back_led,HIGH);//allumer les leds de LCD

    lcd.init();
    lcd.backlight();

    pinMode(trigPin_drinking_water, OUTPUT);
    pinMode(echoPin_drinking_water, INPUT);

    pinMode(trigPin_grey_water, OUTPUT);
    pinMode(echoPin_grey_water, INPUT);

    pinMode(trigPin_Primary_supply_water, OUTPUT);
    pinMode(echoPin_Primary_supply_water, INPUT);

    pinMode(lcd_button,INPUT);

    lev_b=get_level_Primary_supply_water();
    delay(500);
    lev_g=get_level_grey_water();
    lev_d=get_level_drinking_water();
    delay(500);
    digitalWrite(electrovalve1,LOW);
    delay(1500);
}

double get_level_drinking_water()
{
    digitalWrite(trigPin_drinking_water, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin_drinking_water, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_drinking_water, LOW);

    duration = pulseIn(echoPin_drinking_water, HIGH);

    level= ((duration*0.034/2)-18)/10;

    return abs(level);
}

double get_level_grey_water()
{
    digitalWrite(trigPin_grey_water, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin_grey_water, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_grey_water, LOW);

    duration = pulseIn(echoPin_grey_water, HIGH);

    level= ((duration*0.034/2)-18)/10;

    return abs(level);
}

double get_level_Primary_supply_water()
{
    digitalWrite(trigPin_Primary_supply_water, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin_Primary_supply_water, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_Primary_supply_water, LOW);

    duration = pulseIn(echoPin_Primary_supply_water, HIGH);

    level= ((duration*0.034/2)-18)/10;

    return abs(level);
}

void show_tank_level()
{
    lcd.setCursor(0,0);
    lcd.print(" Tank's water level ");
     
    lcd.setCursor(0,1);
    lcd.print("Prim sup water :");
    rest= (lev_b / 1.29)*100;

   if(rest < 100)
    {
    lcd.print(" ");
  if(rest < 10)
    lcd.print(" ");
    lcd.print(rest);
    }
    else
    lcd.print("100");

    lcd.print("%");
     
    lcd.setCursor(0,2);
    lcd.print("Grey water     :");
    rest= (lev_g / 1.29)*100;

   if(rest < 100)
    {
    lcd.print(" ");
  if(rest < 10)
    lcd.print(" ");
    lcd.print(rest);
    }
    else
    lcd.print("100");

    lcd.print("%");
     
    lcd.setCursor(0,3);
    lcd.print("Drinking water :");
    rest= (lev_d / 1.29)*100;

    if(rest < 100)
    {
    lcd.print(" ");
  if(rest < 10)
    lcd.print(" ");
    lcd.print(rest);
    }
    else
    lcd.print("100");

    lcd.print("%");
     


    // lcd.setCursor(0,0);
    // lcd.print(" Tank's water level ");
     
    // lcd.setCursor(0,1);
    // lcd.print("Prim sup water:");


    // lcd.print(lev_b);

     
    // lcd.setCursor(0,2);
    // lcd.print("Grey water    :");


    // lcd.print(lev_g);

     
    // lcd.setCursor(0,3);
    // lcd.print("Drinking water:");

    // lcd.print(lev_d);
}

void show_date_time()
{
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    DateTime now = rtc.now();
  
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.setCursor(0,0);
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print("  ");
  
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    if(now.day()<10)
    {
        lcd.print(0);
    }
    lcd.print(now.day());
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print('/');
    if(now.month()<10)
    { 
        lcd.print(0);
    }
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print(now.month());
    lcd.print('/');
    lcd.print(now.year());
     
    lcd.print(" ");

    if(now.hour()<10)
    {
         
        lcd.print(0);
    }

  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print(now.hour());
     
    lcd.print(':');
    if(now.minute()<10)
    {
        lcd.print(0);
         
    }
    lcd.print(now.minute());
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print("  ");
    /*lcd.print(':');
    if(now.second()<10)
      {
       lcd.print(0);
     }
    lcd.print(now.second());*/
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
}

void show_temprature()
{
    float t = dht.readTemperature();
    
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
  // delay(500);// Wait a few seconds between measurements
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.setCursor(0,1);
    lcd.print("Temperature : ");
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print(t,1);
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print("C ");
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
}

void show_humidity()
{
    float h = dht.readHumidity();
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    // delay(500);// Wait a few seconds between measurements
    
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
  lcd.setCursor(0,2);
     
    lcd.print("Humidity    : ");
    lcd.print(h,1);
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print("% ");
     
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }   
}

int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1023, 465, 0, 100);
  return percentValue;
}

void show_percent_soil_sensors()
{
    // sensorValue1 = analogRead(sensorPin1);
  // percent1 = convertToPercent(sensorValue1);
// if(percent1>=100)
  // percent1=100;
  
    if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  // sensorValue2 = analogRead(sensorPin2);
  // percent2 = convertToPercent(sensorValue2);

  // if(percent2>=100)
  // percent2=100;
  
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.setCursor(0,3);
    lcd.print("SH1:");
    if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
  
    lcd.print(percent1);
    lcd.print("%");
    if(percent1<100)
      lcd.print(" ");
  if(percent2<10)
      lcd.print(" ");
  
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
        lcd.setCursor(12,3);

    lcd.print("SH2:");
    if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
    lcd.print(percent2);
    lcd.print("%");

if(percent2<100)
      lcd.print(" ");
if(percent2<10)
      lcd.print(" ");
  
  if(digitalRead(lcd_button) == HIGH)
    {
        delay(200);
        nb=2;
    lcd.clear();
        show_tank_level();
    }
}

void show_date_time_temp_hum()
{
    show_date_time();
    show_temprature();
    show_humidity();
  show_percent_soil_sensors();
    // delay(1500);
}

void show_date_time_temp_hum_or_water_level()
{
    if(nb==1)
        show_date_time_temp_hum();
    else
    {
        if(nb==2)
            show_tank_level();
    }
}

void button_pushed()
{
    if(digitalRead(lcd_button) == HIGH && nb==1)
    {
        delay(200);
        nb=2;
        lcd.clear();
        show_tank_level();
    }
    else
    {
        if(digitalRead(lcd_button) == HIGH && nb==2)
        {
            delay(200);
            nb=1;
            lcd.clear();
            show_date_time_temp_hum();
        }
    else
    {
      if(analogRead(sink_but)<740)
      {
       // Serial.println(analogRead(sink_but));
         permission_d==1;
        digitalWrite(pump_grey_water1,HIGH);
        digitalWrite(pump_grey_water2,HIGH);
        lev_d=get_level_drinking_water();
   
        delay(500);
      }
    }
    }
}

void watering_plant()
{
  sensorValue1 = analogRead(sensorPin1);
  percent1 = convertToPercent(sensorValue1);
button_pushed();
  if(percent1>=100)
  percent1=100;
button_pushed();
  sensorValue2 = analogRead(sensorPin2);
  percent2 = convertToPercent(sensorValue2);
  if(percent2>=100)
  percent2=100;

  button_pushed();
  if(percent1<=50 && lev_g >= 0.2)
  {
    if(nb==1)
    show_percent_soil_sensors();

        digitalWrite(pump_grey_water1,LOW);
button_pushed();
    while(percent1<=50)
    {
    sensorValue1 = analogRead(sensorPin1);
    percent1 = convertToPercent(sensorValue1);
    //delay(200);
    }
        digitalWrite(pump_grey_water1,HIGH);
    delay(500);
  if(nb==1)
    show_percent_soil_sensors();
button_pushed();
    permission_g=1;
  lev_g=get_level_grey_water();
delay(200);
  }
  // else
  // {
    // digitalWrite(pump_grey_water1,HIGH);
  // }
  button_pushed();
  if(percent2<=50 && lev_g >= 0.2)
  {
      if(nb==1)
    show_percent_soil_sensors();
        digitalWrite(pump_grey_water2,LOW);
button_pushed();
      while(percent2<=50)
    {
    sensorValue2 = analogRead(sensorPin2);
    percent2 = convertToPercent(sensorValue2);
    //delay(200);
    }
        digitalWrite(pump_grey_water2,HIGH);
    delay(500);
button_pushed();
  if(nb==1)
    show_percent_soil_sensors();

    permission_g=1;
  lev_g=get_level_grey_water();
delay(200);
  }
  button_pushed();
}

void management_of_water_system()
{

    if(permission_b==1)
        lev_b=get_level_Primary_supply_water();

    delay(500);

    if(permission_g==1)
        lev_g=get_level_grey_water();

    if(permission_d==1)
        lev_d=get_level_drinking_water();
      delay(500);

    button_pushed(); watering_plant();

    digitalWrite(electrovalve1,LOW);
    button_pushed(); //watering_plant();

  // lev_b=1;
  // lev_d=1;
  // lev_g=1;
  
  button_pushed(); //watering_plant();

    if(lev_b <= 0.1 && (lev_g <= 1.29 || lev_d <= 1.29))
    {
        permission_b=0;
        digitalWrite(electrovalve1,LOW);
        //Serial.println("Primary_supply_water tank is empty !!");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Primary supply water");
        lcd.setCursor(0,1);
        lcd.print("  tank is empty !!  ");
        lcd.setCursor(0,2);
        lcd.print("   Put some water   ");
        lcd.setCursor(0,3);
        lcd.print("      Please !!     ");

        while(permission_b==0)
        {
            if(digitalRead(lcd_button) == HIGH)
            {
                delay(200);
                permission_b=1;
            }
        }

        lcd.clear();

    }
    else
    {
        if(lev_b >= 0.1 && lev_g <= 1.29 && lev_d <= 1.29)// first case
        {
            //Serial.println("Drinking water tank is not full yet");//np
            //Serial.println("Grey water tank is not full yet");//np
            digitalWrite(electrovalve1,LOW);
            button_pushed(); //watering_plant();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" Drinking and grey  ");
            lcd.setCursor(0,1);
            lcd.print("water tank are both");
            lcd.setCursor(0,2);
            lcd.print("    not full yet    ");
            lcd.setCursor(0,3);
            lcd.print("                    ");
            button_pushed(); //watering_plant();
            delay(4000);
            button_pushed(); //watering_plant();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("   Filling of the   ");
            lcd.setCursor(0,1);
            lcd.print("  drinking and grey ");
            lcd.setCursor(0,2);
            lcd.print("water tank on going ");
            lcd.setCursor(0,3);
            lcd.print("                    ");
            button_pushed(); //watering_plant();
            delay(4000);
            button_pushed(); //watering_plant();

            lcd.clear();
            show_date_time_temp_hum_or_water_level();
            button_pushed(); //watering_plant();

            digitalWrite(electrovalve1,LOW);
            button_pushed(); //watering_plant();

            digitalWrite(pump_24v,LOW);//faire fonctionner
            digitalWrite(electrovalve1,HIGH);
            digitalWrite(electrovalve2,LOW);
            digitalWrite(electrovalve3,LOW);
            digitalWrite(electrovalve4,HIGH);
            digitalWrite(electrovalve5,LOW);

            while(lev_b >= 0.1 && lev_g <= 1.29 && lev_d <= 1.29)
            {
                button_pushed(); watering_plant();

                if(cmpt==40)
                {
                    button_pushed(); //watering_plant();

                    if(permission_b==1)
                    {
                        lev_b_2=get_level_Primary_supply_water();
                        //button_pushed(); //watering_plant();
                        if(lev_b > lev_b_2)
                            lev_b=lev_b_2;
                    }
                    //button_pushed(); //watering_plant();

                    // delay(500);

                    if(permission_g==1)
                    {
                        lev_g_2=get_level_grey_water();
                        //button_pushed(); //watering_plant();
                        if(lev_g<lev_g_2)
                            lev_g=lev_g_2;
                    }

                    button_pushed(); //watering_plant();

                    if(permission_d==1)
                    {
                        lev_d_2=get_level_drinking_water();
                        //button_pushed(); //watering_plant();
                        if(lev_d<lev_d_2)
                            lev_d=lev_d_2;
                    }

                    //button_pushed(); //watering_plant();

                    // delay(500);

                    cmpt=0;
                    show_date_time_temp_hum_or_water_level();
                }
                cmpt++;
        
        // lev_b=1;
        // lev_d=1;
        // lev_g=1;
            }
            button_pushed(); //watering_plant();

            digitalWrite(pump_24v,HIGH);//eteindre
            digitalWrite(electrovalve1,LOW);
            digitalWrite(electrovalve2,HIGH);
            digitalWrite(electrovalve3,HIGH);
            digitalWrite(electrovalve4,HIGH);
            digitalWrite(electrovalve5,HIGH);
      delay(1000);
            button_pushed(); //watering_plant();

            if(lev_b<=0.1)
                permission_b=0;

            if(lev_g>=1.29)
                permission_g=0;

            if(lev_d>=1.29)
                permission_d=0;
      
        if(lev_g >= 1.29 && lev_d >= 1.29)
              nb_fois == 1;
        }
        else
        {
            if(lev_b >= 0.1 && lev_g <= 1.29 && lev_d >= 1.29)// second case
            {
                //Serial.println("Drinking water tank is full !!");//p
                //Serial.println("Grey water tank is not full yet !!");//np
                button_pushed(); //watering_plant();

                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("   Drinking water   ");
                lcd.setCursor(0,1);
                lcd.print("    tank is full    ");
                lcd.setCursor(0,2);
                lcd.print("     Grey water     ");
                lcd.setCursor(0,3);
                lcd.print("tank is not full yet");
                button_pushed(); //watering_plant();

                delay(4000);
                button_pushed(); //watering_plant();

                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("   Filling of the   ");
                lcd.setCursor(0,1);
                lcd.print("     grey water     ");
                lcd.setCursor(0,2);
                lcd.print("   tank on going    ");
                lcd.setCursor(0,3);
                lcd.print("                    ");
                button_pushed(); //watering_plant();

                delay(4000);
                button_pushed(); //watering_plant();

                lcd.clear();
                show_date_time_temp_hum_or_water_level();
                button_pushed(); //watering_plant();

                digitalWrite(pump_24v,LOW);//faire fonctionner
                digitalWrite(electrovalve1,HIGH);
                digitalWrite(electrovalve2,HIGH);
                digitalWrite(electrovalve3,LOW);
                digitalWrite(electrovalve4,LOW);
                digitalWrite(electrovalve5,LOW);

                while(lev_b >= 0.1 && lev_g <= 1.29 && lev_d >= 1.29)
                {
                    button_pushed(); watering_plant();
                    show_date_time_temp_hum_or_water_level();

                    if(cmpt==40)
                    {
                        button_pushed(); watering_plant();

                        if(permission_b==1)
                        {
                            lev_b_2=get_level_Primary_supply_water();
                            //button_pushed(); watering_plant();
                            if(lev_b > lev_b_2)
                                lev_b=lev_b_2;
                        }
                        //button_pushed(); watering_plant();

                        // delay(500);
 
                        if(permission_g==1)
                        {
                            lev_g_2=get_level_grey_water();
                            //button_pushed(); watering_plant();
                            if(lev_g<lev_g_2)
                                lev_g=lev_g_2;
                        }

                        button_pushed(); watering_plant();

                        if(permission_d==1)
                        {
                            lev_d_2=get_level_drinking_water();
                            button_pushed(); watering_plant();
                            if(lev_d<lev_d_2)
                                lev_d=lev_d_2;
                        }

                        button_pushed(); //watering_plant();

                        // delay(500);
                        //button_pushed(); watering_plant();

                        cmpt=0;
                        show_date_time_temp_hum_or_water_level();
                    }
                    cmpt++;
                    //button_pushed(); watering_plant();

                }
                button_pushed(); //watering_plant();

                digitalWrite(pump_24v,HIGH);//eteindre
                digitalWrite(electrovalve1,LOW);
                digitalWrite(electrovalve2,HIGH);
                digitalWrite(electrovalve3,HIGH);
                digitalWrite(electrovalve4,HIGH);
                digitalWrite(electrovalve5,HIGH);
        delay(1000);
                button_pushed(); //watering_plant();

                if(lev_b<=0.1)
                    permission_b=0;

                if(lev_g>=1.29)
                    permission_g=0;

                if(lev_d>=1.29)
                    permission_d=0;

        if(lev_g >= 1.29 && lev_d >= 1.29)
        nb_fois == 1;
            }
            else
            {
                if(lev_b >= 0.1 && lev_g >= 1.29 && lev_d <= 1.29)// third case
                {
                    //Serial.println("Drinking water tank is not full yet !!");//np
                    //Serial.println("Grey water tank is full !!");//p
                    button_pushed(); //watering_plant();

                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("     Grey water     ");
                    lcd.setCursor(0,1);
                    lcd.print("   tank is full     ");
                    lcd.setCursor(0,2);
                    lcd.print("   Drinking water   ");
                    lcd.setCursor(0,3);
                    lcd.print("tank is not full yet");
                    button_pushed(); //watering_plant();

                    delay(4000);
                    button_pushed(); //watering_plant();

                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("   Filling of the   ");
                    lcd.setCursor(0,1);
                    lcd.print("   drinking water   ");
                    lcd.setCursor(0,2);
                    lcd.print("   tank on going    ");
                    lcd.setCursor(0,3);
                    lcd.print("                    ");
                    button_pushed(); //watering_plant();

                    delay(4000);
                    button_pushed(); //watering_plant();

                    lcd.clear();
                    show_date_time_temp_hum_or_water_level();
                    button_pushed(); //watering_plant();

                    digitalWrite(pump_24v,LOW);//faire fonctionner
                    digitalWrite(electrovalve1,LOW);
                    digitalWrite(electrovalve2,LOW);
                    digitalWrite(electrovalve3,LOW);
                    digitalWrite(electrovalve4,HIGH);
                    digitalWrite(electrovalve5,HIGH);

                    while(lev_b >= 0.1 && lev_g >= 1.29 && lev_d <= 1.29)
                    {
                        button_pushed(); watering_plant();
                        //show_date_time_temp_hum_or_water_level();
 
                        if(cmpt==40)
                        {
                            button_pushed(); //watering_plant();

                            if(permission_b==1)
                            {
                                lev_b_2=get_level_Primary_supply_water();
                                //button_pushed(); watering_plant();
                                if(lev_b > lev_b_2)
                                    lev_b=lev_b_2;
                            }
                            button_pushed(); watering_plant();

                            // delay(500);
                            //button_pushed(); watering_plant();

                            if(permission_g==1)
                            {
                                lev_g_2=get_level_grey_water();
                                //button_pushed(); watering_plant();
                                if(lev_g<lev_g_2)
                                    lev_g=lev_g_2;
                            }

                            button_pushed(); //watering_plant();

                            if(permission_d==1)
                            {
                                lev_d_2=get_level_drinking_water();
                                //button_pushed(); watering_plant();
                                if(lev_d<lev_d_2)
                                    lev_d=lev_d_2;
                            }

                            button_pushed(); watering_plant();

                            // delay(500);
                            //button_pushed(); watering_plant();

                            cmpt=0;
                            show_date_time_temp_hum_or_water_level();

                        }
                        cmpt++;
                        button_pushed(); watering_plant();

                    }
                    button_pushed(); //watering_plant();

                    digitalWrite(pump_24v,HIGH);//eteindre
                    digitalWrite(electrovalve1,HIGH);
                    digitalWrite(electrovalve2,HIGH);
                    digitalWrite(electrovalve3,HIGH);
                    digitalWrite(electrovalve4,HIGH);
                    digitalWrite(electrovalve5,HIGH);
          delay(1000);
                    button_pushed(); //watering_plant();

                    if(lev_b<=0.1)
                        permission_b=0;

                    if(lev_g>=1.29)
                        permission_g=0;

                    if(lev_d>=1.29)
            permission_d=0;
        
        if(lev_g >= 1.29 && lev_d >= 1.29)
          nb_fois == 1;
          
                }
                else
                {
                    if(lev_g >= 1.29 && lev_d >= 1.29 && nb_fois == 0)// fourth case
                    {
                        //Serial.println("Drinking water tank is full !!");//p
                        //Serial.println("Grey water tank is full !!");//p
                        button_pushed(); watering_plant();

                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("  Grey and Drinking ");
                        lcd.setCursor(0,1);
                        lcd.print("   water tank are   ");
                        lcd.setCursor(0,2);
                        lcd.print("     both full      ");
                        lcd.setCursor(0,3);
                        lcd.print("                    ");
                        button_pushed(); watering_plant();

                        delay(4000);
                        button_pushed(); watering_plant();

                        lcd.clear();
                        //button_pushed(); watering_plant();

                        show_date_time_temp_hum_or_water_level();
                        //button_pushed(); watering_plant();

                        digitalWrite(pump_24v,HIGH);//eteindre
                        digitalWrite(electrovalve1,HIGH);
                        digitalWrite(electrovalve2,HIGH);
                        digitalWrite(electrovalve3,HIGH);
                        digitalWrite(electrovalve4,HIGH);
                        digitalWrite(electrovalve5,HIGH);
                        delay(1000);
                        //button_pushed(); watering_plant();
                        show_date_time_temp_hum_or_water_level();
                        nb_fois=1;
                    }
                }
            }
        }
    }
}

void loop()
{   
digitalWrite(electrovalve1,LOW);
management_of_water_system();
show_date_time_temp_hum_or_water_level();
button_pushed(); //watering_plant();
delay(1000);   
 
}
