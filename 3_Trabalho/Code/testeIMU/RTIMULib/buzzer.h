#ifndef _BUZZER_H_
#define _BUZZER_H_


#ifndef	__WIRING_PI_H__
  #include <wiringPi.h>
#endif
  
#include <softTone.h>
#include <time.h>

#ifndef BUZZER_PIN
  #define BUZZER_PIN 26
#endif

#ifdef __cplusplus
  extern "C"{
#endif

void buzzerInit();
void buzzerTone(char nota, int duracao);

#ifdef __cplusplus
  }
#endif

#endif // _BUZZER_H_
