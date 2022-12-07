/* 
 * File:   CB_TX1.h
 * Author: TP_EO_6
 *
 * Created on 24 novembre 2022, 13:51
 */

#ifndef CB_TX1_H
#define	CB_TX1_H

void SendMessage(unsigned char* message, int length);
void CB_TX1_Add(unsigned char value);
void SendOne(void);
unsigned char CB_TX1_IsTranmitting(void);
unsigned char CB_TX1_Get(void);
int CB_TX1_DataSize(void);
int CB_TX1_RemainingSize(void);

#endif	/* CB_TX1_H */

