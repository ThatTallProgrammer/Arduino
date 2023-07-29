// Pressing a number 1 - 6 selects the associated light. If it is off, turn it on.
// Pressing the OK button will toggle the selected light on and off
// Pressing the left/right buttons moves between lights in the direction of the arrow. 
// Pressing the up/down buttons will increase and decrease the brightness respectively

#include <IRremote.h>
int RECV_PIN = 12;
int pins[6] = {3, 5, 6, 9, 10, 11}; 
long control_codes[10] = {
  0xE916FF00, // LED 1
  0xE619FF00, // LED 2
  0xF20DFF00, // LED 3
  0xF30CFF00, // LED 4
  0xE718FF00, // LED 5
  0xA15EFF00, // LED 6
  0xBB44FF00, // left arrow
  0xBC43FF00, // right arrow
  0xB946FF00, // up arrow
  0xEA15FF00  // down arrow
};

int active_pin = 0;
bool light_on = false;
int brightness_levels[5] = {63, 127, 191, 255};
int brightness_level = 5;

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

// if code in control_codes array, return the index and return -1 otherwise
int in_control_codes(long code)
{
  for(int i = 0; i < 10; i++)
  {
    if(code == control_codes[i])
      return i;
  }
  return -1;
}

void turn_off_leds()
{
  for(int pin = 0; pin < 6; pin++)
  {
    digitalWrite(pins[pin], LOW);
  }
}

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
    int control_code_index = in_control_codes(decoded_result);
    
    if(control_code_index != -1)
    {
      // Serial.println(control_code_index);
      // light has been selected
      if(control_code_index >= 0 && control_code_index < 6)
      {
        active_pin = control_code_index;
        turn_off_leds();
        digitalWrite(pins[control_code_index], HIGH);
      }
      else if(control_code_index >= 6 && control_code_index < 8)
      {
        active_pin = control_code_index == 6 ? (active_pin + 1) % 6 : active_pin - 1;
        active_pin = active_pin < 0  ? 5 : active_pin;
        turn_off_leds();
        digitalWrite(pins[active_pin], HIGH);
      }
      else if(control_code_index >= 8 && control_code_index < 10)
      {
        brightness_level = control_code_index == 8 ? brightness_level + 1 : brightness_level - 1;
        
        // impose extrema
        if(brightness_level > 3)
          brightness_level = 3;
        else if(brightness_level < 0)
          brightness_level = 0;

        Serial.println(brightness_levels[brightness_level]);
      }
      else
      {
        Serial.println("Code not yet assigned");
      }

    }   

    last = millis();      
    IrReceiver.resume(); // Receive the next value
    // }
  }
}