#include "buzzer.h"

void buzzerInit(){
	softToneCreate(BUZZER_PIN);
}

void buzzerTone(char nota, int duracao){
	switch(nota){
	case 'C':
		softToneWrite(BUZZER_PIN, 262); // C4
		delay(duracao);
	break;
	case 'D':
		softToneWrite(BUZZER_PIN, 294); // D4
		delay(duracao);
	break;
	case 'E':
    softToneWrite(BUZZER_PIN, 330); // E4
    delay(duracao);
	break;
	case 'F':
    softToneWrite(BUZZER_PIN, 349); // F4
    delay(duracao);
	break;
	case 'G':
    softToneWrite(BUZZER_PIN, 392); // G4
    delay(duracao);
	break;
	case 'A':
    softToneWrite(BUZZER_PIN, 440); // A4
    delay(duracao);
	break;
	case 'B':
    softToneWrite(BUZZER_PIN, 494); // B4
    delay(duracao);
	break;
	case 'X':
		softToneWrite(BUZZER_PIN, 0);
	break;
	default:
		softToneWrite(BUZZER_PIN, 0);
	break;
	} /* FIM DO SWITCH CASE */
} /* FIM BUZZER TONE */
