/*
 *
 * The Ribbit VM implementation in C
 */



// @@(feature c/gc/treadmill
#define TREADMILL
// )@@



#define DEBUG_GC
#ifdef DEBUG_GC
//#include <assert.h>
#define assert(code) \
    if (!(code)) { \
        printf("assertion failed: %s\n",#code); \
        __asm__("int3;"); \
    } 
#else
#define assert(code) ;
#endif

#ifdef DEBUG
#include <assert.h>
#endif

#ifdef MARK_SWEEP_DSW
#define MARK_SWEEP
#endif








#ifdef DEBUG_I_CALL
#define DEBUG
#endif
#ifdef KERNEL
#include "kernel.h"
#endif

#ifndef KERNEL
#ifdef DEBUG

#include <stdio.h>

#define PRINTLN()                                                              \
  do {                                                                         \
    printf("\n");                                                              \
  } while (0)

#endif

#ifndef NO_STD

#include <stdio.h>
#include <stdlib.h>

#endif
#endif


#ifdef ARG_V

char** argv=NULL;
int argc=0;

#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "ConstantFunctionResult"


// @@(feature (not compression/lzss/2b)
// @@(replace "3,46,46,33,44,43,46,36,44,102,111,101,37,37,44,45,46,37,44,33,46,38,44,58,46,42,44,43,44,45,44,46,46,46,44,46,58,47,44,46,43,58,44,46,45,60,44,46,61,44,46,62,44,46,63,44,46,94,44,46,95,44,114,97,99,44,114,100,99,44,116,114,111,112,45,116,117,111,100,116,115,44,46,126,44,42,37,37,44,121,108,112,112,97,37,37,44,116,105,120,101,37,37,44,100,108,111,102,37,37,44,100,105,37,37,44,116,110,101,105,116,111,117,113,37,37,44,100,102,45,116,117,111,100,116,115,37,37,44,100,110,101,112,112,97,44,113,115,115,97,44,118,115,115,97,44,104,115,97,114,99,44,63,116,99,101,106,98,111,45,102,111,101,44,63,118,113,101,44,114,111,114,114,101,44,104,99,97,101,45,114,111,102,44,63,116,114,111,112,45,116,117,112,110,105,44,63,114,101,103,101,116,110,105,44,114,111,116,99,101,118,62,45,116,115,105,108,44,103,110,105,114,116,115,62,45,114,101,98,109,117,110,44,63,116,114,111,112,45,116,117,112,116,117,111,44,63,101,114,117,100,101,99,111,114,112,44,101,115,114,101,118,101,114,44,120,117,97,45,101,115,114,101,118,101,114,44,63,103,110,105,114,116,115,44,63,108,111,98,109,121,115,44,114,111,116,99,101,118,44,63,114,111,116,99,101,118,44,114,97,104,99,45,101,116,105,114,119,44,115,114,97,104,99,45,101,116,105,114,119,44,43,37,37,44,121,108,112,112,97,44,115,110,111,99,44,116,115,105,108,44,101,110,105,108,119,101,110,44,63,98,105,114,37,37,44,63,114,97,104,99,44,116,114,111,112,45,116,117,112,116,117,111,45,116,110,101,114,114,117,99,44,104,116,103,110,101,108,44,116,111,110,44,63,108,108,117,110,44,116,115,105,108,45,116,110,105,114,112,44,45,37,37,44,60,37,37,44,50,100,108,101,105,102,37,37,44,112,97,109,37,37,44,121,97,108,112,115,105,100,44,63,118,113,101,37,37,44,50,103,114,97,37,37,44,101,115,111,108,99,37,37,44,63,101,99,110,97,116,115,110,105,44,63,114,105,97,112,44,100,102,45,114,97,104,99,45,101,116,105,114,119,37,37,44,49,100,108,101,105,102,37,37,44,49,103,114,97,37,37,44,48,100,108,101,105,102,37,37,44,44,44,44,98,105,114,37,37,59,26,116,71,118,79,117,115,80,71,118,116,102,48,46,117,115,81,137,102,45,57,117,117,71,118,91,116,71,118,96,44,118,104,76,117,14,117,115,82,71,118,79,117,103,139,138,102,30,55,117,112,31,119,108,106,72,117,104,71,118,73,118,70,117,107,70,117,72,117,103,71,118,73,118,70,117,107,135,92,150,11,104,151,86,117,103,96,50,118,105,70,117,103,151,74,117,103,143,102,18,0,4,118,106,103,71,118,73,118,70,117,106,135,32,71,118,73,118,70,117,106,135,46,71,118,73,118,70,117,106,135,32,55,117,112,151,85,117,103,19,119,107,105,72,117,103,71,118,64,118,106,70,117,103,71,118,73,118,70,117,106,135,32,151,74,117,103,143,102,13,49,116,14,117,92,119,81,117,106,72,117,105,70,117,104,151,90,117,104,8,118,70,117,104,135,112,71,118,73,118,70,117,104,135,35,151,96,38,117,104,8,118,70,117,104,135,41,71,118,112,146,77,118,84,119,106,115,13,72,117,103,71,118,79,118,105,70,117,103,70,117,104,151,82,118,72,117,105,116,71,118,73,118,70,117,104,135,40,71,118,73,118,70,117,104,135,35,151,96,32,117,104,31,119,105,114,70,117,104,151,96,35,117,104,31,119,105,114,70,117,72,117,104,151,96,34,117,104,8,118,70,117,104,135,41,71,118,84,119,105,115,13,72,117,104,71,118,79,118,104,70,117,104,71,118,73,118,70,117,104,135,40,151,74,117,104,32,118,104,104,151,89,117,104,14,117,96,33,118,81,117,105,81,117,72,117,104,151,77,118,96,39,117,105,148,103,151,103,96,43,117,104,14,118,104,96,40,117,104,151,96,42,117,104,8,118,70,117,104,135,41,71,118,73,118,70,117,104,135,40,151,85,117,104,8,118,70,117,104,135,101,71,118,73,118,70,117,104,135,35,151,96,47,117,104,8,118,70,117,104,135,116,71,118,73,118,70,117,104,135,35,151,78,118,113,104,8,118,70,117,104,135,102,71,118,73,118,70,117,104,135,35,151,86,117,104,77,118,103,100,72,117,105,70,117,104,150,10,88,116,151,78,118,114,104,140,102,25,8,118,70,117,104,126,77,118,103,99,72,117,104,70,117,103,150,10,88,116,151,78,118,114,103,138,102,22,55,117,115,60,137,102,31,8,118,70,117,104,70,117,104,77,118,103,100,72,117,105,70,117,104,150,10,88,116,151,78,118,114,104,140,102,60,66,119,125,113,96,52,116,71,118,116,102,47,13,118,115,77,103,139,102,77,66,119,121,116,116,102,7,66,119,117,116,117,102,52,66,119,117,116,118,102,72,56,119,106,104,76,117,18,118,105,103,141,18,118,104,116,151,85,117,104,140,102,73,56,119,105,116,76,117,30,118,105,103,141,138,102,55,55,117,104,56,119,72,117,107,64,118,70,117,107,105,103,151,74,117,105,143,102,44,55,117,112,29,118,96,51,118,80,118,107,115,61,92,117,104,115,44,71,118,94,118,80,118,106,115,62,103,151,74,117,70,117,104,140,102,14,55,117,114,28,118,80,118,72,117,106,104,62,117,70,117,104,151,74,117,104,141,102,28,58,118,105,103,141,102,57,66,119,117,116,119,102,33,42,117,103,138,102,50,115,49,102,49,55,117,112,50,118,72,117,106,104,55,117,103,151,96,46,118,70,117,104,104,70,117,104,151,74,117,104,141,102,36,55,117,104,37,118,93,118,106,70,117,104,72,117,103,151,74,117,103,141,102,37,37,118,114,103,139,102,51,55,117,114,0,3,118,70,117,105,72,117,104,77,118,103,98,76,117,55,117,104,0,7,118,65,10,118,106,70,117,104,72,117,103,151,74,117,103,141,112,96,37,117,104,151,74,117,104,98,76,117,55,117,104,28,118,65,8,118,106,72,117,104,70,117,103,151,74,117,103,141,112,138,102,21,55,117,116,30,118,87,117,72,117,104,117,151,74,117,103,139,102,26,55,117,103,138,102,61,7,117,103,139,102,62,5,117,103,139,102,27,1,119,116,105,103,141,102,20,13,118,112,103,139,102,40,1,119,119,87,117,104,103,63,118,114,105,149,66,119,116,64,118,114,83,118,107,116,135,45,151,82,118,116,105,98,76,117,55,117,103,0,10,118,104,105,151,82,118,106,116,66,119,116,107,95,118,104,135,48,147,83,118,126,104,135,65,151,82,118,104,125,83,118,96,58,118,110,8,104,104,96,53,118,109,103,141,112,77,118,103,100,72,117,105,70,117,104,150,10,126,151,78,118,114,104,140,102,41,1,119,120,87,117,104,103,139,102,14,55,117,114,1,119,116,80,118,72,117,106,104,62,117,70,117,104,151,74,117,104,141,71,118,116,102,21,55,117,116,30,118,87,117,72,117,104,117,151,74,117,103,139,102,42,21,117,90,117,103,139,102,19,13,118,114,103,139,102,46,13,118,105,103,147,70,117,105,70,117,103,151,112,146,89,117,104,151,89,117,103,141,102,39,75,117,125,102,43,75,117,124,102,23,75,117,122,102,38,75,117,117,102,32,75,117,120,102,35,75,117,119,102,34,75,117,118,102,8,75,117,116,102,9,11,117,55,117,112,13,118,107,81,117,103,151,90,117,103,139,139,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,102,56,66,119,117,116,120,102,53,66,119,117,116,121,102,58,66,119,117,116,122,102,29,66,119,117,116,124,102,16,66,119,117,116,125,102,12,66,119,117,116,126,102,15,66,119,117,116,127,102,6,66,119,117,116,128,102,4,66,119,117,116,129,102,24,66,119,117,116,130,102,10,66,119,117,116,131,102,11,66,119,117,116,132,102,54,66,119,117,116,134,102,80,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,114,136,64,136,63,136,59,136,65,136,66,136,67,136,68,136,69,136,70,136,74,136,75,136,76,136,78,136,79,136,71,136,72,136,73,102,81,66,119,119,122,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,114,135,41,135,39,135,46,135,95,135,46,135,40,102,82,66,119,119,117,66,119,116,114,135,32,102,17,66,119,117,116,123,102,5,66,119,117,116,133,137" (encode-as-bytes "auto" "" "," "")
unsigned char input[] = {3,46,46,33,44,43,46,36,44,102,111,101,37,37,44,45,46,37,44,33,46,38,44,58,46,42,44,43,44,45,44,46,46,46,44,46,58,47,44,46,43,58,44,46,45,60,44,46,61,44,46,62,44,46,63,44,46,94,44,46,95,44,114,97,99,44,114,100,99,44,116,114,111,112,45,116,117,111,100,116,115,44,46,126,44,42,37,37,44,121,108,112,112,97,37,37,44,116,105,120,101,37,37,44,100,108,111,102,37,37,44,100,105,37,37,44,116,110,101,105,116,111,117,113,37,37,44,100,102,45,116,117,111,100,116,115,37,37,44,100,110,101,112,112,97,44,113,115,115,97,44,118,115,115,97,44,104,115,97,114,99,44,63,116,99,101,106,98,111,45,102,111,101,44,63,118,113,101,44,114,111,114,114,101,44,104,99,97,101,45,114,111,102,44,63,116,114,111,112,45,116,117,112,110,105,44,63,114,101,103,101,116,110,105,44,114,111,116,99,101,118,62,45,116,115,105,108,44,103,110,105,114,116,115,62,45,114,101,98,109,117,110,44,63,116,114,111,112,45,116,117,112,116,117,111,44,63,101,114,117,100,101,99,111,114,112,44,101,115,114,101,118,101,114,44,120,117,97,45,101,115,114,101,118,101,114,44,63,103,110,105,114,116,115,44,63,108,111,98,109,121,115,44,114,111,116,99,101,118,44,63,114,111,116,99,101,118,44,114,97,104,99,45,101,116,105,114,119,44,115,114,97,104,99,45,101,116,105,114,119,44,43,37,37,44,121,108,112,112,97,44,115,110,111,99,44,116,115,105,108,44,101,110,105,108,119,101,110,44,63,98,105,114,37,37,44,63,114,97,104,99,44,116,114,111,112,45,116,117,112,116,117,111,45,116,110,101,114,114,117,99,44,104,116,103,110,101,108,44,116,111,110,44,63,108,108,117,110,44,116,115,105,108,45,116,110,105,114,112,44,45,37,37,44,60,37,37,44,50,100,108,101,105,102,37,37,44,112,97,109,37,37,44,121,97,108,112,115,105,100,44,63,118,113,101,37,37,44,50,103,114,97,37,37,44,101,115,111,108,99,37,37,44,63,101,99,110,97,116,115,110,105,44,63,114,105,97,112,44,100,102,45,114,97,104,99,45,101,116,105,114,119,37,37,44,49,100,108,101,105,102,37,37,44,49,103,114,97,37,37,44,48,100,108,101,105,102,37,37,44,44,44,44,98,105,114,37,37,59,26,116,71,118,79,117,115,80,71,118,116,102,48,46,117,115,81,137,102,45,57,117,117,71,118,91,116,71,118,96,44,118,104,76,117,14,117,115,82,71,118,79,117,103,139,138,102,30,55,117,112,31,119,108,106,72,117,104,71,118,73,118,70,117,107,70,117,72,117,103,71,118,73,118,70,117,107,135,92,150,11,104,151,86,117,103,96,50,118,105,70,117,103,151,74,117,103,143,102,18,0,4,118,106,103,71,118,73,118,70,117,106,135,32,71,118,73,118,70,117,106,135,46,71,118,73,118,70,117,106,135,32,55,117,112,151,85,117,103,19,119,107,105,72,117,103,71,118,64,118,106,70,117,103,71,118,73,118,70,117,106,135,32,151,74,117,103,143,102,13,49,116,14,117,92,119,81,117,106,72,117,105,70,117,104,151,90,117,104,8,118,70,117,104,135,112,71,118,73,118,70,117,104,135,35,151,96,38,117,104,8,118,70,117,104,135,41,71,118,112,146,77,118,84,119,106,115,13,72,117,103,71,118,79,118,105,70,117,103,70,117,104,151,82,118,72,117,105,116,71,118,73,118,70,117,104,135,40,71,118,73,118,70,117,104,135,35,151,96,32,117,104,31,119,105,114,70,117,104,151,96,35,117,104,31,119,105,114,70,117,72,117,104,151,96,34,117,104,8,118,70,117,104,135,41,71,118,84,119,105,115,13,72,117,104,71,118,79,118,104,70,117,104,71,118,73,118,70,117,104,135,40,151,74,117,104,32,118,104,104,151,89,117,104,14,117,96,33,118,81,117,105,81,117,72,117,104,151,77,118,96,39,117,105,148,103,151,103,96,43,117,104,14,118,104,96,40,117,104,151,96,42,117,104,8,118,70,117,104,135,41,71,118,73,118,70,117,104,135,40,151,85,117,104,8,118,70,117,104,135,101,71,118,73,118,70,117,104,135,35,151,96,47,117,104,8,118,70,117,104,135,116,71,118,73,118,70,117,104,135,35,151,78,118,113,104,8,118,70,117,104,135,102,71,118,73,118,70,117,104,135,35,151,86,117,104,77,118,103,100,72,117,105,70,117,104,150,10,88,116,151,78,118,114,104,140,102,25,8,118,70,117,104,126,77,118,103,99,72,117,104,70,117,103,150,10,88,116,151,78,118,114,103,138,102,22,55,117,115,60,137,102,31,8,118,70,117,104,70,117,104,77,118,103,100,72,117,105,70,117,104,150,10,88,116,151,78,118,114,104,140,102,60,66,119,125,113,96,52,116,71,118,116,102,47,13,118,115,77,103,139,102,77,66,119,121,116,116,102,7,66,119,117,116,117,102,52,66,119,117,116,118,102,72,56,119,106,104,76,117,18,118,105,103,141,18,118,104,116,151,85,117,104,140,102,73,56,119,105,116,76,117,30,118,105,103,141,138,102,55,55,117,104,56,119,72,117,107,64,118,70,117,107,105,103,151,74,117,105,143,102,44,55,117,112,29,118,96,51,118,80,118,107,115,61,92,117,104,115,44,71,118,94,118,80,118,106,115,62,103,151,74,117,70,117,104,140,102,14,55,117,114,28,118,80,118,72,117,106,104,62,117,70,117,104,151,74,117,104,141,102,28,58,118,105,103,141,102,57,66,119,117,116,119,102,33,42,117,103,138,102,50,115,49,102,49,55,117,112,50,118,72,117,106,104,55,117,103,151,96,46,118,70,117,104,104,70,117,104,151,74,117,104,141,102,36,55,117,104,37,118,93,118,106,70,117,104,72,117,103,151,74,117,103,141,102,37,37,118,114,103,139,102,51,55,117,114,0,3,118,70,117,105,72,117,104,77,118,103,98,76,117,55,117,104,0,7,118,65,10,118,106,70,117,104,72,117,103,151,74,117,103,141,112,96,37,117,104,151,74,117,104,98,76,117,55,117,104,28,118,65,8,118,106,72,117,104,70,117,103,151,74,117,103,141,112,138,102,21,55,117,116,30,118,87,117,72,117,104,117,151,74,117,103,139,102,26,55,117,103,138,102,61,7,117,103,139,102,62,5,117,103,139,102,27,1,119,116,105,103,141,102,20,13,118,112,103,139,102,40,1,119,119,87,117,104,103,63,118,114,105,149,66,119,116,64,118,114,83,118,107,116,135,45,151,82,118,116,105,98,76,117,55,117,103,0,10,118,104,105,151,82,118,106,116,66,119,116,107,95,118,104,135,48,147,83,118,126,104,135,65,151,82,118,104,125,83,118,96,58,118,110,8,104,104,96,53,118,109,103,141,112,77,118,103,100,72,117,105,70,117,104,150,10,126,151,78,118,114,104,140,102,41,1,119,120,87,117,104,103,139,102,14,55,117,114,1,119,116,80,118,72,117,106,104,62,117,70,117,104,151,74,117,104,141,71,118,116,102,21,55,117,116,30,118,87,117,72,117,104,117,151,74,117,103,139,102,42,21,117,90,117,103,139,102,19,13,118,114,103,139,102,46,13,118,105,103,147,70,117,105,70,117,103,151,112,146,89,117,104,151,89,117,103,141,102,39,75,117,125,102,43,75,117,124,102,23,75,117,122,102,38,75,117,117,102,32,75,117,120,102,35,75,117,119,102,34,75,117,118,102,8,75,117,116,102,9,11,117,55,117,112,13,118,107,81,117,103,151,90,117,103,139,139,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,71,118,116,102,56,66,119,117,116,120,102,53,66,119,117,116,121,102,58,66,119,117,116,122,102,29,66,119,117,116,124,102,16,66,119,117,116,125,102,12,66,119,117,116,126,102,15,66,119,117,116,127,102,6,66,119,117,116,128,102,4,66,119,117,116,129,102,24,66,119,117,116,130,102,10,66,119,117,116,131,102,11,66,119,117,116,132,102,54,66,119,117,116,134,102,80,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,114,136,64,136,63,136,59,136,65,136,66,136,67,136,68,136,69,136,70,136,74,136,75,136,76,136,78,136,79,136,71,136,72,136,73,102,81,66,119,119,122,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,66,119,116,114,135,41,135,39,135,46,135,95,135,46,135,40,102,82,66,119,119,117,66,119,116,114,135,32,102,17,66,119,117,116,123,102,5,66,119,117,116,133,137,0}; // RVM code that prints HELLO!
// )@@
// )@@


#ifndef NULL
#define NULL (0)
#endif

#ifndef KERNEL
typedef unsigned long size_t;
#endif


// For versions of the standard lower than C23, bool, true and false needs to
// be defined
#if __STDC_VERSION__ < 202311L
    typedef unsigned char bool;
    #define true (1)
    #define false (0)
#endif

// an unsigned byte value for the REPL's code
typedef unsigned char byte;

// a tagged value
typedef unsigned long obj;

// a number
typedef long num;


// GC constants

// ATTENTION PLEASE: don't change the base size during testing
#ifndef BASE_HEAP_SIZE_FIELDS
#define BASE_HEAP_SIZE_FIELDS 24000000
#endif

#define RIB_NB_FIELDS 3

#if defined(BIGGER_HEAP_2)
#define HEAP_SIZE_FACTOR 2
#elif defined(BIGGER_HEAP_4)
#define HEAP_SIZE_FACTOR 4
#elif defined(BIGGER_HEAP_8)
#define HEAP_SIZE_FACTOR 8
#elif defined(BIGGER_HEAP_16)
#define HEAP_SIZE_FACTOR 16
#elif defined(BIGGER_HEAP_32)
#define HEAP_SIZE_FACTOR 32
#elif defined(BIGGER_HEAP_64)
#define HEAP_SIZE_FACTOR 64
#else
#define HEAP_SIZE_FACTOR 1
#endif

#ifndef HEAP_SIZE_FIELDS
#define HEAP_SIZE_FIELDS (BASE_HEAP_SIZE_FIELDS * HEAP_SIZE_FACTOR)
#endif

#ifndef HEAP_SIZE_BYTES
#define HEAP_SIZE_BYTES (HEAP_SIZE_FIELDS * sizeof(obj))
#endif

#define MAX_NB_OBJS (HEAP_SIZE_FIELDS / RIB_NB_FIELDS)

typedef struct ribs {
  obj fields[RIB_NB_FIELDS];
#ifdef TREADMILL
    struct ribs* li_next;
    struct ribs* li_prev;
#endif
} rib;

#ifdef TREADMILL
static inline void unlink(rib* object);
static inline void link(rib* object,rib* prev,rib* next);
static inline void write_barrier(rib* object,rib* new_ptr);
int do_not_collect = 0;
#define new_start white_end
rib* white_start = NULL;
rib* white_end = NULL;
rib* black_start = NULL;
rib* grey_start = NULL;
#define new_end grey_start
#define grey_end black_start
#define black_end white_start
// white_start --> white_end == new_start --> new_end == grey_start
// ^                                                         |
// |                                                         v
// white_start == black_end <----------- black_start == grey_end
//
#endif
#ifdef TREADMILL
#define WRITE_BARRIER(src,old,new) write_barrier((rib*)src,(rib*)new);
// TODO: change barriers so write happens in barrier
#define STACK_CHANGE_BARRIER(new_stack) \
    WRITE_BARRIER(root,CAR(root),new_stack); \
    CAR(root) = new_stack; 
#define PC_CHANGE_BARRIER(new_pc) \
    assert(IS_RIB(new_pc)); \
    WRITE_BARRIER(root,CDR(root),new_pc); \
    CDR(root) = new_pc; 
#define FALSE_CHANGE_BARRIER(new_false) \
    assert(IS_RIB(new_false)); \
    WRITE_BARRIER(root,TAG(root),new_false); \
    TAG(root) = new_false;
#else
#define WRITE_BARRIER(src,old,new) ;
#define STACK_CHANGE_BARRIER(new_stack) ;
#define PC_CHANGE_BARRIER(new_pc) ;
#define FALSE_CHANGE_BARRIER(new_false) ;
#endif

rib *heap_start;
#define SPACE_SZ (MAX_NB_OBJS * RIB_NB_FIELDS)
#define heap_bot ((obj *)(heap_start))
#define heap_top (heap_bot + (HEAP_SIZE_FIELDS))
#ifdef MARK_SWEEP
#define _NULL ((obj)NULL)
#else
obj *alloc_limit;
#define heap_mid (heap_bot + (HEAP_SIZE_FIELDS >> 1))
#endif
// end GC constants

#define EXIT_ILLEGAL_INSTR 6
#define EXIT_NO_MEMORY 7

#ifdef MARK_SWEEP
// If the mark and sweep GC is used, we need 2 bits: one to mark a live object
// and one to tag integers and ribs
#define UNTAG(x) ((x) >> 2)
#define TAG_NUM(num) ((((obj)(num)) << 2) | 1)
#define MARK(x) ((x)|2) // assumes x is a tagged obj (ul)
#define UNMARK(x) ((x)^2)
#define IS_MARKED(x) ((x)&2)
#define GET_MARK(o) (IS_MARKED(CDR(o)) + (IS_MARKED(TAG(o)) >> 1))
#else
#define UNTAG(x) ((x) >> 1)
#define TAG_NUM(num) ((((obj)(num)) << 1) | 1)
#endif
#define RIB(x) ((rib *)(x))
#define NUM(x) ((num)(UNTAG((num)(x))))
#define IS_NUM(x) ((x)&1)
#define IS_RIB(x) (!IS_NUM(x))
#define TAG_RIB(c_ptr) (((obj)(c_ptr)))

#define PRIM1() obj x = pop()
#define PRIM2()                                                                \
  obj y = pop();                                                               \
  PRIM1()
#define PRIM3()                                                                \
  obj z = pop();                                                               \
  PRIM2()

// WARNING : the CHECK_ACCESS macro should only be used with stop and copy GC
// CHECK_ACCESS will check if pointers are in the right space range when 
// accessing them with CAR, CDR or TAG
// #define CHECK_ACCESS
#ifdef CHECK_ACCESS
obj check_access(obj x){
  if (IS_NUM(x)){
    printf("ERROR ACCESSING NUMBER AS RIB %ld\n", NUM(x));
  }
  obj to_space_start = (obj)((alloc_limit == heap_mid) ? heap_mid : heap_bot);
  obj to_space_end   = (obj)(to_space_start + (HEAP_SIZE_FIELDS/2));

  if (to_space_start < x && x < to_space_end){
    printf("ERROR ACCESSING OUTSIDE SPACE %ld\n", NUM(x));
  }
  return x;
}

#define CAR(x) RIB(check_access(x))->fields[0]
#define CDR(x) RIB(check_access(x))->fields[1]
#define TAG(x) RIB(check_access(x))->fields[2]
#else
#define CAR(x) RIB(x)->fields[0]
#define CDR(x) RIB(x)->fields[1]
#define TAG(x) RIB(x)->fields[2]
#endif
#define TOS CAR(stack)

#define NUM_0 (TAG_NUM(0))

#define INSTR_AP 0
#define INSTR_SET 1
#define INSTR_GET 2
#define INSTR_CONST 3
#define INSTR_IF 4
#define INSTR_HALT 5

#define PAIR_TAG TAG_NUM(0)
#define CLOSURE_TAG TAG_NUM(1)
#define SYMBOL_TAG TAG_NUM(2)
#define STRING_TAG TAG_NUM(3)
#define SINGLETON_TAG TAG_NUM(5)

// the only three roots allowed
obj stack = NUM_0;
obj pc = NUM_0;
obj FALSE = NUM_0;

// global, but not a root, referenced
obj symbol_table = NUM_0;

size_t pos = 0;


#ifdef NO_STD
#define vm_exit(code)                                                          \
  do {                                                                         \
    asm volatile("mov $0x01, %%eax\nmov %0, %%ebx\nint $0x80" : : "i"(code));  \
  } while (0)
#else
#define vm_exit(code)                                                          \
  do {                                                                         \
    exit((code));                                                              \
  } while (0)
#endif

#if defined(NO_STD) && !defined(NO_REG)
register obj *alloc asm("edi");
#else
obj *alloc;
#endif
obj *alloc_limit;
obj *scan;

#ifdef NO_STD

void *sys_brk(void *addr) {
  long ptr;

  asm volatile("mov %0, %%ebx\n"
               "mov $0x2d, %%eax\n"
               "int $0x80\n"
               : "=a"(ptr)
               : "g"((long)addr)
               : "ebx");

  return (void *)ptr;
}

#endif

rib real_root;
rib *root = &real_root;
rib* initial_scan;
void init_heap() {
#ifdef NO_STD
  heap_start = sys_brk((void *)NULL);
  void *new_brk = sys_brk((void *)heap_top);

  if (new_brk == heap_start) {
    vm_exit(EXIT_NO_MEMORY);
  }

#else
#ifdef TREADMILL
  heap_start = malloc(sizeof(obj) * MAX_NB_OBJS*5);
#else
  heap_start = malloc(HEAP_SIZE_BYTES);
#endif

  if (!heap_start) {
    vm_exit(EXIT_NO_MEMORY);
  }
#endif
#ifdef TREADMILL
  if ((obj)heap_start & 7) {
      puts("unaligned heap start");
      exit(-1);
  }
  //root = heap_start;
  initial_scan = heap_start;
  new_start = heap_start;
  new_end = heap_start+5;
  for (int i=0;i <= 5;i++) {
      rib* prev = i > 0 ? heap_start + (i-1) : heap_start + 5;
      rib* next = i < 5 ? heap_start + (i+1) : heap_start;
      (heap_start+i)->li_prev = prev; 
      (heap_start+i)->li_next = next; 
      (heap_start+i)->fields[0] = 1;
      (heap_start+i)->fields[1] = 1;
      (heap_start+i)->fields[2] = 1;
      initial_scan++;
  }

  for (int i=1;i < 5;i++) {
     assert((heap_start+i)->li_next == heap_start+i+1);
     assert((heap_start+i)->li_prev == heap_start+i-1);
     assert((heap_start+i-1)->li_next == heap_start+i);
     assert((heap_start+i+1)->li_prev == heap_start+i);
  }

  assert(heap_start->li_prev == heap_start+5);
  assert((heap_start+5)->li_next == heap_start);
  white_start = white_end = new_start;
  grey_end = grey_start;

  root->li_next = root; 
  root->li_prev = root;

  CAR(root) = (obj)stack;
  CDR(root) = (obj)pc;
  TAG(root) = (obj)FALSE;
#endif

#ifdef MARK_SWEEP
  // initialize freelist
  scan = heap_top;
  // scan -= RIB_NB_FIELDS;
  *scan = _NULL;
  
  while (scan != heap_bot) {
    alloc = scan; // alloc <- address of previous slot
    scan -= RIB_NB_FIELDS; // scan <- address of next rib slot
    *scan = (obj)alloc; // CAR(next rib) <- address of previous slot
  }
  alloc = scan;
#else
  alloc = heap_bot;
  alloc_limit = heap_mid;
#endif
  STACK_CHANGE_BARRIER(NUM_0);
  stack = NUM_0;
}

// GC algorithms
#ifdef MARK_SWEEP

#ifdef MARK_SWEEP_DSW
void mark(obj *o) {
  // Deutsch-Schorr-Waite algorithm for marking phase
  if (IS_RIB(*o)) {
    
    obj curr = *o;
    obj tmp;
    obj prev = _NULL;

    if (curr == stack) { // initialize descend for TOS
      prev = curr;
      curr = CDR(prev);
      CDR(prev) = _NULL;
      CDR(prev) = MARK(CDR(prev));
    }

  forward:
    if (curr == _NULL) {
      TAG(prev) = MARK(TAG(prev));
      goto backward;
    }
    if (IS_RIB(curr) && (GET_MARK(curr) == 0)) { // car descend
      TAG(curr) = MARK(TAG(curr)); // mark = 01
      tmp = curr;
      curr = CAR(curr);
      CAR(tmp) = prev;
      prev = tmp;
      goto forward;
    }
    
  backward:
    if (prev != _NULL) {
     
      switch(GET_MARK(prev)) {

        // cdr descend
        case 1:
          TAG(prev) = UNMARK(TAG(prev)); // mark will be 10 so unmark tag
          tmp = CAR(prev); 
          CAR(prev) = curr; 
          curr = CDR(prev); 
          CDR(prev) = tmp; 
          CDR(prev) = MARK(CDR(prev)); // mark = 10
          goto forward;
          
        // tag descend
        case 2:
          tmp = UNMARK(CDR(prev)); 
          CDR(prev) = curr; 
          CDR(prev) = MARK(CDR(prev)); // mark will be 11
          curr = TAG(prev); // shouldn't be marked so no need to unmark
          TAG(prev) = tmp;
          TAG(prev) = MARK(TAG(prev)); // mark = 11
          goto forward;

        // retreat... only 3rd field will remain marked (01) so that we can
        // re-use the same logic in the GC function as with the recursive version
        case 3:
          tmp = UNMARK(TAG(prev));
          TAG(prev) = curr; 
          TAG(prev) = MARK(TAG(prev)); // want for 3rd field to stay tagged
          CDR(prev) = UNMARK(CDR(prev)); // mark = 01
          curr = prev;
          prev = tmp;
          goto backward;
      } 
    }
  }
}

#else

void mark(obj *o) { // Recursive version of marking phase
  obj z = *o;
  while (IS_RIB(z)) {
    obj *ptr = RIB(z)->fields;
    if (!IS_MARKED(ptr[1])) { 
      obj tmp = ptr[1]; 
      ptr[1] = MARK(ptr[1]);
      mark(&ptr[0]);
      mark(&ptr[2]);
      z = tmp;
    } else {
      break;
    }
  }
}

#endif

void gc() {
#ifdef DEBUG_GC
  printf("\t--GC called\n");
#endif
  // Mark (only 3 possible roots)
  mark(&stack);
  mark(&pc);
  mark(&FALSE);
  // Sweep
  scan=heap_bot;
  while (scan != heap_top) {
    obj tag = *(scan+1);
    if (IS_MARKED(tag)) {
      *(scan+1) = UNMARK(tag);
    } else {
      *scan = (obj)alloc;
      alloc = scan;
    }
    scan += RIB_NB_FIELDS; // next rib object
  }
  if (*alloc == _NULL){
    printf("Heap is full\n");
    exit(2);
  }
}

#else

// Cheney style stop & copy GC

// NULL is a pointer (0) but would represent NULL
// so it is never present in an obj field, and
// cannot be a number because it is even. This
// saves a couple of bytes v.s having STACK
// as the broken heart value
#define GC_COPIED_OBJ ((obj)NULL)

void copy() {
  obj o = *scan;
  // we sometime reference rib that are allocated in BSS,
  // we do not want to copy those
  if (IS_RIB(o)) {
    obj *ptr = RIB(o)->fields;
    obj field0 = ptr[0];
    obj copy;

    if (field0 == GC_COPIED_OBJ) {
      copy = ptr[1]; // copied, get new address
    } else {
      copy = TAG_RIB(alloc);
      *ptr++ = GC_COPIED_OBJ; // ptr points to CDR
      *alloc++ = field0;
      *alloc++ = *ptr++; // ptr points to TAG
      *alloc++ = *ptr;
      ptr[-1] = copy; // set forward ptr. Since it points to TAG, ptr[-1]
                      // rewrites the CDR
    }
    *scan = copy; // overwrite to new address.
  }
  scan++;
}

void gc() {
#ifdef DEBUG_GC
  obj *from_space = (alloc_limit == heap_mid) ? heap_bot : heap_mid;

  size_t objc = alloc - from_space;
  printf("\t--GC %zu -> ", objc);
#endif

  // swap
  obj *to_space = (alloc_limit == heap_mid) ? heap_mid : heap_bot;
  alloc_limit = to_space + (HEAP_SIZE_FIELDS/2);

  alloc = to_space;

  // root: stack
  scan = &stack;
  copy();

  // root: pc
  scan = &pc;
  copy();

  // root: false
  scan = &FALSE;
  copy();

  // scan the to_space to pull all live references
  scan = to_space;
  while (scan != alloc && alloc != alloc_limit) {
    copy();
  }

  if (alloc == alloc_limit) {
    printf("Error: out of memory");
    exit(2); // out of memory
  }

#ifdef DEBUG_GC

  objc = alloc - to_space;
  printf("%zu\n", objc);
  fflush(stdout);

#endif
}
#endif // end of GC algorithms
#ifdef TREADMILL
#define GC_STARTED 1
#define IS_LIST_END(object) (((obj)object == (obj)&new || (obj)object == (obj)&black || (obj)object == (obj)&grey || (obj)object == (obj)&white))
#define TR_UNTAG(arg) ( (rib*) ((obj)arg & ~7))
#define PTR_1(obj) ((rib*)obj->fields[0])
#define PTR_2(obj) ((rib*)obj->fields[1])
#define PTR_3(obj) ((rib*)obj->fields[2])
#define TG_WHITE(x) (rib*)tagp((obj) x,1,flipped)
#define TG_BLACK(x) (rib*)tagp((obj)x,1,1 - flipped)
#define TAG_GREY(x) (rib*)tagp((obj)x,0,1)
#define UNTAG_GREY(x) (rib*)tagp((obj)x,0,0)
#define TAG_WHITE(x) TG_WHITE(UNTAG_GREY(x))
#define TAG_BLACK(x) TG_BLACK(UNTAG_GREY(x))
#define IS_GREY(x) is_grey((obj)x)
#define IS_BLACK(x) is_black((obj)x)
#define IS_WHITE(x) is_white((obj)x)
#define TAG_BITS 3

#define GREY 1
#define BLACK_WHITE 2
#define WHITE_BLACK 0
int free_alloc = 2;


int flags;
int  flipped = 0;
static inline int is_grey(obj x) {
    return x & GREY;
}
static inline int is_black(obj x) {
    return (!is_grey(x) && ((x & 2) ^ (flipped << 1)));
}
static inline int is_white(obj x) {
    return !is_grey(x) && !is_black(x);
}

static inline long tagp(obj input,obj offset,char value) {
    if (value) {
        input |= (1 << offset);
    } else {
        input &= ~(1 << offset);
    }
    return input;
}

static inline void write_barrier(rib* object,rib* new_ptr) {
  if (IS_NUM((obj)new_ptr)) return;
  if (!IS_BLACK(object->li_next)) return; // source is not black
  if (!IS_WHITE(new_ptr->li_next)) return; //dst is black
  rib* next = new_ptr;
  unlink(next);
  next->li_next = TAG_GREY(grey_start->li_next);
  link(next,grey_start,TR_UNTAG(grey_start->li_next));
}

static inline void unlink(rib* object) {
    assert(((obj) object & 7) == 0);
    TR_UNTAG(object->li_next)->li_prev = object->li_prev;
    obj tag = (obj)object->li_prev->li_next & 7;
    object->li_prev->li_next = (rib*)((obj)TR_UNTAG(object->li_next) | tag);
}

static inline void link(rib* object,rib* prev,rib* next) {
    assert(((obj) object & 7) == 0);
    assert(((obj) prev & 7) == 0);
    assert(((obj) next & 7) == 0);
    assert(object->fields[0] != 0);
    assert(object->fields[1] != 0);
    assert(object->fields[2] != 0);
    assert(prev->fields[0] != 0);
    assert(prev->fields[1] != 0);
    assert(prev->fields[2] != 0);
    assert(next->fields[0] != 0);
    assert(next->fields[1] != 0);
    assert(next->fields[2] != 0);
    obj tag = (obj)object->li_next & 7;
    obj tag2 = (obj)prev->li_next & 7;
    object->li_next = (rib*)((obj)TR_UNTAG(next) | tag); 
    object->li_prev = prev;
    prev->li_next = (rib*)((obj)object | tag2);
    next->li_prev = object;
}
static inline void check_color(rib* object) {
    rib* scan = NULL;
    rib* end = NULL;
    if (IS_GREY(object->li_next)) {
         scan = grey_start;
         end = grey_end;
    } else if (IS_BLACK(object->li_next)) {
         scan = black_start;
         end = black_end;
    } else if (IS_WHITE(object->li_next)) {
         scan = white_start;
         end = white_end;
    }
    int found = 0;
    while (scan != end) {
        if (object == scan) {
            found = 1;
            return;
        }
        scan = TR_UNTAG(scan->li_next);
    }
    if (found == 0 && end != grey_end) {
        scan = new_start;
        end = new_end;
        while (scan != end) {
            if (object == scan) {
                found = 1;
                return;
            }
            scan = TR_UNTAG(scan->li_next);
        }
    }
    assert(found == 1);
}
    
static inline void rt_gc() {
  if (do_not_collect) return;
  assert(!IS_GREY(white_start));
  assert(!IS_GREY(white_end));

  if (grey_start == grey_end && (flags & GC_STARTED )== 0) {
    //puts("start");
    unlink(root);
    assert(grey_start != 0);
    assert(!IS_GREY(grey_start->li_next));
    link(root,grey_start,TR_UNTAG(grey_start->li_next));
    assert(TR_UNTAG(grey_start->li_next) == root);
    grey_end = TR_UNTAG(root->li_next);
    root->li_next = TAG_GREY(TR_UNTAG(root->li_next));
    assert(IS_GREY(root->li_next));
    grey_start = root;
    flags |= GC_STARTED;
  }

  if (grey_start == grey_end) {
    assert(!IS_GREY(grey_start->li_next));
#ifdef DEBUG_GC
    //puts("collect");
    rib* scan = white_start;
    while (scan != white_end) {
        assert(IS_WHITE(scan->li_next));
        scan = TR_UNTAG(scan->li_next);
    }
    scan = black_start;
    while (scan != black_end) {
        assert(IS_BLACK(scan->li_next));
        scan = TR_UNTAG(scan->li_next);
    }
#endif
    new_start = white_start;
    flipped = 1 - flipped;
    white_start = black_start;
    flags &= ~GC_STARTED;
    rib* object = new_start;
#ifdef DEBUG_GC
    while (object != new_end) {
        assert(object != root);
        object->fields[0] = 0x6161616161616162;
        object->fields[1] = 0x6161616161616162;
        object->fields[2] = 0x6161616161616162;
        object = TR_UNTAG(object->li_next);
    }
#endif
  } else {
    assert(IS_GREY(grey_start->li_next));
    rib* object = grey_end->li_prev;
    assert(object != white_end->li_prev);
    assert(object != white_start);
    assert(IS_GREY(object->li_next));

    if (!IS_NUM((obj)PTR_1(object)) && IS_WHITE(PTR_1(object)->li_next)) {
        rib* next = PTR_1(object);
        if (white_start == white_end && white_end == next) {
            white_start = white_start->li_prev;
            white_end = white_end->li_prev;
        }
        else if (white_start == next && white_start != white_end) {
            white_start = TR_UNTAG(white_start)->li_next;
            // puts("white list problems avoided");
        }
        else if (white_end == next && white_start != white_end) {
           white_end = white_end->li_prev;
           // puts("white list problems avoided");
        }
        assert(IS_WHITE(next));
        unlink(next);
        grey_start = grey_start->li_prev;
        assert(!IS_GREY(grey_start->li_next));
        link(next,grey_start,TR_UNTAG(grey_start->li_next));
        assert(!IS_GREY(grey_start->li_next));
        grey_start = TR_UNTAG(grey_start->li_next);
        next->li_next = TAG_GREY(TR_UNTAG(next->li_next));
        assert(grey_end->li_prev == object);
        assert(IS_GREY(PTR_1(object)->li_next));
    }

    if (!IS_NUM((obj)PTR_2(object)) && IS_WHITE(PTR_2(object)->li_next)) {
        rib* next = PTR_2(object);
        assert(IS_WHITE(next));

        if (white_start == white_end && white_end == next) {
            white_start = white_start->li_prev;
            white_end = white_end->li_prev;
        }
        else if (white_start == next && white_start != white_end) {
            white_start = TR_UNTAG(white_start)->li_next;
            // puts("white list problems avoided");
        }
        else if (white_end == next && white_start != white_end) {
           white_end = white_end->li_prev;
           // puts("white list problems avoided");
        }
        unlink(next);
        grey_start = grey_start->li_prev;
        assert(!IS_GREY(grey_start->li_next));
        link(next,grey_start,TR_UNTAG(grey_start->li_next));
        assert(!IS_GREY(grey_start->li_next));
        next->li_next = TAG_GREY(TR_UNTAG(next->li_next));
        grey_start = TR_UNTAG(grey_start->li_next);
        assert(grey_end->li_prev == object);
        assert(IS_GREY(PTR_2(object)->li_next));
    }

    if (!IS_NUM((obj)PTR_3(object)) && IS_WHITE(PTR_3(object)->li_next)) {
        rib* next = PTR_3(object);
        if (white_start == white_end && white_end == next) {
            white_start = white_start->li_prev;
            white_end = white_end->li_prev;
        }
        else if (white_start == next && white_start != white_end) {
            white_start = TR_UNTAG(white_start)->li_next;
            // puts("white list problems avoided");
        }
        else if (white_end == next && white_start != white_end) {
           white_end = white_end->li_prev;
           // puts("white list problems avoided");
        }
        assert(IS_WHITE(next));
        unlink(next);
        grey_start = grey_start->li_prev;
        assert(!IS_GREY(grey_start->li_next));
        link(next,grey_start,TR_UNTAG(grey_start->li_next));
        assert(!IS_GREY(grey_start->li_next));
        next->li_next = TAG_GREY(TR_UNTAG(next->li_next));
        grey_start = TR_UNTAG(grey_start->li_next);
        assert(grey_end->li_prev == object);
        assert(IS_GREY(PTR_3(object)->li_next));
    }

    object->li_next = TAG_BLACK(TR_UNTAG(object->li_next)); 
    grey_end = grey_end->li_prev;
    assert(black_start == object);
    assert(IS_BLACK(object->li_next));

    assert(!IS_GREY(white_start));
    assert(!IS_GREY(white_end));
  }
}


#endif

obj pop() {
  obj x = CAR(stack);
  STACK_CHANGE_BARRIER(CDR(stack));
  stack = CDR(stack);
  return x;
}

void push2(obj car, obj tag) {
#ifdef TREADMILL
    if (initial_scan < heap_start + MAX_NB_OBJS) {
        if (new_start == new_end) {
            puts("empty before");
        }
        rib* start = initial_scan;
        start->li_next = 0;
        start->li_prev = 0;
        start->fields[0] = 0x6161616161616161;
        start->fields[1] = 0x6161616161616161;
        start->fields[2] = 0x6161616161616161;
        link(start,new_start,TR_UNTAG(new_start->li_next));
        if (new_start == new_end) {
      //      puts("empty");
            new_end = TR_UNTAG(new_start->li_next);
        }
        initial_scan++;
    } else if (new_start == new_end) { // list is empty
       // puts("out of real time");
        for (int i=0; i < HEAP_SIZE_FIELDS && new_start == new_end && do_not_collect == 0;i++) {
            rt_gc();
        }
        if (new_start == new_end) {
            printf("do_not_collect = %d\n",do_not_collect);
            puts("out of memory");
            exit(-1);
        }
    }// else puts("normal alloc");
    rib* result = new_start;
    assert(result != root);
    assert(IS_GREY(grey_start->li_next) || grey_start == grey_end);
    result->li_next = TAG_WHITE(TR_UNTAG(result->li_next));
    new_start = TR_UNTAG(new_start->li_next);
    assert(new_start->li_prev != new_end);
    result->fields[0] = car;
    result->fields[1] = stack;
    result->fields[2] = tag;
    STACK_CHANGE_BARRIER(TAG_RIB(result));
    stack = TAG_RIB(result);
    if (free_alloc > 0 || initial_scan < heap_start + MAX_NB_OBJS) {
        free_alloc--;
    } else {
        rt_gc();
    }
#else
#ifdef MARK_SWEEP
  obj tmp = *alloc; // next available slot in freelist
#endif  
  // default stack frame is (value, ->, NUM_0)
  *alloc++ = car;
  *alloc++ = stack;
  *alloc++ = tag;
  stack = TAG_RIB((rib *)(alloc - RIB_NB_FIELDS));
  
#ifdef MARK_SWEEP
  alloc = (obj *)tmp;
  if (!IS_RIB(tmp) || *alloc == _NULL) { // empty freelist?
    gc();
  }
#else
  if (alloc == alloc_limit) {
    gc();
  }
#endif
#endif
}

// Simple version of push for stack operations
static inline void push(obj car){
  push2(car, PAIR_TAG);
}

/**
 * Allocate a rib that is not kept on the stack (can be linked
 * from anywhere). The car and cdr can be live references to other
 * ribs.
 * @param car
 * @param cdr
 * @param tag
 * @return
 */
rib *alloc_rib(obj car, obj cdr, obj tag) {
  push2(car, cdr); // tag is set
  obj old_stack = CDR(stack);
  obj allocated = stack;

  WRITE_BARRIER(allocated,CDR(allocated),TAG(allocated));
  CDR(allocated) = TAG(allocated);
  WRITE_BARRIER(allocated,TAG(allocated),tag);
  TAG(allocated) = tag;

  STACK_CHANGE_BARRIER(old_stack);
  stack = old_stack;

  return RIB(allocated);
}

rib *alloc_rib2(obj car, obj cdr, obj tag) {
  push2(car, tag); // tag is set
  obj old_stack = CDR(stack);
  obj allocated = stack;

  WRITE_BARRIER(allocated,CDR(allocated),cdr);
  CDR(allocated) = cdr;

  STACK_CHANGE_BARRIER(old_stack);
  stack = old_stack;

  return RIB(allocated);
}

unsigned char get_byte() { return input[pos++]; }



#define ENCODING_SIZE  (256)                // @@(replace "256" encoding/ribn-base)@@
#define HALF_ENCODING_SIZE ENCODING_SIZE/2

#if ENCODING_SIZE==92
#define GET_CODE get_code
num get_code() {
  num x = get_byte() - 35;
  return x < 0 ? 57 : x;
}
#else
#define GET_CODE get_byte
#endif

num get_int(num n) {
  num x = GET_CODE();
  n *= HALF_ENCODING_SIZE;
  return x < HALF_ENCODING_SIZE ? n + x : get_int(n + x - HALF_ENCODING_SIZE);
}

rib *list_tail(rib *lst, num i) {
  return (i == 0) ? lst : list_tail(RIB(lst->fields[1]), i - 1);
}

rib *inst_tail(rib *lst, num i){
  return (i == 0) ? lst : inst_tail(RIB(lst->fields[2]), i - 1);
}

obj list_ref(rib *lst, num i) { return list_tail(lst, i)->fields[0]; }

obj get_opnd(obj o) {
  return (IS_NUM(o) ? RIB(list_tail(RIB(stack), NUM(o))) : RIB(o))->fields[0];
}

obj get_cont() {
  obj s = stack;

  while (!NUM(TAG(s))) {
    s = CDR(s);
  }

  return s;
}

#define TRUE (CAR(FALSE))
#define NIL (CDR(FALSE))
// Temp values that can be used to shield 
//  pointers from the evil GC
#define TEMP1 CAR(TRUE)
#define TEMP2 CDR(TRUE)
#define TEMP3 CAR(NIL)
#define TEMP4 CDR(NIL)

#ifdef DEBUG

void chars2str(obj o) {
  if (o != NIL) {
    printf("%c", (char)(NUM(RIB(o)->fields[0]) % 256));
    chars2str(RIB(o)->fields[1]);
  }
}

void sym2str(rib *c) { chars2str(RIB(c->fields[1])->fields[0]); }

void show_operand(obj o) {
  if (IS_NUM(o)) {
    printf("int %ld", NUM(o));
  } else if (TAG(o) == SYMBOL_TAG) {
    printf("sym ");
    sym2str(RIB(o));
  }
}

#endif





// @@(feature bool2scm 
obj bool2scm(bool x) { return x ? CAR(FALSE) : FALSE; }
// )@@

obj prim(int no) {
  switch (no) { 
case 0:  {
    obj new_rib = TAG_RIB(alloc_rib(NUM_0, NUM_0, NUM_0));
    PRIM3();
    WRITE_BARRIER(new_rib,CAR(new_rib),x);
    CAR(new_rib) = x;
    WRITE_BARRIER(new_rib,CDR(new_rib),y);
    CDR(new_rib) = y;
    WRITE_BARRIER(new_rib,TAG(new_rib),z);
    TAG(new_rib) = z;
    push2(new_rib, PAIR_TAG);
    break;
    
  } // )@@
case 1:{
  PRIM2();
  FILE* file = (FILE*) ((long) y ^ 1);
  char buffer[1] = {(char) NUM(x)};
  int success = fwrite(buffer, 1, 1, file);
  if (success != 1) {
  perror("Cannot write to file.");
  }
  fflush(file);
  push2(TRUE, PAIR_TAG);
  break;
}
case 2:{
      FILE* file = fdopen(1, "w");
      push2((long) file | 1, PAIR_TAG);
      break;
    }
case 3:{
      PRIM2();
      TEMP1 = x; // save x for the gc 
      int num_args = 0;
      obj arg = TAG_RIB(y);
      while (arg != NIL) {
         push2(arg, PAIR_TAG); // make sure the arg doesn't get GC'd
         arg = CAR(stack);
         CAR(stack) = CAR(arg);
         arg = TAG_RIB(CDR(arg));
         num_args++;
      }
      push2(TAG_NUM(num_args), PAIR_TAG);
      x = TEMP1; // retrive x from possibly GC'd 
      return TAG_RIB(x);
     }
case 4:  {
    PRIM1();
    vm_exit(NUM(x));
    break;
  } // )@@
case 5:  {
    PRIM2();
    push2(TAG_NUM((NUM(x) / NUM(y))), PAIR_TAG);
    break;
  } // )@@
case 6:  {
    PRIM2();
    push2(TAG_NUM((NUM(x) * NUM(y))), PAIR_TAG);
    break;
  } // )@@
case 7:  {
    PRIM2();
    push2(x - y + 1, PAIR_TAG);
    break;
  } //)@@
case 8:  {
    PRIM2();
    push2(x + y - 1, PAIR_TAG);
    break;
  } //)@@
case 9:  {
    PRIM2();
    push2(bool2scm(NUM(x) < NUM(y)), PAIR_TAG);
    break;
  } //)@@
case 10:  {
    PRIM2();
    push2(bool2scm(x == y), PAIR_TAG);
    break;
  } //)@@
case 11:  {
    PRIM1();
    push2(TAG(x), PAIR_TAG);
    break;
  } //)@@
case 12:  {
    PRIM1();
    push2(CDR(x), PAIR_TAG);
    break;
  } //)@@
case 13:  {
    PRIM1();
    push2(CAR(x), PAIR_TAG);
    break;
  } //)@@
case 14:  {
    PRIM1();
    push2(bool2scm(IS_RIB(x)), PAIR_TAG);
    break;
  } //)@@
case 15:  {
    obj x = CAR(TOS);
    obj y = CDR(stack);
    obj z = TAG_RIB(alloc_rib(x, y, CLOSURE_TAG));
    WRITE_BARRIER(stack,CAR(stack),z);
    TOS = z;
    break;
  } //)@@
case 16:  {
    obj x = pop();
    pop();
    push2(x, PAIR_TAG);
    break;
  } //)@@
case 17:  {
    pop();
    break;
  } // )@@
case 18:  {
    PRIM1();
    push2(x, PAIR_TAG);
    break;
  } // )@@
  default: {
    vm_exit(EXIT_ILLEGAL_INSTR);
  }
  }
  return TAG_NUM(0);
}

#ifdef DEBUG
void show_rib(obj s, int depth){
    if (depth < 0){
        if (IS_RIB(s)){
            printf("[...]");
        }
        else{
            printf("...");
        }
        return;
    }
    if (IS_RIB(s)){
      if (TAG(s) == SINGLETON_TAG){
        if (s == NIL) { printf("'()"); }
        if (s == TRUE) { printf("#t"); }
        if (s == FALSE) { printf("#f"); }
      }
      else{
        printf("[");
        show_rib(CAR(s), depth-1);
        printf(", ");
        show_rib(CDR(s), depth-1);
        printf(", ");
        show_rib(TAG(s), depth-1);
        printf("]");
      }
    }
    else{
        printf("%d", NUM(s));
    }
}

void show_stack(){
    obj itr = stack;
    PRINTLN();
    if (NUM(TAG(itr))){
        printf("[]");
        return;

    }
    printf("[ ");
    int first = 0;
    while(!NUM(TAG(itr))){
        if (first){
            printf(", ");
        }
        else{
            first = 1;
        }
        show_rib(CAR(itr), 0);
        itr = CDR(itr);
    }
    printf(" ]");

}

#endif

void run() {
#define ADVANCE_PC()                                                           \
  do {                                                                         \
    PC_CHANGE_BARRIER(TAG(pc)) \
    pc = TAG(pc);                                                              \
  } while (0)
  while (1) {
    num instr = NUM(CAR(pc));
    switch (instr) {
    default: { // error
      vm_exit(EXIT_ILLEGAL_INSTR);
    }
    case INSTR_HALT: { // halt
      vm_exit(0);
    }
    case INSTR_AP: // call or jump
    {
      bool jump = TAG(pc) == NUM_0;
#ifdef DEBUG_I_CALL
      printf(jump ? "--- jump " : "--- call ");
      show_operand(CDR(pc));
      show_stack();
      PRINTLN();
#endif
      obj proc = get_opnd(CDR(pc));
      while (1) {
#define code CAR(proc)
          if (IS_NUM(code)) {
            pop(); // @@(feature (and arity-check (not prim-no-arity)))@@
            proc=prim(NUM(code));

            if (IS_RIB(proc)) continue;

            if (jump) {
              // jump
              obj cont = get_cont();
              PC_CHANGE_BARRIER(cont);
              pc = cont;
              WRITE_BARRIER(stack,CDR(stack),CAR(pc));
              CDR(stack) = CAR(pc);
            }
            PC_CHANGE_BARRIER(TAG(pc));
            pc = TAG(pc);
          } else {
            num nargs = NUM(pop()); // @@(feature arity-check)@@
            obj s2 = TAG_RIB(alloc_rib(NUM_0, proc, PAIR_TAG));
            proc = CDR(s2);
            WRITE_BARRIER(s2,CDR(s2),CDR(proc));
            WRITE_BARRIER(pc,CAR(pc),CAR(proc));
            CAR(pc) = CAR(proc); // save the proc from the mighty gcrvm.c


            num nparams_vari = NUM(CAR(code));
            num nparams = nparams_vari >> 1;
            // @@(feature arity-check
            num vari = nparams_vari&1;
            if (vari ? nparams > nargs : nparams != nargs) {
                printf("*** Function called with an unexpected number of arguments\n");
                printf("-> Number of expected argument by the function: %ld\n", nparams);
                printf("-> Number of applied argument by the caller: %ld\n", nargs);
#ifdef DEBUG
                printf("-> Function symbol or location on stack:");
                show_operand(CDR(pc));
                printf("\n");
#endif
                exit(1);
            }
            // )@@
            // @@(feature rest-param (use arity-check)
            nargs-=nparams;
            if (vari){
                obj rest = NIL;
                for(int i = 0; i < nargs; ++i){
                    WRITE_BARRIER(TRUE,TEMP1,s2);
                    TEMP1 = s2;
                    rest = TAG_RIB(alloc_rib(pop(), rest, PAIR_TAG));
                    s2=TEMP1;
                }
                s2 = TAG_RIB(alloc_rib(rest, s2, PAIR_TAG));
            }
            // )@@

            for (int i = 0; i < nparams; ++i) {
              s2 = TAG_RIB(alloc_rib(pop(), s2, PAIR_TAG));
            }

            nparams = nparams + vari; // @@(feature arity-check)@@

            obj c2 = TAG_RIB(list_tail(RIB(s2), nparams));

            if (jump) {
              obj k = get_cont();
              WRITE_BARRIER(c2,CAR(c2),CAR(k));
              CAR(c2) = CAR(k);
              WRITE_BARRIER(c2,TAG(c2),TAG(k));
              TAG(c2) = TAG(k);
            } else {
              WRITE_BARRIER(c2,CAR(c2),stack);
              CAR(c2) = stack;
              WRITE_BARRIER(c2,TAG(c2),TAG(pc));
              TAG(c2) = TAG(pc);
            }

            STACK_CHANGE_BARRIER(s2);
            stack = s2;

            obj new_pc = CAR(pc);
            CAR(pc) = TAG_NUM(instr);
            PC_CHANGE_BARRIER(TAG(new_pc));
            pc = TAG(new_pc);
          }
          break;
      }
      break;
    }
#undef code
    case INSTR_SET: { // set
#ifdef DEBUG_I_CALL
      printf("--- set ");
      show_operand(CDR(pc));
      show_stack();
      PRINTLN();
#endif
      obj x = CAR(stack);
      rib* src = ((IS_NUM(CDR(pc))) ? list_tail(RIB(stack), NUM(CDR(pc))) : RIB(CDR(pc)));
      WRITE_BARRIER(src,src->fields[0],x);
      src->fields[0] = x;
      STACK_CHANGE_BARRIER(CDR(stack));
      stack = CDR(stack);
      ADVANCE_PC();
      break;
    }
    case INSTR_GET: { // get
#ifdef DEBUG_I_CALL
      printf("--- get ");
      show_operand(CDR(pc));
      show_stack();
      PRINTLN();
#endif
      push2(get_opnd(CDR(pc)), PAIR_TAG);
      ADVANCE_PC();
      break;
    }
    case INSTR_CONST: { // const
#ifdef DEBUG_I_CALL
      printf("--- const ");
      show_operand(CDR(pc));
      show_stack();
      PRINTLN();
#endif
      push2(CDR(pc), PAIR_TAG);
      ADVANCE_PC();
      break;
    }
    case INSTR_IF: { // if
#ifdef DEBUG_I_CALL
      printf("--- if");
      show_stack();
      PRINTLN();
#endif

      obj p = pop();
      if (p != FALSE) {
        PC_CHANGE_BARRIER(CDR(pc));
        pc = CDR(pc);
      } else {
        PC_CHANGE_BARRIER(TAG(pc));
        pc = TAG(pc);
      }
      break;
    }
    }
  }
#undef ADVANCE_PC
}

rib *symbol_ref(num n) { return RIB(list_ref(RIB(symbol_table), n)); }

obj lst_length(obj list) {
  size_t l = 0;

  while (IS_RIB(list) && NUM(TAG(list)) == 0) {
    ++l;
    list = CDR(list);
  }

  return TAG_NUM(l);
}

rib *create_sym(obj name) {
  rib *list = alloc_rib(name, lst_length(name), STRING_TAG);
  rib *sym = alloc_rib(FALSE, TAG_RIB(list), SYMBOL_TAG);
  rib *root = alloc_rib(TAG_RIB(sym), symbol_table, PAIR_TAG);
  return root;
}

void build_sym_table() {
  num n = get_int(0);

  while (n > 0) {
    n--;
    symbol_table = TAG_RIB(create_sym(NIL));
  }

  obj accum = NIL;

  while (1) {
    byte c = get_byte();
    if (c == 44) {
      symbol_table = TAG_RIB(create_sym(accum));
      accum = NIL;
      continue;
    }

    if (c == 59)
      break;

    accum = TAG_RIB(alloc_rib(TAG_NUM(c), TAG_RIB(accum), PAIR_TAG));
  }

  symbol_table = TAG_RIB(create_sym(accum));
}

void set_global(obj c) {
  WRITE_BARRIER(CAR(symbol_table),CAR(CAR(symbol_table)),c);
  CAR(CAR(symbol_table)) = c;
  symbol_table = CDR(symbol_table);
}


// @@(feature encoding/optimal
void decode() {
  int weights[] = {0,1,58,1,5,1,30,1,4,1,0,1,7,1,4,1,19,1,0,1,7,1,5,1,1}; // @@(replace "{0,1,58,1,5,1,30,1,4,1,0,1,7,1,4,1,19,1,0,1,7,1,5,1,1}" (list->host encoding/optimal/start "{" "," "}"))@@

  obj n;
  int d;
  int op;
  int i;

  while (1) {
    num x = GET_CODE();
    n = x;
    op = -1;

    while((d=weights[++op])<=n) n-=d;

    if (op < 4) push2(NUM_0, NUM_0); // JUMP
    if (op < 24) n = op%2> 0 ? get_int(n):n; 


    if (op < 20) {
      i = (op / 4) - 1;
      i = i < 0?0:i;
      n = !(op & 0b10)  ? TAG_NUM(n) : TAG_RIB(symbol_ref(n));
    }
    else if (op < 22) {
      n = TAG_RIB(alloc_rib(TAG_RIB(alloc_rib2(TAG_NUM(n), NUM_0, pop())), NIL, CLOSURE_TAG));
      i = 3;
      if (stack == NUM_0) break;
    }
    else if (op < 24){
      push2(TAG_RIB(inst_tail(RIB(TOS), n)), NUM_0);
      continue;
    }
    else if (op < 25){
      n = pop();
      i=4;
    }


    rib *c = alloc_rib(TAG_NUM(i), n, NUM_0);
    WRITE_BARRIER(c,c->fields[2],TOS);
    c->fields[2] = TOS;
    WRITE_BARRIER(stack,TOS,TAG_RIB(c));
    TOS = TAG_RIB(c);
  }

  PC_CHANGE_BARRIER(TAG(CAR(n)));
  pc = TAG(CAR(n));
}
// )@@


void setup_stack() {
  push2(NUM_0, PAIR_TAG);
  push2(NUM_0, PAIR_TAG);

  obj first = CDR(stack);
  CDR(stack) = NUM_0;
  WRITE_BARRIER(stack,TAG(stack),first);
  TAG(stack) = first;

  WRITE_BARRIER(first,CAR(first),TAG_NUM(INSTR_HALT));
  CAR(first) = TAG_NUM(INSTR_HALT);
  WRITE_BARRIER(first,CDR(first),NUM_0);
  CDR(first) = NUM_0;
  WRITE_BARRIER(first,TAG(first),PAIR_TAG);
  TAG(first) = PAIR_TAG;
}

#ifdef NOSTART
void _start() {
#else

void init() {
#endif
  init_heap();
#ifdef TREADMILL
  do_not_collect = 1;
#endif
  obj new_false = TAG_RIB(alloc_rib(TAG_RIB(alloc_rib(NUM_0, NUM_0, SINGLETON_TAG)),
                            TAG_RIB(alloc_rib(NUM_0, NUM_0, SINGLETON_TAG)),
                            SINGLETON_TAG));
  FALSE_CHANGE_BARRIER(new_false);
  FALSE = new_false;

  build_sym_table();
  decode();
#ifdef TREADMILL
  do_not_collect = 0;
#endif

  set_global(
      TAG_RIB(alloc_rib(NUM_0, symbol_table, CLOSURE_TAG))); /* primitive 0 */
  set_global(FALSE);
  set_global(TRUE);
  set_global(NIL);

  setup_stack();
  run();
}

#ifdef ARG_V

int main(int _argc, char* _argv[]) { 
    argc=_argc;
    argv=_argv;
    init(); 
}

#else

int main() { init(); }


#endif

#pragma clang diagnostic pop
