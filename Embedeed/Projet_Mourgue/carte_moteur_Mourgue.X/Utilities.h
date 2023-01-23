/* 
 * File:   Utilities.h
 * Author: GEII Robot
 *
 * Created on 23 janvier 2023, 10:28
 */


#ifndef UTILITIES_H
#define	UTILITIES_H


double Modulo2PIAngleRadian(double angleRadian) ;
float getFloat(unsigned char *p, int index);
double getDouble(unsigned char *p, int index);
void getBytesFromFloat(unsigned char *p, int index, float f);
void getBytesFromInt32(unsigned char *p, int index, long in);
void getBytesFromDouble(unsigned char *p, int index, double d);

#endif /*UTILITIES_H*/




