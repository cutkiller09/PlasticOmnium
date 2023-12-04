#ifndef STANDARD_H_
#define STANDARD_H_

/* Standard types */

/* types definitions portable on 32 bits and 64 bits targets:
 * int <=> 32 bits on both targets
 * long long int <=> 64 bits on both targets
 */
typedef char bool;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64;
typedef signed char sint8;
typedef signed short sint16;
typedef signed int sint32;
typedef signed long long int sint64;
typedef float float32;


typedef enum
{
	E_OK = 0,
	E_NOT_OK = 1
}Std_ReturnType;

#define K_ERROR_RETURN (-1)

#define NULL (bool)0
#define NULL_PTR (void*)0

#define K_FALSE (bool)0
#define K_TRUE  (bool)1


#define INT8_MAX    0x7f
#define INT8_MIN    (-INT8_MAX - 1)
#define INT16_MAX   0x7fff
#define INT16_MIN   (-INT16_MAX - 1)
#define INT32_MAX	(2147483647)
#define INT32_MIN   (-INT32_MAX - 1L)
#define INT64_MAX   0x7fffffffffffffffLL
#define INT64_MIN   (-INT64_MAX - 1LL)


/* Maximum of unsigned integral types.  */
# define UINT8_MAX  (255)
# define UINT16_MAX (65535)
# define UINT32_MAX (4294967295U)
# define UINT64_MAX (__UINT64_C(18446744073709551615))

#endif /* STANDARD_H_ */
