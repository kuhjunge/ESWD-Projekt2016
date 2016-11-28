/* 
 * File:   types.h
 * Author: Chris Deter
 *
 * Created on 23. November 2016, 18:11
 */

#ifndef TYPES_H
#define TYPES_H

// defines to be used as boolean
#define FALSE 0
#define TRUE !FALSE

#define SIMULATOR 1

typedef unsigned char		uint8_t;
//typedef unsigned short int	uint16_t;
//typedef unsigned long		uint32_t;

typedef signed char		int8_t;
#if SIMULATOR > 0
typedef signed short int	int16_t;
#endif
//typedef signed long		int32_t;

#endif /* TYPES_H */

