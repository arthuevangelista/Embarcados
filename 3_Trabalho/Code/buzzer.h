#ifndef _BUZZER_H_

#define _BUZZER_H_

#include <softTone.h>
#define buzzerPin 26

void buzzerInit();
void buzzerTone(char nota, int duracao);

#endif // _BUZZER_H_
