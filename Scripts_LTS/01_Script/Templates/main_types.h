////////////////////////////////////////////////////////////////////////////////
//
//      O       OOOO  OOOOOOO   O       O         ## ## ##
//     O O     O         O      O      O O
//    O   O    O         O      O     O   O      ## ## ##
//   OOOOOOO   O         O      O    OOOOOOO
//  O       O   OOOO     O      O   O       O   ## ## ##
//
//----------------------------------------------------------------------------
// HEAD : (c) ACTIA Automotive 2011 : " Any copy and re-use of this
//        file without the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (-)Confident. ACTIA   (-)Confident. Client
//        --------------------------------------------------------------------
//        MAIN : BSP ACTIA Standard types definition
////////////////////////////////////////////////////////////////////////////////
// FILE : main_types.h (HEADER)
//----------------------------------------------------------------------------
// DESC : BSP ACTIA Standard types definition
//----------------------------------------------------------------------------
// HIST :
// Version 		:$Revision: 4518 $
// Url			:$HeadURL: https://forge2.actia.fr/bgse/8160_actiway/SPU4026-3/_Reference/Generic/SPU4026-3_SW_Bootloader/Sources/CORE/base/main_types.h $
// Updated the :$Date: 2013-08-30 14:38:04 +0100 (ven., 30 août 2013) $
// By 			:$Author: ylassall $
////////////////////////////////////////////////////////////////////////////////
#ifndef MAIN_TYPES_INCLUDE
#define MAIN_TYPES_INCLUDE

////////////////////////////////////////////////////////////////////////////////
// DEFINITION
////////////////////////////////////////////////////////////////////////////////
#define SU64SUPPORT		1U


////////////////////////////////////////////////////////////////////////////////
// MACRO
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// TYPEDEF
////////////////////////////////////////////////////////////////////////////////
typedef signed char 		TS8;
typedef unsigned char 		TU8;
typedef unsigned char		*TU8_p;
typedef signed short 		TS16;
typedef unsigned short 		TU16;
typedef unsigned short		*TU16_p;
typedef signed int			TS32;
typedef unsigned int 		TU32;
typedef unsigned int		*TU32_p; 


#if (SU64SUPPORT>0U)
typedef signed long long	TS64;
typedef unsigned long long	TU64;
#endif

typedef enum
{
	E_OK = 0,
	E_NOT_OK = 1
}Std_ReturnType;

#define K_ERROR_RETURN (-1)

#define NULL 0
#define NULL_PTR (void*)0

#define K_FALSE 0
#define K_TRUE  1

#define TBITF	int				// Used exclusively for Bitfield definition 

#if !defined(__bool_true_false_are_defined) && !defined(__cplusplus)
#define false 0
#define true  1
#endif

#endif	/*	MAIN_TYPES_INCLUDE	*/
