#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>

#define BUZZER_PIN 7

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


int main(void){
if(wiringPiSetup() == -1){
printf("wiringPi initialization failed !");
return 1;
}

softToneCreate(BUZZER_PIN);

while(1){
		  buzzerTone('C', 100);
          buzzerTone('X', 100);
          buzzerTone('D', 100);
          buzzerTone('X', 100);
          buzzerTone('E', 200);
          buzzerTone('X', 100);
}

return 0;
}
