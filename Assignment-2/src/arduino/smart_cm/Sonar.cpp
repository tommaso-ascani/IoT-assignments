#include "Sonar.h"
#include <Arduino.h>

Sonar::Sonar(int trigPin, int echoPin) {
  this->EchoPin = echoPin;
  this->TrigPin = trigPin;
  pinMode(this->TrigPin, OUTPUT);
  pinMode(this->EchoPin, INPUT); 
}

float Sonar::getDist(){
    digitalWrite(this->TrigPin,LOW);                    //Invio impulso
    delayMicroseconds(3);                               //
    digitalWrite(this->TrigPin,HIGH);                   //
    delayMicroseconds(5);                               // 
    digitalWrite(this->TrigPin,LOW);                    //
    
    float timeMicroSec = pulseIn(this->EchoPin, HIGH);  //Aspetta che il pin selezionato vada da LOW ad HIGH, ovvero che il sonar riceva l'echo dell'impulso precedentemente inviato
    float t = timeMicroSec / 1000.0 / 1000.0 / 2;       //Trasformo il tempo in microsecondi in secondi e poi divido per due poichè è il tempo che l'impulso impiega per andare, 
                                                        //  incontrare un ostacolo, rimbalzarci sopra, e tornare. Quindi / 2 per calcolare solo il tempo di andata/ritorno dell'impulso
    float distance = t*(this->VelSuono);                //Calcolo la distanza
    return distance;  
}
