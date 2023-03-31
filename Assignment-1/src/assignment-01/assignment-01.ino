#include <avr/sleep.h>
#include <EnableInterrupt.h>

void(* resetFunc) (void) = 0;               //Funzione di default per il reset programmatico di Arduino

#define LED_PIN_1 7
#define LED_PIN_2 8
#define LED_PIN_3 9
#define LED_PIN_4 10
#define LED_ROSSO 11
#define POT_PIN A0

#define WAIT_TIME 1000                      //Tempo in millisecondi prima che Arduino vada in Sleep Mode

bool direction = true;                      //Direzione di lampeggiamento dei led durante il gioco
int position = 7;                           //Posizione iniziale del lampeggiamento dei led durante il gioco
int speed;                                  //Intervallo in  millisedcondi del lampeggiamento dei led durante il gioco (progressivamente più veloce all'aumentare del livello); Usata anche per tenere conto del livello corrente
int tempo_lamp;                             //Durata del lampeggiamento dei led durante il gioco in secondi
int tempo_stop = 2000;                      //Intervallo di tempo che ha il giocatore per premere il pulsante corrispondente al led rimasto acceso (valore progressivamente più basso all'aumentare del livello)
int dim_factor = 200;                       //Fattore di dimuizione dell'intervallo tempo_stop
long int position_p;                        //Posizione casuale del led che rimarrà acceso durante il livello corrente
long int tempo_temp;                        //Variabile temporanea usata per sapere i millisecondi correnti
int btn_pressed;                            //Valore del bottone premuto durante il livello
int punteggio = 0;                          //Valore del punteggio

bool waiting = true;                        //Usato per sapere lo stato corrente del gioco
bool sleeping = false;                      //Usato per sapere lo stato corrente del gioco
bool game = false;                          //Usato per sapere lo stato corrente del gioco
bool bouncing = false;                      //Usato per non misurare i rimbalzi del bottone, quindi i molteplici input premuti
bool check_win = false;                     //Usato per sapere se il livello è stato completato con successo oppure no

int fadeAmount = 5;                         //Parametro usato per la velocità del fade del led rosso
int currIntensity;                          //Quantità attuale dell'intensità di corrente
int waiter = 0;                             //Variabile usata per il controllo del tempo prima di andare in Sleep Mode

void setup() {
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT); 
  pinMode(LED_PIN_3, OUTPUT); 
  pinMode(LED_PIN_4, OUTPUT);
  pinMode(LED_ROSSO, OUTPUT);
  
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  si_inter();
}

void loop() {      
  while(waiter < WAIT_TIME and waiting == true){
    f_wait();
  }

  if(sleeping == true){
    f_sleep();
  }

  if(bouncing == true){
    f_bouncing();
  }  

  if(game == true){
    f_game();
  }
}

//##############################################################################################

void f_bouncing(){                           //Funzione che evita che vengano presi in input i rimbalzi del pulsante premuto
  no_inter();
  delay(200);
  si_inter();
  bouncing = false;
}

void no_inter(){                              //Funzione che disabilità gli interrupt nei pin dei bottoni
  disableInterrupt(2);
  disableInterrupt(3);
  disableInterrupt(4);
  disableInterrupt(5);
}

void si_inter(){                              //Funzione che abilita gli interrupt nei pin dei bottoni
  enableInterrupt(2, btnPres, HIGH);
  enableInterrupt(3, btnPres, HIGH);
  enableInterrupt(4, btnPres, HIGH);
  enableInterrupt(5, btnPres, HIGH);
}

void f_game(){                                                    //Funzione che fa iniziare il gioco
  Serial.println("GO!");
 
  Serial.println("Livello: " + String(speed));
  speed = map(speed, 1,8, 8, 1);                                  //Si converte il valore di speed, che è considerato il livello corrente del gioco,
  speed = speed * 100;                                            //        nel valore, in millisecondi, dell'intervallo di lampeggiamento dei led di gioco
  tempo_temp = millis();                                          //Si ottiene il valore corrente dei millisecondi
  randomSeed(analogRead(4));                                      //Si inzializza "casualmente" il seed del generatore pseudo-casuale, leggendo il valore della corrente su un pin analogico non connesso
  tempo_lamp = random(3, 10);
  
  while(tempo_temp  + (tempo_lamp*1000) > millis() ){             //Ciclo di lampeggiamento dei led che dura tempo_lamp*1000 (in millisecondi)
    digitalWrite(position, HIGH);
    delay(speed);
    digitalWrite(position, LOW);
      
    if(direction == true){
      position++;
    }else{
      position--;
    }
  
    if(position == 10){
      direction = false;
    }else if(position == 7){
      direction = true;
    }
  }
  randomSeed(analogRead(4));                                      //Si inzializza "casualmente" il seed del generatore pseudo-casuale, leggendo il valore della corrente su un pin analogico non connesso
  position_p = random(7, 11);
  
  digitalWrite(position_p, HIGH);                                 //Accende il led scelto casualmente per quel livello
  delay(tempo_stop);                                              //    lo tiene acceso per il tempo tempo_stop
  digitalWrite(position_p, LOW);                                  //     e poi lo spegne

  if(check_win == true){                                          //Controlla se il flag di completamento del livello è positivo
    vittoria();                                                   //      
  }else if(check_win == false){                                   //      oppure è negativo
    sconfitta();                                                  //
  }
}

void vittoria(){                                                  //Funzione richiamata solo in caso di superamento del livello con successo
  Serial.println("");
  Serial.println("VITTORIA");
  punteggio++;                                                    //Incremento del punteggio corrente
  Serial.println("New point! Score: " + String(punteggio));

  speed = speed / 100;
  speed = map(speed, 8, 1, 1, 8);

  if(speed >= 8){                                                 //Controllo se il livello corrente è l'ultimo
    Serial.println("Game complete!");                             //    
    delay(100);                                                   //    se lo è allora si resetta il gioco              
    resetFunc();                                                  //
  }
  
  check_win = false;                                              //Setta il flag di completamento del livello a false
  speed++;                                                        //Si incrementa il valore del livello corrente
  tempo_stop = tempo_stop - dim_factor;                           //Si diminuisce l'intervallo di tempo che ha il giocatore per premere il pulsante
}

void sconfitta(){                                                 //Funzione richiamata solo in caso di NON superamento del livello
  Serial.println("");
  Serial.println("SCONFITTA");
  Serial.println("Game Over. Final Score: " + String(punteggio));
  delay(10000);
  resetFunc();                                                    //Si resetta il gioco
}

void f_sleep(){                                                   //Funzione per far andare il Sleep Mode Arduino
  analogWrite(LED_ROSSO, 0);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  sleep_enable();
  sleep_mode();
  
  sleep_disable();
  sleeping = false;                                               //Si setta lo stato di Arduino corrente
  game = false;                                                   //Si setta lo stato di Arduino corrente
  waiter = 0;
  currIntensity = 0;
  fadeAmount = 5;
}

void f_wait(){                                                                                                            //Funzione per far aspettare ad Arduino che il gioco cominci
  speed = map(analogRead(POT_PIN), 0, 1023, 1, 8);                                                                        //Si legge il valore della corrente dal potenziometro, che corrisponde al livello iniziale di difficoltà
  tempo_stop = (map(speed, 1,8, 8, 1) * 200) + 400;                                                                       //      settando di conseguenza l'intervallo di tempo che ha il giocatore per premere il pulsante giusto
  
  if(waiter == 0){
    Serial.println("");
    Serial.println("Welcome to the Catch the Bouncing Led Ball Game. Press Key T1 to Start");
  }
  
  analogWrite(LED_ROSSO, currIntensity);                                                                                  //Si effettua il fade del led rosso 
  currIntensity = currIntensity + fadeAmount;                                                                             //
  if (currIntensity == 0 || currIntensity == 255) {                                                                       //
    fadeAmount = -fadeAmount;                                                                                             //
  }
  delay(10);
  waiter++;
  
  if(waiter == WAIT_TIME){                                                                                                //Se il tempo corrente in attesa arriva al valore indicato allora 
    sleeping = true;                                                                                                      //      Si setta lo stato corrente di Arduino di Sleeping a true
  }
}

void btnPres(){                                                               //Funzione che controlla se e quale tasto viene premuto
  for(int i=0; i<4; i++){                                                     //Ciclo che controlla se qualche bottone è stato premuto
    if(digitalRead(i+2) == 1){                                                //      controllando i pin di input se sono in HIGH == 1
      btn_pressed = i+2;                                                      //      in caso positivo si memorizza il numero del bottone premuto
      break;                                                                  //      e si esce dal loop
    }
  }
  
  if(sleeping == false){                                                      //Se Arduino NON sta dormendo
    game = true;                                                              //      si setta che inizi il gioco
    waiting = false;                                                          //      e che non si aspetti
    analogWrite(LED_ROSSO, 0);                                                //      spegnendo il led rosso
  }

  if(game == true){                                                           //Se Arduino sta giocando
    if(btn_pressed+5 == position_p){                                          //      si controlla se il pulsante premuto corrisponde al led rimasto acceso
      check_win = true;                                                       //      in caso positivo si setta che si avrà superato il livello con successo
    }else{                                                                    //
      check_win = false;                                                      //      altrimenti si setta che il livello NON è stato superato
    }
  }
  
  bouncing = true;                                                            //Si setta che subito dopo il pulsante premuto si avranno dei rimablzi del pulsante
}
