// Pressing a number 1 - 6 selects the associated light. If it is off, turn it on.
// Pressing the same number again will turn off the light.
// Pressing the OK button will toggle the selected light on and off
// Pressing the left/right buttons moves between lights in the direction of the arrow. 
// Pressing the up/down buttons will increase and decrease the brightness respectively

#include <IRremote.h>
int RECV_PIN = 12;
int pins[6] = {3, 5, 6, 9, 10, 11}; 
// int LED1 = 3;
// int LED2 = 5;
// int LED3 = 6;
// int LED4 = 9;
// int LED5 = 10;
// int LED6 = 11;
long pin_0 = 0xE916FF00;
long pin_1 = 0xE619FF00;
long pin_2 = 0xF20DFF00;
long pin_3 = 0xF30CFF00;
long pin_4 = 0xE718FF00;
long pin_5 = 0xA15EFF00;
long current_selection = -1;
int active_pin = 0;
bool light_on = false;
//long on1  = 0x00FF6897;
// long off1 = 0x00FF9867;
// long on2 = 0x00FFB04F;
// long off2 = 0x00FF19E6;
// long on3 = 0x00FF18E7;
// long off3 = 0x00FF7A85;
// long on4 = 0x00FF10EF;
// long off4 = 0x00FF38C7;
// long on5 = 0x00FF5AA5;
// long off5 = 0x00FF42BD;
// long on6 = 0x00FF4AB5;
// long off6 = 0x00FF52AD;
void dump() 
{
  int count = IrReceiver.decodedIRData.rawDataPtr->rawlen;

  if (IrReceiver.decodedIRData.protocol == UNKNOWN) 
  {
    Serial.println("Could not decode message");
  } 
  else 
  {
    if (IrReceiver.decodedIRData.protocol == NEC) 
    {
      Serial.print("Decoded NEC: ");
    } 
    else if (IrReceiver.decodedIRData.protocol == SONY) 
    {
      Serial.print("Decoded SONY: ");
    } 
    else if (IrReceiver.decodedIRData.protocol == RC5) 
    {
      Serial.print("Decoded RC5: ");
    } 
    else if (IrReceiver.decodedIRData.protocol == RC6) 
    {
      Serial.print("Decoded RC6: ");
    }
    Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
    Serial.print(" (");
    Serial.print(IrReceiver.decodedIRData.numberOfBits, DEC);
    Serial.println(" bits)");
  }
  
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");
  
  for (int i = 0; i < count; i++) 
  {
    if ((i % 2) == 1) 
    {
      Serial.print(IrReceiver.decodedIRData.rawDataPtr->rawbuf[i]*USECPERTICK, DEC);
    } 
    else  
    {
      Serial.print(-(int)IrReceiver.decodedIRData.rawDataPtr->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}

void setup()
 {
  pinMode(RECV_PIN, INPUT);

  // activate all 6 pins in the pin array
  for(int pin = 0; pin < 6; pin++)
  {   
    pinMode(pins[pin], OUTPUT);
  }

  pinMode(13, OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); 
 }
int on = 0;
unsigned long last = millis();

void loop() 
{
  if (IrReceiver.decode()) 
  {
    // If it's been at least 1/4 second since the last
    // IR received, toggle the relay
    if (millis() - last > 250) 
    {
      on = !on;
      digitalWrite(13, on ? HIGH : LOW);
      dump();
    }
    // Serial.println(IrReceiver.decodedIRData.decodedRawData);
    long int decoded_result = IrReceiver.decodedIRData.decodedRawData;

    // if the code  is unknown, do nothing
    if(decoded_result == pin_0 || decoded_result == pin_1 || decoded_result == pin_2 || decoded_result == pin_3 || decoded_result == pin_4 || decoded_result != pin_5)
    {
      // decide what number was pressed
      if (decoded_result == current_selection)
        light_on = !light_on;
      else 
      {
        if (decoded_result == pin_0)
          active_pin = pins[0];
        else if (decoded_result == pin_1)
          active_pin = pins[1];
        else if (decoded_result == pin_2)
          active_pin = pins[2];
        else if (decoded_result == pin_3)
          active_pin = pins[3];
        else if (decoded_result == pin_4)
          active_pin = pins[4];
        else if (decoded_result == pin_5)
          active_pin = pins[5];

        for(int pin = 0; pin < 6; pin++)
          digitalWrite(pin, LOW);

        current_selection = decoded_result;
        light_on = true;
      }

      if(light_on)
        digitalWrite(active_pin, HIGH);
      else
        digitalWrite(active_pin, LOW);    
        //  digitalWrite(LED1, HIGH);
      // if (decoded_result == off1 )
      //    digitalWrite(LED1, LOW); 
      // if (IrReceiver.decodedIRData.decodedRawData == on2 )
      //    digitalWrite(LED2, HIGH);
      // if (IrReceiver.decodedIRData.decodedRawData == off2 )
      //    digitalWrite(LED2, LOW); 
      // if (IrReceiver.decodedIRData.decodedRawData == on3 )
      //    digitalWrite(LED3, HIGH);
      // if (IrReceiver.decodedIRData.decodedRawData == off3 )
      //    digitalWrite(LED3, LOW);
      // if (IrReceiver.decodedIRData.decodedRawData == on4 )
      //    digitalWrite(LED4, HIGH);
      // if (IrReceiver.decodedIRData.decodedRawData == off4 )
      //    digitalWrite(LED4, LOW); 
      // if (IrReceiver.decodedIRData.decodedRawData == on5 )
      //    digitalWrite(LED5, HIGH);
      // if (IrReceiver.decodedIRData.decodedRawData == off5 )
      //    digitalWrite(LED5, LOW); 
      // if (IrReceiver.decodedIRData.decodedRawData == on6 )
      //    digitalWrite(LED6, HIGH);
      // if (IrReceiver.decodedIRData.decodedRawData == off6 )
      //    digitalWrite(LED6, LOW);        
      last = millis();      
      IrReceiver.resume(); // Receive the next value
    }
  }
}