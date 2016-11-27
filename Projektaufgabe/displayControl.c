/* 
 * File:   displayControl.c
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 14:03
 */

#include <stdio.h>
#include "displayControl.h"

void initDisp(void) {

}

void dispSet(char topRow[], char bottomRow[]) {
    int i;
    for(i = 0; i < DISPLAY_ARRAY_SIZE; i++) {
        printf("%c", topRow[i]);
    }
    printf("\n");
    for(i = 0; i < DISPLAY_ARRAY_SIZE; i++) {
        printf("%c", bottomRow[i]);
    }
    printf("\n");
}
