/* 
 * File:   displayControl.c
 * Author: Kuhjunge
 *
 * Created on 27. November 2016, 14:03
 */

#include "displayControl.h"
#if SIMULATOR > 0
#include <stdio.h>
#endif

void initDisp(void) {

}

// in types.h SIMULATOR auf 1 stellen um diese Implementation zu nutzen
#if SIMULATOR > 0
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
#else
// in types.h SIMULATOR auf 0 stellen um diese Implementation zu nutzen
void dispSet(char topRow[], char bottomRow[]) {
    // ToDo
}
#endif
