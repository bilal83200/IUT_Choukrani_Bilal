#ifndef ROBOT_H
#define ROBOT_H
#define PWMPER 24.0
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1





typedef struct robotStateBITS {

    union {

        struct {
            unsigned char taskEnCours;

            float vitesseGaucheConsigne;
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            float distanceTelemetreGauche;
            float distanceTelemetreCentre;
            float distanceTelemetreDroite;
            float distanceTelemetreExDroite;
            float distanceTelemetreExGauche;
        };
    };
} ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */


