/**
 * Please download app BlueDuino from Apple App Store to play the sketch.
 */

#define TOTAL_PINS 22

#define PIN_CAPABILITY_NONE      0x00
#define PIN_CAPABILITY_DIGITAL   0x01
#define PIN_CAPABILITY_ANALOG    0x02
#define PIN_CAPABILITY_PWM       0x04
#define PIN_CAPABILITY_SERVO     0x08
#define PIN_CAPABILITY_I2C       0x10

#define PIN_MODE_INPUT            0x00
#define PIN_MODE_OUTPUT           0x01
#define PIN_MODE_ANALOG           0x02
#define PIN_MODE_PWM              0x03

#define PIN_STATE_HIGH            0x01
#define PIN_STATE_LOW            0x00

#define WAIT_SECONDS          3

#define IS_PIN_DIGITAL(p)       (((p) >= 0 && (p) <= 10) || ((p) >= 14 && (p) < TOTAL_PINS && (p) != 17 ))
#define IS_PIN_ANALOG(p)        ((p) == 4 || (p) == 6 || ((p) >= 8 && (p) <= 10) || ((p) >= 18 && (p) < TOTAL_PINS))
#define IS_PIN_PWM(p)           ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 )
#define IS_PIN_SERVO(p)         (IS_PIN_DIGITAL(p) && (p) - 2 < MAX_SERVOS)
#define IS_PIN_I2C(p)           ((p) == 2 || (p) == 3)
#define PIN_TO_DIGITAL(p)       (p)
#define PIN_TO_ANALOG(p)        ((p) >= 18 ? (p) - 18:((p) >= 8 ? (p):((p) == 6 ? 7 :6)))
#define PIN_TO_PWM(p)           PIN_TO_DIGITAL(p)

byte pin_mode[TOTAL_PINS];
byte pin_state[TOTAL_PINS];
byte pin_pwm[TOTAL_PINS];
byte pin_servo[TOTAL_PINS];

void setup()
{
  Serial1.begin(9600);
  
   for (int pin = 0; pin < TOTAL_PINS; pin++)
    {
        // Set pin to input with internal pull up
        if (IS_PIN_DIGITAL(pin)) {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, HIGH);
        }
        
        // Save pin mode and state
        pin_mode[pin] = PIN_MODE_OUTPUT;
        pin_state[pin] = HIGH;
    }
}

byte reportDigitalInput()
{
    static byte pin = 0;
    byte report = 0;
    
    if (!IS_PIN_DIGITAL(pin))
    {
        pin++;
        if (pin >= TOTAL_PINS)
            pin = 0;
        return 0;
    }
    
    if (pin_mode[pin] == PIN_MODE_INPUT)
    {
        byte current_state = digitalRead(pin);
        
        if (pin_state[pin] != current_state)
        {
            pin_state[pin] = current_state;
            byte buf[] = {'G', pin, pin_mode[pin], current_state};
            Serial1.write(buf, 4);
            
            report = 1;
        }
    }
    
    pin++;
    if (pin >= TOTAL_PINS)
        pin = 0;
    
    return report;
}


void reportPinCapability(byte pin)
{
    byte buf[] = {'P', pin, 0x00};
    byte pin_cap = 0;
    
    if (IS_PIN_DIGITAL(pin))
        pin_cap |= PIN_CAPABILITY_DIGITAL;
    
    if (IS_PIN_ANALOG(pin))
        pin_cap |= PIN_CAPABILITY_ANALOG;
    
    if (IS_PIN_PWM(pin))
        pin_cap |= PIN_CAPABILITY_PWM;
    
//    if (IS_PIN_SERVO(pin))
//        pin_cap |= PIN_CAPABILITY_SERVO;
    
    buf[2] = pin_cap;
    Serial1.write(buf, 3);
}
void reportPinDigitalData(byte pin)
{
    byte state = digitalRead(pin);
    byte mode = pin_mode[pin];
    byte buf[] = {'G', pin, mode, state};
    Serial1.write(buf, 4);
}

void reportPinPWMData(byte pin)
{
    byte value = pin_pwm[pin];
    byte mode = pin_mode[pin];
    byte buf[] = {'G', pin, mode, value};
    Serial1.write(buf, 4);
}

byte reportPinAnalogData()
{
    static byte pin = 0;
    byte report = 0;
    
    if (!IS_PIN_DIGITAL(pin))
    {
        pin++;
        if (pin >= TOTAL_PINS)
            pin = 0;
        return 0;
    }
    
    if (pin_mode[pin] == PIN_MODE_ANALOG)
    {
        delay(WAIT_SECONDS);
        uint16_t value = analogRead(PIN_TO_ANALOG(pin));
        byte value_lo = value;
        byte value_hi = value>>8;
        
        byte mode = pin_mode[pin];
        mode = (value_hi << 4) | mode;
        
        byte buf[] = {'G', pin, mode, value_lo};
        Serial1.write(buf, 4);
    }
    
    pin++;
    if (pin >= TOTAL_PINS)
        pin = 0;
    
    return report;
}

void loop()
{
  while(Serial1.available() > 0)
  {
    byte cmd;
    cmd = Serial1.read();
    switch (cmd)
    {
      case 'C':
      {
        byte buf[2];
        buf[0] = 'C';
        buf[1] = TOTAL_PINS;
        Serial1.write(buf, 2);
      }
      break;
      case 'A':
      {
         for (int pin = 0; pin < TOTAL_PINS; pin++)
            {
                    if (!IS_PIN_DIGITAL(pin)) {
                      continue;
                    }
                    reportPinCapability(pin);
                    if ( (pin_mode[pin] == PIN_MODE_INPUT) || (pin_mode[pin] == PIN_MODE_OUTPUT) )
                    {
                        reportPinDigitalData(pin);
                    }
                    else if (pin_mode[pin] == PIN_MODE_PWM)
                    {
                        reportPinPWMData(pin);
                    }
                    else if (pin_mode[pin] == PIN_MODE_ANALOG)
                    {
                         reportPinDigitalData(pin);
                    }
             }
      }
      break;
      case 'T':
      {
        delay(WAIT_SECONDS);
        int pin = Serial1.read();
        delay(WAIT_SECONDS);
        int state = Serial1.read();
        
        if (state == PIN_STATE_HIGH) {
          digitalWrite(pin, HIGH);
          pin_state[pin] = HIGH;
        } else {
          digitalWrite(pin, LOW);
          pin_state[pin] = LOW;
        }
      }
      break;
      case 'N':
      {
        delay(WAIT_SECONDS);
        byte pin = Serial1.read();
        delay(WAIT_SECONDS);
        byte value = Serial1.read();
        analogWrite(pin, value);
        pin_pwm[pin] = value;
      }
      break;
      case 'S':
      {
        delay(WAIT_SECONDS);
        byte pin = Serial1.read();
        delay(WAIT_SECONDS);
        byte mode = Serial1.read();
       
        if (mode != pin_mode[pin])
        {
          pin_mode[pin] = mode;
          if (mode == PIN_MODE_OUTPUT)
          {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, HIGH);
            pin_state[pin] = HIGH;
            reportPinDigitalData(pin);
          }
          else if (mode == PIN_MODE_INPUT)
          {
             pinMode(pin, INPUT);
             reportPinDigitalData(pin);
          } 
          else if (mode == PIN_MODE_ANALOG && IS_PIN_ANALOG(pin))
          {
            pinMode(pin, INPUT);
            reportPinDigitalData(pin);
          }
          else if (mode == PIN_MODE_PWM && IS_PIN_PWM(pin))
          {
            pinMode(pin, OUTPUT);
            analogWrite(pin, 0);
            pin_pwm[pin] = 0;
            reportPinPWMData(pin);
          }
        }
      }
      break;
    }
    
  }
  
  reportPinAnalogData();
  
  byte input_data_pending = reportDigitalInput();
  if (input_data_pending) 
  {
    return;
  }

  
}



