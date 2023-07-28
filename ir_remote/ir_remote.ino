#include <IRremote.h>
int RECV_PIN = 11;
int LED1 = 2;
int LED2 = 3;
int LED3 = 4;
int LED4 = 5;
int LED5 = 6;
int LED6 = 7;
long on1  = 0xE619FF00;
//long on1  = 0x00FF6897;
long off1 = 0x00FF9867;
long on2 = 0x00FFB04F;
long off2 = 0x00FF19E6;
long on3 = 0x00FF18E7;
long off3 = 0x00FF7A85;
long on4 = 0x00FF10EF;
long off4 = 0x00FF38C7;
long on5 = 0x00FF5AA5;
long off5 = 0x00FF42BD;
long on6 = 0x00FF4AB5;
long off6 = 0x00FF52AD;
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
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);  
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); 
  //IrReceiver.enableIRIn(); // Start the receiver
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
//       digitalWrite(8, on ? HIGH : LOW);
       digitalWrite(13, on ? HIGH : LOW);
       //dump();
      }
    // Serial.println(IrReceiver.decodedIRData.decodedRawData);
    long int decoded_result = IrReceiver.decodedIRData.decodedRawData;

    if (decoded_result == on1 )
       digitalWrite(LED1, HIGH);
    if (decoded_result == off1 )
       digitalWrite(LED1, LOW); 
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