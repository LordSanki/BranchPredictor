#ifndef __CUSTOM_TYPES_H__
#define __CUSTOM_TYPES_H__

typedef unsigned int ui32;
typedef signed int i32;
typedef unsigned long int ui64;
typedef signed long int i64;
typedef float f32;
typedef double f64;
typedef signed int i16;
typedef unsigned int ui16;
typedef signed char i8;
typedef unsigned char ui8;
typedef char c8;

#define DO(X) std::cout<<#X<<" : "<<X<<std::endl;std::cout.flush();

#define DATA 255U

#define DEFAULT_LOCAL_HISTORY_COUNTER 2
#define MAX_LOCAL_HISTORY_COUNTER_VALUE 3
#define MIN_LOCAL_HISTORY_COUNTER_VALUE 0
#define LOCAL_HISTORY_PC_BITS_TO_IGNORE 2
#define LOCAL_HISTORY_COUNTER_THRESH 1

#define DEFAULT_GLOBAL_HISTORY_COUNTER 0

#endif //__CUSTOM_TYPES_H__

