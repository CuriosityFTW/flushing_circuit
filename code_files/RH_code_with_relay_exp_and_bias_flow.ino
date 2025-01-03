#include "Adafruit_SHT4x.h"

//MPXV sensors
float prox_pres_raw, exp_pres_raw;
float prox_pres_pin = A4;
float exp_pres_pin = A5;
float prox_pres, exp_pres;
float x1, y1, x2, y2;

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

const int Rinse_valve = 2; // 2/2 rinse flow valve
const int Inspiration_valve = 3; // 3/2 humidifier flow valve
const int Expiration_valve = 4; // 3/2 exhalation valve

String S,S2,S3,S4,SubS,S_no_of_cycles,S_t_exp,S_t_hold,S_t_insp;

void setup()
{
  Serial.begin(115200);

  while (!Serial)
  delay(10);     //will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit SHT4x test");
  
  if (! sht4.begin()) 
  {
    Serial.println("Couldn't find SHT4x");
    while (1) 
    delay(1);
  }
  
  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);

  //You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  switch (sht4.getPrecision()) 
  {
     case SHT4X_HIGH_PRECISION: 
       Serial.println("High precision");
       break;
     case SHT4X_MED_PRECISION: 
       Serial.println("Med precision");
       break;
     case SHT4X_LOW_PRECISION: 
       Serial.println("Low precision");
       break;
  }

  // You can have 6 different heater settings
  // higher heat & longer times uses more power
  // & reads will take longer too!
  sht4.setHeater(SHT4X_NO_HEATER);
  switch (sht4.getHeater()) 
  {
     case SHT4X_NO_HEATER: 
       Serial.println("No heater");
       break;
     case SHT4X_HIGH_HEATER_1S: 
       Serial.println("High heat for 1 second");
       break;
     case SHT4X_HIGH_HEATER_100MS: 
       Serial.println("High heat for 0.1 second");
       break;
     case SHT4X_MED_HEATER_1S: 
       Serial.println("Medium heat for 1 second");
       break;
     case SHT4X_MED_HEATER_100MS: 
       Serial.println("Medium heat for 0.1 second");
       break;
     case SHT4X_LOW_HEATER_1S: 
       Serial.println("Low heat for 1 second");
       break;
     case SHT4X_LOW_HEATER_100MS: 
       Serial.println("Low heat for 0.1 second");
       break;
  }

  pinMode(Rinse_valve, OUTPUT);
  pinMode(Inspiration_valve, OUTPUT);
  pinMode(Expiration_valve, OUTPUT);
  
  Serial.println("Enter the values of these parameters in respective order, separated by commas: no_of_cycles,t_exp,t_hold,t_insp");
  Serial.println("Note 1: Parameter units: no_of_cycles(-),t_exp(ms),t_hold(ms),t_insp(ms)");
  Serial.println("Note 2: The parameter values must be integers");
  Serial.println("Note 3: Space or period must not be used anywhere while entering the parameter values");
}

void loop() 
{
  while (Serial.available())
  {
    S = Serial.readStringUntil('\n');
  }
  
  sensors_event_t humidity, temp;

  while(S.length() > 0)
  {
    Serial.println(S);

    int pos_comma;
    
    pos_comma = S.indexOf(',');
    S_no_of_cycles = S.substring(0,pos_comma);
    S2 = S.substring(pos_comma + 1);
    
    pos_comma = S2.indexOf(',');
    S_t_exp = S2.substring(0,pos_comma);
    S3 = S2.substring(pos_comma + 1);
    
    pos_comma = S3.indexOf(',');
    S_t_hold = S3.substring(0,pos_comma);
    S4 = S3.substring(pos_comma + 1);
    
    S_t_insp = S4;

    int no_of_cycles,t_exp,t_hold,t_insp;  
  
    char C_no_of_cycles[10];
    S_no_of_cycles.toCharArray(C_no_of_cycles,S_no_of_cycles.length() + 1); 
    no_of_cycles = atoi(C_no_of_cycles);
  
    char C_t_exp[10];
    S_t_exp.toCharArray(C_t_exp,S_t_exp.length() + 1); 
    t_exp = atoi(C_t_exp);
  
    char C_t_hold[10];
    S_t_hold.toCharArray(C_t_hold,S_t_hold.length() + 1); 
    t_hold = atoi(C_t_hold);
  
    char C_t_insp[10];
    S_t_insp.toCharArray(C_t_insp,S_t_insp.length() + 1); 
    t_insp = atoi(C_t_insp);
    
    Serial.print("no_of_cycles: "); 
    Serial.println(no_of_cycles);
    Serial.print("t_exp: ");
    Serial.print(t_exp);
    Serial.println(" ms");
    Serial.print("t_hold: "); 
    Serial.print(t_hold);
    Serial.println(" ms");
    Serial.print("t_insp: "); 
    Serial.print(t_insp);
    Serial.println(" ms");
    
    int m = 1;  
   
    while(m <= no_of_cycles)
    { 
      uint32_t timestamp = millis();
      sht4.getEvent(&humidity, &temp);
      timestamp = millis() - timestamp;

      Serial.print("Cycle no. = ");
      Serial.println(m);
      
      digitalWrite(Rinse_valve, LOW);
      digitalWrite(Inspiration_valve, LOW);
      digitalWrite(Expiration_valve, HIGH);
      delay(t_insp);
      
      //Proximal MPXV sensor
      prox_pres_raw = analogRead(prox_pres_pin); //prox_pres_pin = A4
      x1 = 5 * prox_pres_raw / 1023;
      y1 = (x1 / (0.09 * 5)) - (0.171065 / 0.425);
      prox_pres = 10.197 * y1 - 0.33;

      //Expiratory MPXV sensor
      exp_pres_raw = analogRead(exp_pres_pin); //exp_pres_pin = A5
      x2 = 5 * exp_pres_raw / 1023;
      y2 = (x2 / (0.09 * 5)) - (0.171065 / 0.425);
      exp_pres = 10.197 * y2 - 0.33;

      Serial.println("t_insp");
      Serial.print("Prox_pres = "); Serial.print(prox_pres); Serial.print(" cmH2O | ");
      Serial.print("Exp_pres = "); Serial.print(exp_pres); Serial.println(" cmH2O");
      Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degC");
      Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println(" %");
      
      digitalWrite(Inspiration_valve, HIGH);
      digitalWrite(Rinse_valve, HIGH);
      digitalWrite(Expiration_valve, LOW);
      delay(t_exp);

      //Proximal MPXV sensor
      prox_pres_raw = analogRead(prox_pres_pin); //prox_pres_pin = A4
      x1 = 5 * prox_pres_raw / 1023;
      y1 = (x1 / (0.09 * 5)) - (0.171065 / 0.425);
      prox_pres = 10.197 * y1 - 0.33;

      //Expiratory MPXV sensor
      exp_pres_raw = analogRead(exp_pres_pin); //exp_pres_pin = A5
      x2 = 5 * exp_pres_raw / 1023;
      y2 = (x2 / (0.09 * 5)) - (0.171065 / 0.425);
      exp_pres = 10.197 * y2 - 0.33;

      Serial.println("t_exp");
      Serial.print("Prox_pres = "); Serial.print(prox_pres); Serial.print(" cmH2O | ");
      Serial.print("Exp_pres = "); Serial.print(exp_pres); Serial.println(" cmH2O");      
      Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degC");
      Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println(" %");
      
      digitalWrite(Rinse_valve, HIGH);
      digitalWrite(Inspiration_valve, HIGH);
      digitalWrite(Expiration_valve, LOW);
      delay(t_hold);

      //Proximal MPXV sensor
      prox_pres_raw = analogRead(prox_pres_pin); //prox_pres_pin = A4
      x1 = 5 * prox_pres_raw / 1023;
      y1 = (x1 / (0.09 * 5)) - (0.171065 / 0.425);
      prox_pres = 10.197 * y1 - 0.33;

      //Expiratory MPXV sensor
      exp_pres_raw = analogRead(exp_pres_pin); //exp_pres_pin = A5
      x2 = 5 * exp_pres_raw / 1023;
      y2 = (x2 / (0.09 * 5)) - (0.171065 / 0.425);
      exp_pres = 10.197 * y2 - 0.33;

      Serial.println("t_hold");
      Serial.print("Prox_pres = "); Serial.print(prox_pres); Serial.print(" cmH2O | ");
      Serial.print("Exp_pres = "); Serial.print(exp_pres); Serial.println(" cmH2O");      
      Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degC");
      Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println(" %");

      m += 1;
    }
    digitalWrite(Rinse_valve, LOW);
    digitalWrite(Inspiration_valve, LOW);
    digitalWrite(Expiration_valve, LOW);
    
    S = "";    
  }
}
