#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"                                     
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "main.h"


float ADCValue0;
float ADCValue1;
float ADCValue2;
unsigned char stateRobot = STATE_NULL;
unsigned char nextStateRobot = STATE_NULL;


int main(void) {
    InitOscillator();

    InitIO();
    InitPWM();

    InitTimer23();
    InitTimer1();
    InitADC1();
    InitTimer4();

    PWMSetSpeedConsigne(24, MOTEUR_DROIT);
    PWMSetSpeedConsigne(24, MOTEUR_GAUCHE);


    //    LED_BLANCHE_1 = 1;
    //    LED_BLEUE_1 = 1;
    //    LED_ORANGE_1 = 1;
    //    LED_ROUGE_1 = 1;
    //    LED_VERTE_1 = 1;
    //







    while (1) {

        if (_RH0 == 1)
            stateRobot = STATE_ATTENTE;

        if (timestamp >= 60000)
            stateRobot = STATE_NULL;

        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreDroite = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreExDroite = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreExGauche = 34 / volts - 5;

        }
        if (robotState.distanceTelemetreDroite > 35 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreGauche > 35) {
            LED_BLANCHE_2 = 1;
            LED_BLEUE_2 = 1;
            LED_ORANGE_2 = 1;
            LED_ROUGE_2 = 1;
            LED_VERTE_2 = 1;
            
            
        }
        else{
            LED_BLANCHE_2 = 0;
            LED_BLEUE_2 = 0;
            LED_ORANGE_2 = 0;
            LED_ROUGE_2 = 0;
            LED_VERTE_2 = 0;
            
        }

    } // fin main
}

void lumierecapteur() {
    if (robotState.distanceTelemetreGauche < 50) {
        LED_BLEUE_1 = 1;
    } else LED_BLEUE_1 = 0;
    if (robotState.distanceTelemetreCentre < 40) {
        LED_ORANGE_1 = 1;
    } else LED_ORANGE_1 = 0;
    if (robotState.distanceTelemetreDroite < 50) {
        LED_ROUGE_1 = 1;
    } else LED_ROUGE_1 = 0;

    if (robotState.distanceTelemetreExDroite < 50) {
        LED_VERTE_1 = 1;
    } else LED_VERTE_1 = 0;

    if (robotState.distanceTelemetreExGauche < 50) {
        LED_BLANCHE_1 = 1;
    } else LED_BLANCHE_1 = 0;
}

void OperatingSystemLoop(void) {


    switch (stateRobot) {
        case STATE_NULL:
            PWMSetSpeedConsigne(0.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0.0, MOTEUR_GAUCHE);
            break;
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0.0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(24.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(24.0, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;

            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(15.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(10.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-10.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10.0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RALENTI:
            PWMSetSpeedConsigne(0.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0.0, MOTEUR_GAUCHE);
            stateRobot = STATE_RALENTI_EN_COURS;
            break;
        case STATE_RALENTI_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_AVANCE_DOUCEMENT:
            PWMSetSpeedConsigne(15.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15.0, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_DOUCEMENT_EN_COURS;
            break;
        case STATE_AVANCE_DOUCEMENT_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
             case STATE_GAUCHE2:
            PWMSetSpeedConsigne(17.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0.0, MOTEUR_GAUCHE);
            stateRobot = STATE_GAUCHE2_EN_COURS;
            break;
        case STATE_GAUCHE2_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
                   case STATE_DROITE2:
            PWMSetSpeedConsigne(0.0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(17.0, MOTEUR_GAUCHE);
            stateRobot = STATE_DROITE2_EN_COURS;
            break;
        case STATE_DROITE2_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //ÈDtermination de la position des obstacles en fonction des ÈÈËtlmtres
    if (robotState.distanceTelemetreDroite < 38 &&
            robotState.distanceTelemetreCentre > 44 &&
            robotState.distanceTelemetreGauche > 38) //Obstacle ‡droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroite > 38&&
            robotState.distanceTelemetreCentre > 44 &&
            robotState.distanceTelemetreGauche < 38) //Obstacle ‡gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 43) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroite > 38 &&
            robotState.distanceTelemetreCentre > 44 &&
            robotState.distanceTelemetreGauche > 38) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;
    else if (robotState.distanceTelemetreCentre < 44) //pas d?obstacle
        positionObstacle = OBSTACLE_DEVANT;
    else if (robotState.distanceTelemetreExDroite < 33
            && robotState.distanceTelemetreExGauche < 33)
        positionObstacle = OBSTACLE_EX;
    else if (robotState.distanceTelemetreDroite < 38 &&
            robotState.distanceTelemetreCentre < 44 &&
            robotState.distanceTelemetreGauche < 38) //pas d?obstacle
        positionObstacle = OBSTACLE_DEVANT;
    else if (robotState.distanceTelemetreExGauche < 33 &&
            robotState.distanceTelemetreCentre < 44 &&
            robotState.distanceTelemetreGauche < 38)
        positionObstacle = OBSTACLE_EX_GAUCHE_CENTRE;
    else if (robotState.distanceTelemetreDroite < 38 &&
            robotState.distanceTelemetreCentre < 44 &&
            robotState.distanceTelemetreExDroite < 33)
        positionObstacle = OBSTACLE_EX_DROITE_CENTRE;
    else if (robotState.distanceTelemetreCentre > 46) // ne sert a rien 
        positionObstacle = OBSTACLE_EN_FACE2;
    else if (robotState.distanceTelemetreGauche < 28
            && robotState.distanceTelemetreExGauche < 33 &&
            robotState.distanceTelemetreDroite < 28
            && robotState.distanceTelemetreExDroite < 33)
        positionObstacle = OBSTACLE_EX_GAUCHE_GAUCHE_ET_EX_DROITE_DROITE;
 else if ( robotState.distanceTelemetreExDroite < 39)
        positionObstacle = OBSTACLE_EXD;
 else if ( robotState.distanceTelemetreExGauche < 39)
        positionObstacle = OBSTACLE_EXG;

    //ÈDtermination de lÈ?tat ‡venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_DEVANT)
        nextStateRobot = STATE_RALENTI;
    else if (positionObstacle == OBSTACLE_EX)
        nextStateRobot = STATE_AVANCE_DOUCEMENT;
    else if (positionObstacle == OBSTACLE_EX_GAUCHE_CENTRE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EX_DROITE_CENTRE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_DROITE_ET_EX)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_GAUCHE_ET_EX)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EX_GAUCHE_GAUCHE_ET_EX_DROITE_DROITE)
        nextStateRobot = STATE_AVANCE;
 else if (positionObstacle == OBSTACLE_EXD)
        nextStateRobot = STATE_GAUCHE2;
   else if (positionObstacle == OBSTACLE_EXG)
        nextStateRobot = STATE_DROITE2;

    //Si l?on n?est pas dans la transition de lÈ?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}



