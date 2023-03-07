/* 
 * File:   QEI.h
 * Author: GEII Robot
 *
 * Created on 23 janvier 2023, 08:34
 */

#ifndef QEI_H
#define	QEI_H

void QEIUpdateData();
void InitQEI1();
void InitQEI2();
void SendPositionData();

#define FREQ_ECH_QEI 250


#endif	/* QEI_H */

