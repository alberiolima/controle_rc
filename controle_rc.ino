/* 
 *  Controle sem fio usando controle remoto do PS2
 * 
 *  Autor: Albério Lima
 *  (conforme outras pessoas forem alterando o código seu nome será colocado aqui)
 *  
 */
#include <SPI.h>
#include <RF24.h>

#define ctrl_joy0_botao   2 //Primeiro joystick botão
#define ctrl_joy1_botao   3 //Segundo joystick botão
#define intervaloEnvio   50 //Intervalo entre os envios

struct potenciometro {
  int maximo=0;
  int minimo=2048;
  int meio;
  int porta;
  int valor=0;
};
potenciometro potenc[4];

unsigned long tempoEnviar = 0;

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(9, 10);

void setup() {
  
  //Iniciar porta serial para depuração
  Serial.begin( 9600 );  
  
  //Definicao das portas de entradas para os potenciometros
  potenc[0].porta = A0;
  potenc[1].porta = A1;
  potenc[2].porta = A2;
  potenc[3].porta = A3;  
  
  //Definição da portas de entrada, que serão usados pelos joysticks
  pinMode( ctrl_joy0_botao, INPUT );
  pinMode( ctrl_joy1_botao, INPUT );
  for( int pot=0;pot<4;pot++){
    pinMode( potenc[pot].porta, INPUT );
  }

  //Calibragem
  for(int i = 0;i<50;i++){
    for(int pot = 0;pot < 4; pot++){
      potenc[pot].valor += analogRead( potenc[pot].porta );   
    }
  }
  for (int pot = 0;pot < 4; pot++){
    potenc[pot].meio = (potenc[pot].valor / 50 );
  }
  
  Serial.println( "meios" );
  for (int pot = 0;pot < 4; pot++){
    Serial.print( potenc[pot].meio  );
    Serial.print( ' ' );  
  }  
  Serial.println();

  radio.begin();   

  radio.setAutoAck(false);    
  radio.setDataRate(RF24_250KBPS);    

}

void loop() {

  

  if ( millis() >= tempoEnviar ) {
    tempoEnviar = millis() + intervaloEnvio;

    //Trada os quatro potenciômetros
    int dados_enviar = 0;
    for (int pot = 0;pot < 4; pot++){
      potenc[pot].valor = analogRead( potenc[pot].porta );    
      if ( potenc[pot].valor < potenc[pot].minimo ) {
        potenc[pot].minimo = potenc[pot].valor;
      } else if (potenc[pot].valor > potenc[pot].maximo ) {
        potenc[pot].maximo = potenc[pot].valor;
      }
    }
    
    //ler botão do joystick 1
    boolean joy0_BOTAO = digitalRead( ctrl_joy1_botao );

    //ler botão do joystick 2
    boolean joy1_BOTAO = digitalRead( ctrl_joy1_botao );

    //Envia os dados via radio
    radioTX();

    for (int pot = 0;pot < 4; pot++){
      Serial.print( potenc[pot].valor  );
      Serial.print( ' ' );  
    }
    Serial.println();    
  }

}

void radioTX(){
  byte data[16];
  for (int pot = 0;pot < 4; pot++){
    data[pot+pot] = potenc[pot].valor / 256;
    data[pot+pot+1] = potenc[pot].valor - ( data[pot+pot] * 256 );  
  }
  
  radio.stopListening();
  radio.openWritingPipe(pipe);
  radio.write( &data, sizeof(data) );  
}

