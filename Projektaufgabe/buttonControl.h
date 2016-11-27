/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   buttonControl.h
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 15:19
 */

#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include "thermoTypes.h"

void initButton(void);

uint8_t isPressed(void);

button_t getButton(void);

#endif /* BUTTONCONTROL_H */

