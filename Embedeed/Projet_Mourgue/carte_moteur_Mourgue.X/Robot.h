
#ifndef ROBOT_H
#define ROBOT_H

typedef struct robotStateBITS {



        struct {
            unsigned char taskEnCours;
            float vitesseGaucheConsigne;
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            float distanceTelemetreDroit; 
            float distanceTelemetreGauche;
            float distanceTelemetreCentre;
            float distanceTelemetreDroit2 ;
            float distanceTelemetreGauche2 ;
            float vitesseDroitFromOdometry ;
            float vitesseGaucheFromOdometry ;
            float vitesseLineaireFromOdometry ;
            float vitesseAngulaireFromOdometry ;
            float xPosFromOdometry_1 ;
            float yPosFromOdometry_1 ;
            float xPosFromOdometry ;
            float yPosFromOdometry ;
            float angleRadianFromOdometry ;
            float angleRadianFromOdometry_1 ;
            
            PidCorrector PidX;
            PidCorrector PidTheta;
            
        };

} ROBOT_STATE_BITS;

extern volatile ROBOT_STATE_BITS robotState;



#endif /* ROBOT_H */