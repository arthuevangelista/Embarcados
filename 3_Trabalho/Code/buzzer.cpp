#include "buzzer.h"

void buzzerInit(){
	softToneCreate(buzzerPin);
}

void buzzerTone(char nota, int duracao){
	switch(nota){
	case 'C':
		softToneWrite(buzzerPin, 262); // C4
		delay(duracao);
	break;
	case 'D':
		softToneWrite(buzzerPin, 294); // D4
		delay(duracao);
	break;
	case 'E':
    softToneWrite(buzzerPin, 330); // E4
    delay(duracao);
	break;
	case 'F':
    softToneWrite(buzzerPin, 349); // F4
    delay(duracao);
	break;
	case 'G':
    softToneWrite(buzzerPin, 392); // G4
    delay(duracao);
	break;
	case 'A':
    softToneWrite(buzzerPin, 440); // A4
    delay(duracao);
	break;
	case 'B':
    softToneWrite(buzzerPin, 494); // B4
    delay(duracao);
	break;
	case 'X':
		softToneWrite(buzzerPin, 0);
	break;
	default:
		softToneWrite(buzzerPin, 0);
	break;
	} /* FIM DO SWITCH CASE */
} /* FIM BUZZER TONE */
