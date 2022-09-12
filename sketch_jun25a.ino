#include <avr/io.h>
#include <SPI.h>
#include <MFRC522.h>
#include <string.h> 

#define RST  9			
#define SS  10
int cerradura = 5;//Se establecen los pines de salida
int alarma = 6;
MFRC522 mfrc522(SS,RST);
byte LecturaUID[4]; 				
byte Usuario1[4]= {0x90, 0x0E, 0xE4, 0xA4} ;  //Se definen las trajetas que el sistema admite  
byte Usuario2[4]= {0x06, 0x76, 0x25, 0xD9} ;  
void setup() {
  pinMode(cerradura,OUTPUT);
  digitalWrite(cerradura,LOW);
  pinMode(alarma,OUTPUT);
  digitalWrite(alarma,LOW);
  Serial.begin(9600);			
  SPI.begin();				
  mfrc522.PCD_Init();		
  Serial.println("Sistema iniciado");		
}

void loop() {
  if ( !mfrc522.PICC_IsNewCardPresent())	//Valida que haya una tarjeta en el lector
    return;				
  
  if ( !mfrc522.PICC_ReadCardSerial()) 		//Valida que se pueda leer la tarjeta
    return;					
    
  Serial.print("UID:");			
  for (byte i = 0; i < mfrc522.uid.size; i++) {	//EL codigo unico de la tarjeta es almacenado en una variable para luego compararla,y ademas se presenta en pantalla del arduino ID dicho codigo
    if (mfrc522.uid.uidByte[i] < 0x10){		
      Serial.print(" 0");			
    }
    else{						
      Serial.print(" ");				
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX); 
    LecturaUID[i]=mfrc522.uid.uidByte[i];   	
    }
  Serial.print("\t");   		                       
  if(comparaUID(LecturaUID, Usuario1)){	//Si la tarjeta coincide con las almacenadas en memoria,permite el acceso y se activa la cerradura
    Serial.println("Bienvenido");
    digitalWrite(cerradura,HIGH);
    delay(2000);	
    digitalWrite(cerradura,LOW);
  }
  else if(comparaUID(LecturaUID, Usuario2)){
    Serial.println("Bienvenido");
    digitalWrite(cerradura,HIGH);
    delay(2000);	
    digitalWrite(cerradura,LOW);
  }	
  else{				
    Serial.println("Usuario no registrado"); //Si la tarjeta no esta registrada el buzzer lo indica mediante un pitido intermitente 
    digitalWrite(alarma,HIGH);
    delay(100);	 
    digitalWrite(alarma,LOW);
    delay(100);	
    digitalWrite(alarma,HIGH);
    delay(100);	 
    digitalWrite(alarma,LOW);
    delay(100);	
    digitalWrite(alarma,HIGH);
    delay(100);
    digitalWrite(alarma,LOW);
  }          
  mfrc522.PICC_HaltA();  	           
}
boolean comparaUID(byte lectura[],byte usuario[])	
{
  for (byte i=0; i < mfrc522.uid.size; i++){		
  if(lectura[i] != usuario[i])				
    return(false);					
  }
  return(true);					
}
