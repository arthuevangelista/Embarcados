#ifndef _BUZZER_HPP_

#define _BUZZER_HPP_

#include <softTone.h>
#define buzzerPin 26

void buzzerInit();
void buzzerTone(char nota, int duracao);

#endif // _BUZZER_HPP_
