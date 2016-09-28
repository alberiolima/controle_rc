/* 
 *  Controle sem fio usando controle remoto do PS2
 * 
 *  Autor: Albério Lima
 *  (conforme outras pessoas forem alterando o código seu nome será colocado aqui)
 *  
 * 
 * pinagem modulo wireless
 * 
 * Nome  se8r01   PRO MINI(3.3V 8MHZ)
 * 3.3V   1       3.3V
 * GND    2       GND
 * CE     3       7
 * CSN    4       10
 * SCK    5       13
 * MOSI   6       11
 * MISO   7       12
 * IRQ    8       8
 */
 
#include "se8r01.h"

#define ctrl_joy0_botao   2 //Primeiro joystick botão
#define ctrl_joy1_botao   3 //Segundo joystick botão
#define intervaloEnvio   50 //Intervalo entre os envios

struct potenciometro {  
  int porta;
  int meio;
  int valor = 0;
};
potenciometro potenc[4];

unsigned long tempoEnviar = 0;

const int PAYLOAD_WIDTH = 12;

void init_radio() {
  if (!init_rf(10, 7, 8, PAYLOAD_WIDTH)) {
    Serial.println("Chip not found!");
    while (1);
  }
  setPower(POWER_5dbm);
  selectTxPipe(0);
  setRtr(15);
  //setRfSpeed( SPEED_500Kbps );
  changeMode(TX_MODE);  
  
}

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
  delay(2);
  //Calibragem
  for(int i = 0;i<50;i++){
    for(int pot = 0;pot < 4; pot++){
      potenc[pot].valor += analogRead( potenc[pot].porta );   
    }
    delay(1);
  }
  for (int pot = 0;pot < 4; pot++){
    potenc[pot].meio = ( potenc[pot].valor / 50 );
  }

  init_radio();

}

void loop() {  

  if ( millis() >= tempoEnviar ) {
    tempoEnviar = millis() + intervaloEnvio;

    //Trata os quatro potenciômetros
    for (int pot = 0;pot < 4; pot++){
      potenc[pot].valor = analogRead( potenc[pot].porta );
      if ( potenc[pot].valor < potenc[pot].meio ) {
        potenc[pot].valor = map( potenc[pot].valor, 0, potenc[pot].meio - 1, 0, 511  );
      } else {
        potenc[pot].valor = map( potenc[pot].valor, potenc[pot].meio, 1023, 512, 1023  );
      }
    }
    
    //ler botão do joystick 1
    boolean joy0_BOTAO = digitalRead( ctrl_joy0_botao );

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
  byte data[12];
  for (int pot = 0;pot < 4; pot++){
    data[pot+pot] = potenc[pot].valor / 256;
    data[pot+pot+1] = potenc[pot].valor - ( data[pot+pot] * 256 );  
  }
  data[8] = 0;
  data[9] = 0;
  data[10] = 0;
  data[11] = 0;
  
  sendWithAck(data);

}
