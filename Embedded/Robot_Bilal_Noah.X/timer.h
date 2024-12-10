#ifndef TIMER_H
#define TIMER_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

extern unsigned long timestamp;
extern unsigned long stop;
void InitTimer23(void);
void InitTimer1(void);
void InitTimer4(void);
void SetFreqTimer1(float freq);
void SetFreqTimer4(float freq);

//void PWMSetSpeed(float vitesseEnPourcents, int moteur);
#endif /* TIMER_H */

