//RH sensor
#include "Adafruit_SHT4x.h"
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

//MPXV sensors
float insp_pres_raw, prox_pres_raw, exp_pres_raw;
float insp_pres_pin = A0;
float prox_pres_pin = A1;
float exp_pres_pin = A3;
float insp_pres, prox_pres, exp_pres;
float x, y;

//SDP sensor
#include <Wire.h>

//Valves' relays
// pin5 = 2/2 valve; proximal flushing
// pin6 = 3/2 valve; expiration valve
// pin7 = 2/2 valve; expiratory flow sensor flushing

const int pin5 = 5; // proximal flushing
const int pin6 = 6; // expiration valve
const int pin7 = 7; // expiratory flow sensor flushing

String S,S2,S3,S4,SubS,S_no_of_cycles,S_t_insp,S_t_exp,S_t_apnea;

void setup()
{
  Serial.begin(115200);

  //RH sensor
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

  //SDP sensor
  Wire.begin();
  Wire.beginTransmission(0x25); //Start communication over i2c on channel 25 (page 6 in cut sheet)
  Wire.write(0x36); Wire.write(0x03); //Start sensor in mode "Averate till read, Mass flow", use 3615 for DP (see page 7 in cutsheet)
  Wire.endTransmission();
  delay(8); //First measurement is available after 8ms, page 7 in cut sheet

  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  
  Serial.println("Enter the values of these parameters in respective order, separated by commas: no_of_cycles,t_insp,t_exp,t_apnea");
  Serial.println("Note 1: Parameter units: no_of_cycles(-),t_insp(ms),t_exp(ms),t_apnea(ms)");
  Serial.println("Note 2: The parameter values must be integers");
  Serial.println("Note 3: Space or period must not be used anywhere while entering the parameter values");
}

void loop() 
{
  //RH sensor
  sensors_event_t humidity, temp;
  
  while (Serial.available())
  {
    S = Serial.readStringUntil('\n');
  }

  while(S.length() > 0)
  {
    Serial.println(S);

    int pos_comma;
    
    pos_comma = S.indexOf(',');
    S_no_of_cycles = S.substring(0,pos_comma);
    S2 = S.substring(pos_comma + 1);
    
    pos_comma = S2.indexOf(',');
    S_t_insp = S2.substring(0,pos_comma);
    S3 = S2.substring(pos_comma + 1);
    
    pos_comma = S3.indexOf(',');
    S_t_exp = S3.substring(0,pos_comma);
    S4 = S3.substring(pos_comma + 1);
    
    S_t_apnea = S4;

    int no_of_cycles,t_insp,t_exp,t_apnea;  
  
    char C_no_of_cycles[10];
    S_no_of_cycles.toCharArray(C_no_of_cycles,S_no_of_cycles.length() + 1); 
    no_of_cycles = atoi(C_no_of_cycles);
  
    char C_t_insp[10];
    S_t_insp.toCharArray(C_t_insp,S_t_insp.length() + 1); 
    t_insp = atoi(C_t_insp);
  
    char C_t_exp[10];
    S_t_exp.toCharArray(C_t_exp,S_t_exp.length() + 1); 
    t_exp = atoi(C_t_exp);
  
    char C_t_apnea[10];
    S_t_apnea.toCharArray(C_t_apnea,S_t_apnea.length() + 1); 
    t_apnea = atoi(C_t_apnea);
    
    Serial.print("no_of_cycles: "); 
    Serial.println(no_of_cycles);
    Serial.print("t_insp: ");
    Serial.print(t_insp);
    Serial.println(" ms");
    Serial.print("t_exp: "); 
    Serial.print(t_exp);
    Serial.println(" ms");
    Serial.print("t_apnea: "); 
    Serial.print(t_apnea);
    Serial.println(" ms");
    
    int m = 1;  
   
    while(m <= no_of_cycles)
    {       
        Serial.print("Cycle no. = "); Serial.println(m);
  
        //Apnea phase
        Serial.println("Apnea phase");
  
// pin5 = 2/2 valve; proximal flushing
// pin6 = 3/2 valve; expiration valve
// pin7 = 2/2 valve; expiratory flow sensor flushing
        
        digitalWrite(pin5, HIGH);
        digitalWrite(pin6, LOW);
        digitalWrite(pin7, HIGH);
                
        uint32_t t_current = 0; //Current timestamp
        uint32_t t_elapsed = 0; //Time elapsed in a phase

        while(t_elapsed <= t_apnea)
        {
          t_current = millis();
          /*
          //Inspiratory MPXV sensor
          insp_pres_raw = analogRead(insp_pres_pin); //insp_pres_pin = A3
          x = 5 * insp_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          insp_pres = 10.197 * y - 0.33 - 0.11 - 3.10 - 0.05;
          */
          //Proximal MPXV sensor
          prox_pres_raw = analogRead(prox_pres_pin); //prox_pres_pin = A4
          x = 5 * prox_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          prox_pres = 10.197 * y - 0.33 + 1 - 0.67 - 0.11 - 3.10 - 0.05 + 2.16;
          /*
          //Expiratory MPXV sensor
          exp_pres_raw = analogRead(exp_pres_pin); //exp_pres_pin = A5
          x = 5 * exp_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          exp_pres = 10.197 * y - 0.33 - 3.10 - 0.05;
          */
          //RH sensor        
          sht4.getEvent(&humidity, &temp);
          
          //Serial.print("Insp_pres = "); Serial.print(insp_pres); Serial.print(" cmH2O | ");        
          Serial.print("Prox_pres = "); Serial.print(prox_pres); Serial.print(" cmH2O | ");
          //Serial.print("Exp_pres = "); Serial.print(exp_pres); Serial.println(" cmH2O");
          Serial.print("Temp = "); Serial.print(temp.temperature); Serial.print(" degC | ");
          Serial.print("RH = "); Serial.print(humidity.relative_humidity); Serial.println(" %");          

          delay(500);

          t_current = millis() - t_current; //Time taken to execute the current while loop
          t_elapsed += t_current; //Time elapsed in the inspiratory phase      
        }        
        /*
        //RH sensor        
        sht4.getEvent(&humidity, &temp);
        
        Serial.print("Temp = "); Serial.print(temp.temperature); Serial.print(" degC | ");
        Serial.print("RH = "); Serial.print(humidity.relative_humidity); Serial.println(" %");
        */
        t_current = 0;
        t_elapsed = 0;
       
        //Inspiration phase
        Serial.println("Inspiration phase");
  
// pin5 = 2/2 valve; proximal flushing
// pin6 = 3/2 valve; expiration valve
// pin7 = 2/2 valve; expiratory flow sensor flushing
        
        digitalWrite(pin5, HIGH);
        digitalWrite(pin6, HIGH);
        digitalWrite(pin7, LOW);
        delay(t_insp);
/*        
        uint32_t t_current = 0; //Current timestamp
        uint32_t t_elapsed = 0; //Time elapsed in a phase

        while(t_elapsed <= t_insp)
        {
          t_current = millis();

          //Inspiratory MPXV sensor
          insp_pres_raw = analogRead(insp_pres_pin); //insp_pres_pin = A3
          x = 5 * insp_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          insp_pres = 10.197 * y - 0.33 - 0.11;
          
          //Proximal MPXV sensor
          prox_pres_raw = analogRead(prox_pres_pin); //prox_pres_pin = A4
          x = 5 * prox_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          prox_pres = 10.197 * y - 0.33 + 1 - 0.67 - 0.11;
      
          //Expiratory MPXV sensor
          exp_pres_raw = analogRead(exp_pres_pin); //exp_pres_pin = A5
          x = 5 * exp_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          exp_pres = 10.197 * y - 0.33;
                  
          //Read pressure from SDP810
          int16_t dP_Raw; //16 bit intiger to store the msb and lsb
          double dP; //Double precision float to store actual pressure reading
          byte msb;
          byte lsb;
          Wire.requestFrom(0x25, 5); //Contents of first two bytes
          msb = Wire.read(); //Byte1 is msb
          lsb = Wire.read(); //Byte2 is lsb
          byte temp1 = Wire.read(); byte temp2 = Wire.read(); byte temp3 = Wire.read();
          dP_Raw = msb<<8; //Assign msb to combined variable
          dP_Raw |= lsb; //Add the lsb to the combined variable
          dP = dP_Raw/float(60) + 0.02; //Divide the raw reading by the scaling factor (page 10 in cut sheet)

          Serial.print("Insp_pres = "); Serial.print(insp_pres); Serial.print(" cmH2O | ");        
          Serial.print("Prox_pres = "); Serial.print(prox_pres); Serial.print(" cmH2O | ");
          Serial.print("Exp_pres = "); Serial.print(exp_pres); Serial.print(" cmH2O | ");
          Serial.print("dP_exp = "); Serial.print(dP); Serial.println(" Pa"); 

          delay(10);                     
          
          t_current = millis() - t_current; //Time taken to execute the current while loop
          t_elapsed += t_current; //Time elapsed in the inspiratory phase      
        }
*/       
        /* 
        //Inspiratory MPXV sensor
        insp_pres_raw = analogRead(insp_pres_pin); //insp_pres_pin = A3
        x = 5 * insp_pres_raw / 1023;
        y = (x / (0.09 * 5)) - (0.171065 / 0.425);
        insp_pres = 10.197 * y - 0.33 - 0.11 - 3.10 - 0.05;
        */
        //Proximal MPXV sensor
        prox_pres_raw = analogRead(prox_pres_pin); //prox_pres_pin = A4
        x = 5 * prox_pres_raw / 1023;
        y = (x / (0.09 * 5)) - (0.171065 / 0.425);
        prox_pres = 10.197 * y - 0.33 + 1 - 0.67 - 0.11 - 3.10 - 0.05 + 2.16;
        /*
        //Expiratory MPXV sensor
        exp_pres_raw = analogRead(exp_pres_pin); //exp_pres_pin = A5
        x = 5 * exp_pres_raw / 1023;
        y = (x / (0.09 * 5)) - (0.171065 / 0.425);
        exp_pres = 10.197 * y - 0.33 - 3.10 - 0.05;
        */
        //RH sensor
        sht4.getEvent(&humidity, &temp);
        
        //Serial.print("Insp_pres = "); Serial.print(insp_pres); Serial.print(" cmH2O | ");        
        Serial.print("Prox_pres = "); Serial.print(prox_pres); Serial.print(" cmH2O | ");
        //Serial.print("Exp_pres = "); Serial.print(exp_pres); Serial.println(" cmH2O");
        Serial.print("Temp = "); Serial.print(temp.temperature); Serial.print(" degC | ");
        Serial.print("RH = "); Serial.print(humidity.relative_humidity); Serial.println(" %");

        //t_current = 0;
        //t_elapsed = 0;
        
        //Expiration phase
        Serial.println("Expiration phase");
  
// pin5 = 2/2 valve; proximal flushing
// pin6 = 3/2 valve; expiration valve
// pin7 = 2/2 valve; expiratory flow sensor flushing
        
        digitalWrite(pin5, HIGH);
        digitalWrite(pin6, LOW);
        digitalWrite(pin7, LOW);
        delay(t_exp);
/*
        while(t_elapsed <= t_exp)
        {
          t_current = millis();

          //Inspiratory MPXV sensor
          insp_pres_raw = analogRead(insp_pres_pin); //insp_pres_pin = A3
          x = 5 * insp_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          insp_pres = 10.197 * y - 0.33 - 0.11;
          
          //Proximal MPXV sensor
          prox_pres_raw = analogRead(prox_pres_pin); //prox_pres_pin = A4
          x = 5 * prox_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          prox_pres = 10.197 * y - 0.33 + 1 - 0.67 - 0.11;
      
          //Expiratory MPXV sensor
          exp_pres_raw = analogRead(exp_pres_pin); //exp_pres_pin = A5
          x = 5 * exp_pres_raw / 1023;
          y = (x / (0.09 * 5)) - (0.171065 / 0.425);
          exp_pres = 10.197 * y - 0.33;
                  
          //Read pressure from SDP810
          int16_t dP_Raw; //16 bit intiger to store the msb and lsb
          double dP; //Double precision float to store actual pressure reading
          byte msb;
          byte lsb;
          Wire.requestFrom(0x25, 5); //Contents of first two bytes
          msb = Wire.read(); //Byte1 is msb
          lsb = Wire.read(); //Byte2 is lsb
          byte temp1 = Wire.read(); byte temp2 = Wire.read(); byte temp3 = Wire.read();
          dP_Raw = msb<<8; //Assign msb to combined variable
          dP_Raw |= lsb; //Add the lsb to the combined variable
          dP = dP_Raw/float(60) + 0.02; //Divide the raw reading by the scaling factor (page 10 in cut sheet)

          Serial.print("Insp_pres = "); Serial.print(insp_pres); Serial.print(" cmH2O | ");        
          Serial.print("Prox_pres = "); Serial.print(prox_pres); Serial.print(" cmH2O | ");
          Serial.print("Exp_pres = "); Serial.print(exp_pres); Serial.print(" cmH2O | ");      
          Serial.print("dP_hold = "); Serial.print(dP); Serial.println(" Pa"); 

          delay(10);  
          
          t_current = millis() - t_current; //Time taken to execute the current while loop
          t_elapsed += t_current; //Time elapsed in the inspiratory phase      
        }        
*/
        /*
        //Inspiratory MPXV sensor
        insp_pres_raw = analogRead(insp_pres_pin); //insp_pres_pin = A3
        x = 5 * insp_pres_raw / 1023;
        y = (x / (0.09 * 5)) - (0.171065 / 0.425);
        insp_pres = 10.197 * y - 0.33 - 0.11 - 3.10 - 0.05;
        */
        //Proximal MPXV sensor
        prox_pres_raw = analogRead(prox_pres_pin); //prox_pres_pin = A4
        x = 5 * prox_pres_raw / 1023;
        y = (x / (0.09 * 5)) - (0.171065 / 0.425);
        prox_pres = 10.197 * y - 0.33 + 1 - 0.67 - 0.11 - 3.10 - 0.05 + 2.16;
        /*
        //Expiratory MPXV sensor
        exp_pres_raw = analogRead(exp_pres_pin); //exp_pres_pin = A5
        x = 5 * exp_pres_raw / 1023;
        y = (x / (0.09 * 5)) - (0.171065 / 0.425);
        exp_pres = 10.197 * y - 0.33 - 3.10 - 0.05;
        */ 
        //RH sensor       
        sht4.getEvent(&humidity, &temp);
        
        //Serial.print("Insp_pres = "); Serial.print(insp_pres); Serial.print(" cmH2O | ");        
        Serial.print("Prox_pres = "); Serial.print(prox_pres); Serial.print(" cmH2O | ");
        //Serial.print("Exp_pres = "); Serial.print(exp_pres); Serial.println(" cmH2O");
        Serial.print("Temp = "); Serial.print(temp.temperature); Serial.print(" degC | ");
        Serial.print("RH = "); Serial.print(humidity.relative_humidity); Serial.println(" %");

        //t_current = 0;
        //t_elapsed = 0;
        
        m += 1;
    }

// pin5 = 2/2 valve; proximal flushing
// pin6 = 3/2 valve; expiration valve
// pin7 = 2/2 valve; expiratory flow sensor flushing
    
      digitalWrite(pin5, LOW);
      digitalWrite(pin6, LOW);
      digitalWrite(pin7, LOW);
    
    S = "";    
  }
}
