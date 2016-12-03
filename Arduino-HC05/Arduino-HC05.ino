#define BUFSIZE 8

int BIN_1 = 3;
int BIN_2 = 5;
int AIN_1 = 6;
int AIN_2 = 9;

int MAX_PWM_VOLTAGE = 240;
int LOW_PWM_VOLTAGE = 35;

int speed = 80;
/**
 * min -255 max 255
 * D104 motore DX vel 104
 * 
 * D33
 */
// buffer di ricezione con spazio per il terminatore
char rxBuffer[BUFSIZE+1];
// numero di caratteri in rxBuffer
int numReceived = 0;

void setup() {
    pinMode(BIN_1, OUTPUT);
    pinMode(BIN_2, OUTPUT);
    pinMode(AIN_1, OUTPUT);
    pinMode(AIN_2, OUTPUT);
    Serial.begin(9600);
}

void loop() {

  checkCommands();
/*
    if(Serial.available())
    {
      int val = Serial.read();
 
      switch(val)
      {
        case '1':
          digitalWrite(AIN_2, LOW);
          analogWrite(AIN_1, MAX_PWM_VOLTAGE);
          Serial.write('F');
          break;
        case '3':
          analogWrite(AIN_2, MAX_PWM_VOLTAGE);
          digitalWrite(AIN_1, LOW);
          Serial.write('R');
          break;
        case '2':
          digitalWrite(AIN_2, LOW);
          digitalWrite(AIN_1, LOW);
          Serial.write('S');
          break;
      }
    }*/
}

// Controlla se ci sono caratteri in arrivo sulla seriale, se ci sono li aggiunge al buffer se ci stanno
// tranne CR nel qual caso cerca di eseguire il comando
void checkCommands()
{
  int val = Serial.read();
  if(val == '\n')
  {
    //return: cerca di eseguire il comando nel buffer
    rxBuffer[numReceived] = '\0';// termina la stringa
    Serial.println(rxBuffer);
    
    numReceived = 0;
    interpreta();
  }
  else if (val != -1)
  {
    // aggiungi al buffer
    if(numReceived < BUFSIZE)
    {
      rxBuffer[numReceived] = val;
      numReceived++;  
    }
  }
}
void interpreta()
{
  int arg = 0;
  switch(rxBuffer[0])
  {
    case 'd':
       arg = atoi(&rxBuffer[1]); // atoi legge fino a che non trova il terminatore \0
       Serial.println(arg);
       if(arg < -255)
       {
        arg == -255;
       }
       else if(arg > 255)
       {
        arg == 255;
       }

       if(arg < 0)
       {
         
          analogWrite(AIN_2, -arg);
          digitalWrite(AIN_1, LOW);  
       }
       else
       {
          analogWrite(AIN_1, arg);
          digitalWrite(AIN_2, LOW);
       }
       
      break;
      
    case 's':
      
      break;
      
    default:
      break;
  }
}


