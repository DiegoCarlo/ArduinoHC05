/*
 * Commando generico "S___,_E
 * da Data[1] a Data[3] viene emmesso il valore della velocita (?);
 * in Data[5] viene indicato lo stato del motore ( O = OFF ; 1 = ON);

*/
//#include <SoftwareSerial.h>;
#define DIM 7
// Variabili per ricezione
char A_Data[DIM];
char Ser_Inp_Byte;
int  Rx_Chr_Count;
int  Err_Flg;

int BIN_1 = 3;
int BIN_2 = 5;
int AIN_1 = 6;
int AIN_2 = 9;

int MAX_PWM_VOLTAGE = 240;
int LOW_PWM_VOLTAGE = 35;

char *Comma_Ptr;
int  Comma_Psz;
int Mot_Acc = 0;
int Mot_Vel = 0;

//SoftwareSerial Bluetooth_Serial(10,11);

void setup() 
{
   pinMode(BIN_1, OUTPUT); //motori di dx
   pinMode(BIN_2, OUTPUT);
  
   pinMode(AIN_1, OUTPUT);//motori di sx
   pinMode(AIN_2, OUTPUT);
 
   Serial.begin(9600);
}

void  RxToLx(int Vel)
{
   digitalWrite(BIN_2 , LOW);
   digitalWrite(AIN_1, LOW);
   analogWrite(BIN_1, Vel);
   analogWrite(AIN_2, Vel);
}
void LxToRx(int Vel)
{
   digitalWrite(BIN_1, LOW);
   digitalWrite(AIN_2, LOW);
   analogWrite(BIN_2, Vel);
   analogWrite(AIN_1, Vel);
}
void Stop()
{
   digitalWrite(BIN_1, LOW);
   digitalWrite(AIN_1, LOW);
   digitalWrite(BIN_2, LOW);
   digitalWrite(AIN_2, LOW);
}


void loop() 
{
  A_Data[0] = 0;
  Rx_Chr_Count = 0;
  Err_Flg = 0;
  //------------------------------------------       
  while(true)
  {
    delay(1000);
    // nel caso Serial non sia available
    if(Serial.available() <1)
    {
      // il ciclo passa all'iterazione successiva
      Serial.println("E- !Available");
      continue; 
    }

    // leggo i dati da seriale
    Ser_Inp_Byte = (char)Serial.read();

    // se il contatore di caratteri è uguale a 0
    if (Rx_Chr_Count == 0)
    {
      Serial.print("Contatore Char ");
      Serial.println(Rx_Chr_Count);
      // se il caraattere letto è 'S'
      if(Ser_Inp_Byte !=  'S' )
      {
        Serial.print("Ser_Inp_Byte ");
        Serial.println(Ser_Inp_Byte);// setto flag errore a 1
        Err_Flg = 1;
      }
    }
    // se il contatore di caratteri è diverso da 0
    else
    {
      // e il contatore di caratteri è maggiore della dimensione del comando
      if (Rx_Chr_Count > DIM)
      {
        Serial.print("Rx_Chr_Count ");
        Serial.println(Rx_Chr_Count);// setto flag errore a 1
        // setto la flag errore a 1
        Err_Flg = 1;
      }
    }
    // se si è verificato un errore
    if(Err_Flg)
    {
      Serial.println("Reset");// setto flag errore a 1
      // resetto i dati
      A_Data[0] = 0;
      Rx_Chr_Count = 0;
      Err_Flg = 0;
      // passo alla prossima iterazione
      continue;
    }
    // se il contatore è nell'intervallo [1 , 7] aggiungo ai dati il carattere letto
    A_Data[Rx_Chr_Count++] = Ser_Inp_Byte;
    Serial.print("A_Data ");
    Serial.println(A_Data);
    // e se tale carattere non è terminale 
    if(Ser_Inp_Byte != 'E')
    {
      Serial.print("Ser_Inp_Byte ");
      Serial.println(Ser_Inp_Byte);
      // ricomincio leggendo il successivo
      continue;
    }
    //--------------------------------------------------------------FINE Acquizione 
    // cerco la posizione della virgola
    Comma_Ptr = strchr(A_Data, ',');
      
    Comma_Psz = Comma_Ptr - A_Data;
    //--------------------------------------------------------------Ricerca ','
    
    Mot_Vel = atoi(A_Data +1 );
    Mot_Acc = atoi(Comma_Ptr+1);
    Serial.print("Mot_Vel ");
    Serial.println(Mot_Vel);
    Serial.print("Mot_Acc ");
    Serial.println(Mot_Acc);
      
  //--------------------------------------------------------------Elaborazione
   /*          
  *           Nel leggere il tuo prg ho provato ad interpretare la condizione in cui vuoi far lavorare i tuoi motori
  */
    while(Mot_Acc)
    {
      if(Mot_Vel > 0 )
      {
        RxToLx(Mot_Vel);
      }
      else if(Mot_Vel < 0)
      {
        LxToRx((Mot_Vel * - 1));
      }
      else
      {
        Stop();
      }
    }
    if(!Mot_Acc) Stop();
    Serial.println(A_Data);
    Serial.println(Mot_Vel);
    Serial.println(Mot_Acc);
  }
}


