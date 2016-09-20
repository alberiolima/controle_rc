/* 
 *  Controle sem fio usando controle remoto do PS2
 * 
 *  Autor: Albério Lima
 *  (conforme outras pessoas forem alterando o código seu nome será colocado aqui)
 *  
 */

#define ctrl_joy1_x      A0 //Primeiro joystick movimento vertical
#define ctrl_joy1_y      A1 //Primeiro joystick movimento horizontal
#define ctrl_joy1_botao  A1 //Primeiro joystick botão

#define ctrl_joy2_x      A2 //Segundo joystick movimento vertical
#define ctrl_joy2_y      A3 //Segundo joystick movimento horizontal
#define ctrl_joy2_botao  A1 //Segundo joystick botão

#define intervaloEnvio   50 //Intervalo entre os envios

unsigned long tempoEnviar = 0;

void setup() {

  //Iniciar porta serial para depuração
  Serial.begin( 9600 );
  
  
  //Definição da portas de entrada, que serão usados pelos joysticks
  pinMode( ctrl_joy1_x, INPUT );
  pinMode( ctrl_joy1_y, INPUT );
  pinMode( ctrl_joy1_botao, INPUT );
  pinMode( ctrl_joy2_y, INPUT );
  pinMode( ctrl_joy2_x, INPUT );
  pinMode( ctrl_joy2_botao, INPUT );

  //Calibragem 

}

void loop() {

  if ( millis() >= tempoEnviar ) {
    tempoEnviar = millis() + intervaloEnvio;

    int joy1_X = analogRead( ctrl_joy1_x );
    int joy1_Y = analogRead( ctrl_joy1_y );
    boolean joy1_BOTAO = digitalRead( ctrl_joy1_botao );
    
    int joy2_X = analogRead( ctrl_joy2_y );
    int joy2_Y = analogRead( ctrl_joy2_x );
    boolean joy2_BOTAO = digitalRead( ctrl_joy2_botao );

    Serial.print( joy1_X );
    Serial.print( ' ' );
    Serial.print( joy1_Y );
//    Serial.print( ' ' );
    
//    Serial.print( joy2_X );
//    Serial.print( ' ' );
//    Serial.print( joy2_Y );
    Serial.println();
    
  }

}
