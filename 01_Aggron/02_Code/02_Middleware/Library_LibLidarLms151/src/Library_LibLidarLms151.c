/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_LibLidarLms151.c 
 * $Author: Slo
 * $Date: 2016/01/14
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_LibLidarLms151.c 
  * \brief Fichier Source du Service LibLidarLms151
*/
/*============================================================================*/
/* COPYRIGHT (C) ROBOSOFT 2015	                                              */ 
/* ALL RIGHTS RESERVED                                                        */
/*                                                                            */
/* The reproduction, transmission, or use of this document or its content is  */
/* not permitted without express written authority. Offenders will be liable  */
/* for damages.                                                               */
/* All rights, including rights created by patent grant or registration of a  */
/* utility model or design, are reserved.                                     */
/*                                                                            */
/*============================================================================*/ 

/* Includes 								      */
/*============================================================================*/ 
#include "types.h"
#include "Library_StdLib.h"
#include "Library_LibLidarLms151.h" 
#include "Driver_TcpSocketDriver.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#define K_CONFIGURATION_DESACTIVATED // For Integration needs (configuration is too long during integration after each module restart)

/* Constants and types  						      */
/*============================================================================*/

/**
 * @enum  te_command_status
 * @brief define the command status
 */
typedef enum {
	E_COMMAND_ERROR=0,
	E_COMMAND_SUCCESS
}te_command_status;

/**
 * @enum  te_start_status
 * @brief define the start status
 */
typedef enum {
	E_START_NO_ERROR=0,
	E_START_NOT_ALLOWED
}te_start_status;

/**
 * @enum  te_subState
 * @brief define the mode substate
 */
typedef enum {
	E_IDLE = 0,			/*! Mode is in IDLE state */
	E_READ,				/*! Mode is actually in Read state */
	E_ERROR				/*! Mode is in Error state */
}te_subState;

/**
 * @enum  te_ReadStatus
 * @brief define the read status
 */
typedef enum
{
	E_READ_BUSY=0,		/*! Read busy */
	E_READ_FINISH,		/*! Read finished */
	E_READ_NO_DATA		/*! No data read */
}te_ReadStatus;

/**
 * @enum  te_WriteStatus
 * @brief define the write status
 */
typedef enum
{
	E_WRITE_BUSY=0,		/*! Read busy */
	E_WRITE_FINISH,		/*! Read finished */
	E_WRITE_ERROR		/*! Error During Data Write */
}te_WriteStatus;

/**
 * @enum  te_ContentOutChannel
 * @brief define the lidar Content Out channel
 */
typedef enum
{
	E_OUT_DIST1 = 0,	/*! Content channel is DIST1 */
	E_OUT_DIST2,		/*! Content channel is DIST2 */
	E_OUT_RSSI1,		/*! Content channel is RSSI1 */
	E_OUT_RSSI2			/*! Content channel is RSSI2 */
}te_ContentOutChannel;

/* Commands define */
static const uint8 gau8_CmdSetAccesMode[]="SetAccessMode";					/*! Global constant Set Access Mode */
static const uint8 gau8_CmdmLMPsetscancfg[]="mLMPsetscancfg";				/*! Global constant Info of scan frequency and angular resolution */
static const uint8 gau8_CmdmLMPscancfg[]="LMPscancfg";						/*! Global constant Get Info of scan frequency and angular resolution */
static const uint8 gau8_CmdLMDscandatacfg[]="LMDscandatacfg"; 				/*! Global constant Configure scan data content */
static const uint8 gau8_CmdLMPoutputRange[]="LMPoutputRange"; 				/*! Global constant Configure scan data output */
static const uint8 gau8_CmdmEEwriteall[]="mEEwriteall";						/*! Global constant Store Parameters */
static const uint8 gau8_CmdRun[]="Run"; 									/*! Global constant Log Out */
static const uint8 gau8_CmdLMDscandata[]="LMDscandata"; 					/*! Global constant Request Scan*/
static const uint8 gau8_CmdLSPsetdatetime[]="LSPsetdatetime";				/*! Global constant Set date time */
static const uint8 gau8_CmdLCMstate[]="LCMstate";							/*! Global constant Status of LMS */
static const uint8 gau8_CmdLMCstartmeas[]="LMCstartmeas";					/*! Global constant Start measurements */
static const uint8 gau8_CmdLMCstopmeas[]="LMCstopmeas";						/*! Global constant Stop measurements */
static const uint8 gau8_CmdStatusAndTime[]="STlms";							/*! Global constant Get Status and Time */
static const uint8 gau8_LoginMdp[]= "81BE23AA";
static const uint8 gau8_LoginService[]= "04";

static const uint8	gu8_telegram_start	= 0x02;								/*! Global constant Telegram start */
static const uint8	gu8_telegram_end	= 0x03;								/*! Global constant Telegram end */
static const uint32 gu32_Reserved=1;										/*! Global constant Reserved value "1" */

#define K_FRAME_SIZE_MAX 							262144ul				/*! buffer maximal size value */
#define K_START 									1						/*! Start value */
#define K_AMOUNT_OF_ENCODERS_MAX 					3						/*! Maximum amount of encoders */
#define K_AMOUNT_OF_16BITS_CHANNELS_MAX 			5 //2					/*! Maximum amount of 16bits channels */
#define K_AMOUNT_OF_8BITS_CHANNELS_MAX 				5 //2					/*! Maximum amount of 8bits channels */
#define K_CONTENT_SIZE							    128
#define K_FILE_LMS  "/home/agv/AGV/LOG_LMS151.txt"

/* Variables 								      */
/*============================================================================*/ 
static te_LibLidarLms151_error			ge_LibLidarLms151_error=E_LIBLMS151_ERR_NOT_INITIALIZED;
static Std_ReturnType 					gt_InitLibLidarLms151_Status; /* Global Initialization variable */
static Std_ReturnType 					gt_ConfLibLidarLms151_Status; /* Global Configuration variable */


static ts_LibLidarLms151_scan_data 		gs_LibLidarLms151_scan_data;					/*! Global scan data */
static te_subState 						ge_SubState;									/*! Global sub state */
static te_mode 							ge_Mode;										/*! Global mode */
static uint8 							gau8_content[K_CONTENT_SIZE];								/*! Global Content data buffer */
static uint32 							gu32_FramePosition;								/*! Global Frame position used for mutliple read before having a complete frame */
static uint32 							gu32_Timestamp;									/*! Last Global Timestamp returned by lms */
static uint32 							gu32_old_Timestamp;							    /*! Previous Global Timestamp returned by lms */
static DrvTcpsocket_cfg_type 			gs_socket_configuration ;						/*! Global Socket configuration */
static ts_LibLidarLms151_laserconf 			gs_configuration ;								/*! Global Configuration */
static ts_LibLidarLms151_dataconf 		gs_DataContentConfiguration ;					/*! Global Data Content Configuration */
static DrvTcpsocket_descriptor_type 	gs32_socket_desc ; 								/*! Global Socket Descriptor */
static uint8 							gau8_FrameSend[K_FRAME_SIZE_MAX] ;				/*! Global Frame send buffer */
static uint8 							gau8_FrameReceive[K_FRAME_SIZE_MAX] ;			/*! Global Frame receive buffer */
static uint8 							gau8_TemporaryFrameReceive[K_FRAME_SIZE_MAX] ;	/*! Global Temporary frame receive buffer */
static te_InternalStateMachine 			ge_InternalStateMachine = E_STATE_LMS151_UNDEFINED;	/*! Global Internal state machine */
static te_LibLidarLms151_device_status 	ge_DeviceStatus;								/*! Global Device status */
static bool								gb_is_new_data;									/*! Global New Data status */
static uint32							gu32_FrameSize;
static FILE* fd_Lms = NULL_PTR ;



/* Private functions prototypes 					      */
/*============================================================================*/
static Std_ReturnType lms151_write( const uint8 * const pu8_BinaryTelegram, const uint32 u32_size, te_WriteStatus *const pe_WriteStatus);
static Std_ReturnType lms151_read(te_ReadStatus *const pe_ReadStatus);
static Std_ReturnType lms151_logIn_set(te_WriteStatus *const pe_WriteStatus);
static Std_ReturnType lms151_logIn_check(te_ReadStatus *const pe_ReadStatus);
static Std_ReturnType lms151_logOut_set(te_WriteStatus *const pe_WriteStatus);
static Std_ReturnType lms151_logOut_check(te_ReadStatus *const pe_ReadStatus);
static Std_ReturnType lms151_FrequencyAndResolution_set(te_WriteStatus *const pe_WriteStatus);
static Std_ReturnType lms151_FrequencyAndResolution_check(te_ReadStatus *const pe_ReadStatus);
static Std_ReturnType lms151_configure_ScanDataContent_set(te_WriteStatus *const pe_WriteStatus);
static Std_ReturnType lms151_configure_ScanDataContent_check(te_ReadStatus *const pe_ReadStatus);
static Std_ReturnType lms151_configure_ScanDataOutput_set(te_WriteStatus *const pe_WriteStatus);
static Std_ReturnType lms151_configure_ScanDataOutput_check(te_ReadStatus *const pe_ReadStatus);
static Std_ReturnType lms151_store_parameters_set(te_WriteStatus *const pe_WriteStatus);
static Std_ReturnType lms151_store_parameters_check(te_ReadStatus *const pe_ReadStatus);
static Std_ReturnType lms151_Permanent_Scan_set(te_WriteStatus *const pe_WriteStatus);
static Std_ReturnType lms151_Permanent_Scan_check(te_ReadStatus *const pe_ReadStatus);

/* inline functions prototypes							      */
/*============================================================================*/
static inline Std_ReturnType GetAndCompareString(const uint8 * const pau8_Command);
static inline Std_ReturnType GetAndCompareData(const uint32 u32_Data);
static inline Std_ReturnType GetData(uint32 *const pu32_Data);
static inline Std_ReturnType GetContent(te_ContentOutChannel  *const pe_ContentOutChannel);

/* inline functions 							      */
/*============================================================================*/
/**
* \fn  static inline Std_ReturnType GetContent(te_ContentOutChannel  *pe_ContentOutChannel)
* \brief this function Get content from Message
*
* \param
*
* \return E_OK if message content is one of the te_ContentOutChannel type and pe_ContentOutChannel is not NULL
* 		  else E_NOT_OK
*/
static inline Std_ReturnType GetContent(te_ContentOutChannel  *const pe_ContentOutChannel)
{
	/* Declaration */
	Std_ReturnType t_GetContent_status;
	uint8 * pu8_Message;
	sint8 s8_strcmp;

	/* Initialization */
	t_GetContent_status=E_OK;

	/* Function Body */
	if(NULL_PTR!=pe_ContentOutChannel)
	{
		pu8_Message = (uint8 *)strtok ((char *)NULL, (const char *)" "); //Content
		if (NULL!=pu8_Message)
		{
			/*! If content is DIST1, then data referred by pe_ContentOutChannel will be set to E_OUT_DIST1 */
			s8_strcmp=strcmp((const char *)pu8_Message,"DIST1");
			if (0==s8_strcmp)
			{
				*pe_ContentOutChannel=E_OUT_DIST1;
			}
			else
			{
				/*! If content is DIST2, then data referred by pe_ContentOutChannel will be set to E_OUT_DIST2 */
				s8_strcmp=strcmp((const char *)pu8_Message,"DIST2");
				if (0==s8_strcmp)
				{
					*pe_ContentOutChannel=E_OUT_DIST2;
				}
				else
				{
					/*! If content is RSSI1, then data referred by pe_ContentOutChannel will be set to E_OUT_RSSI1 */
					s8_strcmp=strcmp((const char *)pu8_Message,"RSSI1");
					if (0==s8_strcmp)
					{
						*pe_ContentOutChannel=E_OUT_RSSI1;
					}
					else
					{
						/*! If content is RSSI2, then data referred by pe_ContentOutChannel will be set to E_OUT_RSSI2 */
						s8_strcmp=strcmp((const char *)pu8_Message,"RSSI2");
						if (0==s8_strcmp)
						{
							*pe_ContentOutChannel=E_OUT_RSSI2;
						}
						else
						{
							t_GetContent_status=E_NOT_OK;
						}
					}
				}
			}
		}
		else
		{
			;
		}
	}
	else
	{
		t_GetContent_status=E_NOT_OK;
	}

	/* return code */
	return(t_GetContent_status);
}


/**
* \fn  static inline GetAndCompareData(const uint32 u32_Data)
* \brief this function Get and compare data received in the lidar response
*
* \param
*
* \return E_OK if data received is equal to data referred by pau8_Command and pau8_Command is not NULL
* 		  else E_NOT_OK
*/
static inline Std_ReturnType GetAndCompareData(const uint32 u32_Data)
{
	/* Declaration */
	Std_ReturnType t_GetAndCompareData_status;
	uint8 * pu8_Message;
	sint32 s32_value;

	/* Initialization */

	/* Function Body */
	pu8_Message = (uint8 *)strtok ((char *)NULL, (const char *)" ");
	if (NULL!=pu8_Message)
	{
		s32_value=sscanf((const char * volatile)pu8_Message, "%127s", gau8_content); /*! Buffer overrun Data Will not */
		if (s32_value>0)
		{
			s32_value=strtol((const char *)gau8_content,(char **)NULL,16);
			if (u32_Data!=s32_value)
			{
				/*! If Data is not the same read */
				t_GetAndCompareData_status=E_NOT_OK;
			}
			else
			{
				t_GetAndCompareData_status=E_OK;
			}
		}
		else
		{
			t_GetAndCompareData_status=E_NOT_OK;
		}
	}
	else
	{
		t_GetAndCompareData_status=E_NOT_OK;
	}

	/* return code */
	return(t_GetAndCompareData_status);
}

/**
* \fn  static inline GetData(uint32 const *pu32_Data)
* \brief this function Get data
*
* \param
*
* \return E_OK if pu32_Data and strtok return is not NULL
* 		  else E_NOT_OK
*/
static inline Std_ReturnType GetData(uint32  *const pu32_Data)
{
	/* Declaration */
	Std_ReturnType t_GetData_status;
	uint8 * pu8_Message;
	sint32 s32_value;

	/* Initialization */

	/* Function Body */
	if (NULL_PTR!=pu32_Data)
	{
		pu8_Message = (uint8 *)strtok ((char *)NULL, (const char *)" ");
		if (NULL!=pu8_Message)
		{
			s32_value=sscanf((const char * volatile)pu8_Message, "%127s", gau8_content); /*! Buffer overrun Data Will not happen */
			if (s32_value>0)
			{
				*pu32_Data=(uint32)strtoul((const char *)gau8_content,(char **)NULL,16);
				t_GetData_status=E_OK;
			}
			else
			{
				t_GetData_status=E_NOT_OK;
			}
		}
		else
		{
			t_GetData_status=E_NOT_OK;
		}
	}
	else
	{
		t_GetData_status=E_NOT_OK;
	}


	/* return code */
	return(t_GetData_status);
}

/**
* \fn  static inline Std_ReturnType GetAndCompareString(const uint8 * const pau8_Command)
* \brief this function Get and compare strings received in the lidar response
*
* \param
*
* \return E_OK if string received is equal to string referred by pau8_Command and pau8_Command is not NULL
* 		  else E_NOT_OK
*/
static inline Std_ReturnType GetAndCompareString(const uint8 * const pau8_Command)
{
	/* Declaration */
	Std_ReturnType t_GetAndCompareString_status;
	sint8 s8_strcmp;
	uint8 * pu8_Message;

	/* Initialization */

	/* Function Body */
	/*! Get and compare command received */
	pu8_Message = (uint8 *)strtok ((char *)NULL, (const char *)" ");
	if (NULL!=pu8_Message)
	{
		s8_strcmp=strcmp((const char *)pu8_Message,(const char *)pau8_Command);
		if (0!=s8_strcmp)
		{
			/*! If command received is not the same configured then set return status to E_NOT_OK */
			t_GetAndCompareString_status=E_NOT_OK;
		}
		else
		{
			t_GetAndCompareString_status=E_OK;
		}
	}
	else
	{
		t_GetAndCompareString_status=E_NOT_OK;
	}

	/* return code */
	return(t_GetAndCompareString_status);
}

/* Private functions 							      */
/*============================================================================*/

/**
 * \fn  static Std_ReturnType lms151_configure_ScanDataContent(te_WriteStatus *const pe_WriteStatus)
 * \brief this function configure scan data content
 *
 * \param none
 *
 * \return  E_OK if scan data content configuration command has been well sent (lms151_write return is E_OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_configure_ScanDataContent_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	uint32 u32_size;
	Std_ReturnType t_ScanDataContent_status;

    /* Initialization */

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
    	u32_size= sprintf ((char *)gau8_FrameSend, "%c%s %02d %02d %X %X %X %02d %02d %X %X %X %X %s%c",
				  gu8_telegram_start,
				  "sWN LMDscandatacfg",
				  gs_DataContentConfiguration.u8_data_channel_0,
				  gs_DataContentConfiguration.u8_data_channel_1,
				  gs_DataContentConfiguration.b_remission,
				  gs_DataContentConfiguration.e_resolution,
				  gs_DataContentConfiguration.u8_unit,
				  gs_DataContentConfiguration.u8_encoder_0,
				  gs_DataContentConfiguration.u8_encoder_1,
				  gs_DataContentConfiguration.b_position,
				  gs_DataContentConfiguration.b_device_name,
				  gs_DataContentConfiguration.b_comment,
				  gs_DataContentConfiguration.b_time,
				  gs_DataContentConfiguration.au8_output_rate,
				  gu8_telegram_end);

		/*! Write Frequency and Resolution configuration Telegram */
		t_ScanDataContent_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_ScanDataContent_status=E_NOT_OK;
    }

    /* return code  */
	return(t_ScanDataContent_status);
}

/**
 * \fn  static Std_ReturnType lms151_configure_ScanDataContent_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function check Scan Data Content has been well configured
 *
 * \param te_ReadStatus *pe_ReadStatus : read status
 *
 * \return  E_OK if scan data content configuration command has been well sent and pe_ReadStatus is not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_configure_ScanDataContent_check(te_ReadStatus *const pe_ReadStatus)
{
    /* Declaration */
	Std_ReturnType t_ScanDataContent_status;

    /* Initialization */
	t_ScanDataContent_status=E_OK;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_ReadStatus)
    {
		/*! Read Frequency and Resolution configuration Answer */
    	t_ScanDataContent_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_ScanDataContent_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
								break;
				case E_READ_FINISH :
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // gau8_CmdLMDscandatacfg
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{

		}
    }
    else
    {
    	/*! pe_ReadStatus is NULL, return E_NOT_OK */
    	t_ScanDataContent_status=E_NOT_OK;
    }

    /* return code  */
	return(t_ScanDataContent_status);
}

/**
 * \fn  static Std_ReturnType lms151_AskDeviceStatus_set(te_WriteStatus *const pe_WriteStatus)
 * \brief this function will Ask device status
 *
 * \param none
 *
 * \return  E_OK if ask device status command has been sent (lms151_write return is OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_AskDeviceStatus_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	Std_ReturnType t_AskDeviceStatusSet_status;
	uint32 u32_size;

    /* Initialization */

    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
    	u32_size=sprintf ((char *)gau8_FrameSend, "%c%s%c",
    			  gu8_telegram_start,
    			  "sRN STlms",
    			  gu8_telegram_end);

		/*! Write Frequency and Resolution configuration Telegram */
    	t_AskDeviceStatusSet_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_AskDeviceStatusSet_status=E_NOT_OK;
    }

    /* return code  */
	return(t_AskDeviceStatusSet_status);
}



/**
 * \fn  static Std_ReturnType lms151_AskDeviceStatus_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function will check Ask device status command has been well sent
 *
 * \param te_ReadStatus *pe_ReadStatus : read status
 *
 * \return  E_OK if ask device status command has been well sent and pe_ReadStatus is not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_AskDeviceStatus_check(te_ReadStatus *const pe_ReadStatus)
{
    /* Declaration */
	Std_ReturnType t_AskDeviceStatusCheck_status;

    /* Initialization */
	t_AskDeviceStatusCheck_status=E_OK;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_ReadStatus)
    {
		/*! Read Frequency and Resolution configuration Answer */
    	t_AskDeviceStatusCheck_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_AskDeviceStatusCheck_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
								break;
				case E_READ_FINISH :
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType

								/*! Get and compare command received is gau8_CmdStatusAndTime */
								t_AskDeviceStatusCheck_status|=GetAndCompareString(gau8_CmdStatusAndTime);

								/*! Get and compare s32_start_angle */
								t_AskDeviceStatusCheck_status|= GetData((uint32 *)&ge_DeviceStatus);
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{
			; /*! Do Nothing */
		}
    }
    else
    {
    	/*! pe_ReadStatus is NULL, return E_NOT_OK */
    	t_AskDeviceStatusCheck_status=E_NOT_OK;
    }

    /* return code  */
	return(t_AskDeviceStatusCheck_status);
}

/**
 * \fn  static Std_ReturnType lms151_configure_ScanDataOutput_set(te_WriteStatus *const pe_WriteStatus)
 * \brief this function will configure scan data output
 *
 * \param none
 *
 * \return  E_OK if scan data output is configured (lms151_write return is OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_configure_ScanDataOutput_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	Std_ReturnType t_ScanDataOutput_status;
	uint32 u32_size;

    /* Initialization */

    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
    	u32_size=sprintf ((char *)gau8_FrameSend, "%c%s +1 %X %X %X%c",
    			  gu8_telegram_start,
    			  "sWN LMPoutputRange",
    			  gs_configuration.u32_angle_resolution,
    			  gs_configuration.s32_start_angle,
    			  gs_configuration.s32_stop_angle,
    			  gu8_telegram_end);


    	/*! Write Frequency and Resolution configuration Telegram */
    	t_ScanDataOutput_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_ScanDataOutput_status=E_NOT_OK;
    }

    /* return code  */
	return(t_ScanDataOutput_status);
}

/**
 * \fn  static Std_ReturnType lms151_configure_ScanDataOutput_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function will check configure scan data output command has been well done
 *
 * \param te_ReadStatus *pe_ReadStatus : read status
 *
 * \return  E_OK if scan data output command has been well sent and pe_ReadStatus not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_configure_ScanDataOutput_check(te_ReadStatus *const pe_ReadStatus)
{
    /* Declaration */
	Std_ReturnType t_ScanDataOutput_status;

    /* Initialization */
	t_ScanDataOutput_status=E_OK;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_ReadStatus)
    {
		/*! Read Frequency and Resolution configuration Answer */
    	t_ScanDataOutput_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_ScanDataOutput_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
								break;
				case E_READ_FINISH :
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // LMPoutputRange
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{
			;
		}
    }
    else
    {
    	/*! pe_ReadStatus is NULL, return E_NOT_OK */
    	t_ScanDataOutput_status=E_NOT_OK;
    }

    /* return code  */
	return(t_ScanDataOutput_status);
}

/**
 * \fn  static Std_ReturnType lms151_store_parameters_set(te_WriteStatus *const pe_WriteStatus)
 * \brief this function will store parameters
 *
 * \param none
 *
 * \return  E_OK if store parameters command has been well sent (lms151_write return E_OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_store_parameters_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	Std_ReturnType t_store_status;
	uint32 u32_size;

    /* Initialization */

    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
        /* Function Body */
    	/*! Build and send message sWN LMPoutputRange */
    	u32_size=sprintf((char *)gau8_FrameSend, "%c%s%c",
    			  gu8_telegram_start,
    			  "sMN mEEwriteall",
    			  gu8_telegram_end);

    	/*! Write Frequency and Resolution configuration Telegram */
    	t_store_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_store_status=E_NOT_OK;
    }

    /* return code  */
	return(t_store_status);
}

/**
 * \fn  static Std_ReturnType lms151_store_parameters_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function will check store parameters command has been well done
 *
 * \param te_ReadStatus *pe_ReadStatus : read status
 *
 * \return  E_OK if store parameters command has been well sent and pe_ReadStatus not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_store_parameters_check(te_ReadStatus *const pe_ReadStatus)
{
    /* Declaration */
	Std_ReturnType t_store_parameters_status;

    /* Initialization */
	t_store_parameters_status=E_OK;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_ReadStatus)
    {
		/*! Read Frequency and Resolution configuration Answer */
    	t_store_parameters_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_store_parameters_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
								break;
				case E_READ_FINISH :
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType

								/*! Get and compare command received is gau8_CmdmEEwriteall */
								t_store_parameters_status|=GetAndCompareString(gau8_CmdmEEwriteall);

								/*! Get and compare data received is E_COMMAND_SUCCESS */
								t_store_parameters_status|= GetAndCompareData((uint32)E_COMMAND_SUCCESS);
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{
			;/*! Do Nothing */
		}
    }
    else
    {
    	/*! pe_ReadStatus is NULL, return E_NOT_OK */
    	t_store_parameters_status=E_NOT_OK;
    }

    /* return code  */
	return(t_store_parameters_status);
}

/**
 * \fn  static Std_ReturnType lms151_FrequencyAndResolution_set(te_WriteStatus *const pe_WriteStatus)
 * \brief this function will check frequency and resolution command has been well done
 *
 * \param none
 *
 * \return  E_OK if frequency and resolution command has been well sent (lms151_write return code set to E_OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_FrequencyAndResolution_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	uint32 u32_size;
	Std_ReturnType t_FrequencyAndResolution_status;

    /* Initialization */
	u32_size=0;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
    	/*! Build and send message sMN mLMPsetscancfg */
    	u32_size=sprintf ((char *)gau8_FrameSend, "%c%s %X +1 %X %X %X%c",
    			  gu8_telegram_start,
    			  "sMN mLMPsetscancfg",
    			  gs_configuration.u32_scaning_frequency,
    			  gs_configuration.u32_angle_resolution,
    			  gs_configuration.s32_start_angle,
    			  gs_configuration.s32_stop_angle,
    			  gu8_telegram_end);

    	/*! Write Frequency and Resolution configuration Telegram */
    	t_FrequencyAndResolution_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_FrequencyAndResolution_status=E_NOT_OK;
    }

    /* return code  */
	return(t_FrequencyAndResolution_status);
}

/**
 * \fn  static Std_ReturnType lms151_FrequencyAndResolution_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function will check frequency and resolution command has been well done
 *
 * \param te_ReadStatus *pe_ReadStatus : read status
 *
 * \return  E_OK if frequency and resolution command has been well sent and pe_ReadStatus not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_FrequencyAndResolution_check(te_ReadStatus *const pe_ReadStatus)
{
    /* Declaration */
	Std_ReturnType t_FrequencyAndResolution_status;

    /* Initialization */
	t_FrequencyAndResolution_status=E_OK;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_ReadStatus)
    {
		/*! Read Frequency and Resolution configuration Answer */
    	t_FrequencyAndResolution_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_FrequencyAndResolution_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
								break;
				case E_READ_FINISH :
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType

								/*! Get and compare command received is CmdmLMPsetscancfg */
								t_FrequencyAndResolution_status=GetAndCompareString(gau8_CmdmLMPsetscancfg);

								/*! Get and compare data received is E_LIBLMS151_NO_ERROR */
								t_FrequencyAndResolution_status|= GetAndCompareData((uint32)E_LIBLMS151_NO_ERROR);

								/*! Get and compare data received is gs_configuration.u32_scaning_frequency */
								t_FrequencyAndResolution_status|= GetAndCompareData(gs_configuration.u32_scaning_frequency);

								/*! Get and compare data received is gu32_Reserved */
								t_FrequencyAndResolution_status|= GetAndCompareData(gu32_Reserved);

								/*! Get and compare data received is gs_configuration.u32_angle_resolution */
								t_FrequencyAndResolution_status|= GetAndCompareData(gs_configuration.u32_angle_resolution);
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{
			; /*! Do Nothing */
		}
    }
    else
    {
    	/*! pe_ReadStatus is NULL, return E_NOT_OK */
    	t_FrequencyAndResolution_status=E_NOT_OK;
    }

    /* return code  */
	return(t_FrequencyAndResolution_status);
}

/**
 * \fn  static Std_ReturnType lms151_logIn_set(te_WriteStatus *const pe_WriteStatus)
 * \brief this function will login user
 *
 * \param none
 *
 * \return  E_OK if login has been well sent (lms151_write return code set to E_OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_logIn_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	Std_ReturnType t_logIn_status;
	uint32 u32_size;

    /* Initialization */
	u32_size=0;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
    	/*! Build and send message sMN SetAccessMode */
    	u32_size=sprintf ((char *)gau8_FrameSend, "%c%s %s %s%c",
    			  gu8_telegram_start,
    			  "sMN SetAccessMode",
    			  gau8_LoginService,
    			  gau8_LoginMdp,
    			  gu8_telegram_end);

    	/*! Write Frequency and Resolution configuration Telegram */
    	t_logIn_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_logIn_status=E_NOT_OK;
    }

    /* return code  */
	return(t_logIn_status);
}

/**
 * \fn  static Std_ReturnType lms151_logIn_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function will check log in command has been well done
 *
 * \param te_ReadStatus *pe_ReadStatus : read status
 *
 * \return  E_OK if login command has been well sent (lms151_write return code set to E_OK) and pe_ReadStatus not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_logIn_check(te_ReadStatus *const pe_ReadStatus)
{
    /* Declaration */
	Std_ReturnType t_logIn_status;

    /* Initialization */
	t_logIn_status=E_OK;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_ReadStatus)
    {
		/*! Check login has been well done */
		t_logIn_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_logIn_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
									break;
				case E_READ_FINISH :
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType

								/*! Get and compare command received is CmdSetAccesMode */
								t_logIn_status|=GetAndCompareString(gau8_CmdSetAccesMode);

								/*! Get and compare data received is E_COMMAND_SUCCESS */
								t_logIn_status|= GetAndCompareData((uint32)E_COMMAND_SUCCESS);
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{
			; /*! Do Nothing */
		}
    }
    else
    {
    	/*! pe_ReadStatus is NULL, return E_NOT_OK */
		t_logIn_status=E_NOT_OK;
    }

    /* return code  */
	return(t_logIn_status);
}


/**
 * \fn  static Std_ReturnType lms151_StartMeas_set(te_WriteStatus *const pe_WriteStatus)
 * \brief this function will start measurements
 *
 * \param none
 *
 * \return  E_OK if start measurements has been well sent (lms151_write return code set to E_OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_StartMeas_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	Std_ReturnType t_start_status;
	uint32 u32_size;

    /* Initialization */

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
    	/*! Build and send message sMN LMCstartmeas */
    	u32_size=sprintf((char *)gau8_FrameSend, "%c%s%c",
    			  gu8_telegram_start,
    			  "sMN LMCstartmeas",
    			  gu8_telegram_end);

    	/*! Write Frequency and Resolution configuration Telegram */
    	t_start_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_start_status=E_NOT_OK;
    }

    /* return code  */
	return(t_start_status);
}

/**
 * \fn  static Std_ReturnType lms151_StartMeas_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function will check start measurements has been well done
 *
 * \param te_ReadStatus *pe_ReadStatus : read status
 *
 * \return  E_OK if start measurements has been well sent (lms151_write return code set to E_OK) and pe_ReadStatus not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_StartMeas_check(te_ReadStatus *pe_ReadStatus)
{
    /* Declaration */
	Std_ReturnType t_start_status;

    /* Initialization */
	t_start_status=E_OK;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_ReadStatus)
    {
		/*! Check login has been well done */
    	t_start_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_start_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
									break;
				case E_READ_FINISH :
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType

								/*! Get and compare command received is gau8_CmdLMCstartmeas */
								t_start_status|=GetAndCompareString(gau8_CmdLMCstartmeas);

								/*! Get and compare data received is E_START_NO_ERROR */
								t_start_status|= GetAndCompareData((uint32)E_START_NO_ERROR);
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{
			; /*! Do Nothing */
		}
    }
    else
    {
    	/*! pe_ReadStatus is NULL, return E_NOT_OK */
    	t_start_status=E_NOT_OK;
    }

    /* return code  */
	return(t_start_status);
}

/**
 * \fn  static Std_ReturnType lms151_logOut_set(te_WriteStatus *const pe_WriteStatus)
 * \brief this function will log Out User from LMS
 *
 * \param none
 *
 * \return E_OK if permanent scan is logout command has been well sent (lms151_write return code set to E_OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_logOut_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	Std_ReturnType t_logOut_status;
	uint32 u32_size;

    /* Initialization */

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
    	/*! Build and send message sMN Run */
    	u32_size=sprintf((char *)gau8_FrameSend, "%c%s%c",
    			  gu8_telegram_start,
    			  "sMN Run",
    			  gu8_telegram_end);

    	/*! Write Frequency and Resolution configuration Telegram */
    	t_logOut_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_logOut_status=E_NOT_OK;
    }

    /* return code  */
	return(t_logOut_status);
}

/**
 * \fn  static Std_ReturnType lms151_logOut_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function will check logout command has been well sent
 *
 * \param te_ReadStatus *pe_ReadStatus : Read status
 *
 * \return E_OK if permanent scan is logout command has been well sent and pe_ReadStatus is not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_logOut_check(te_ReadStatus *pe_ReadStatus)
{
    /* Declaration */
	Std_ReturnType t_logOut_status;

    /* Initialization */
	t_logOut_status=E_OK;

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_ReadStatus)
    {
		/*! Check login has been well done */
    	t_logOut_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_logOut_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
									break;
				case E_READ_FINISH :
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType

								/*! Get and compare command received is CmdSetAccesMode */
								t_logOut_status=GetAndCompareString(gau8_CmdRun);

								/*! Get and compare data received is E_COMMAND_SUCCESS */
								t_logOut_status|= GetAndCompareData((uint32)E_COMMAND_SUCCESS);
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{
			; /*! Do Nothing */
		}
    }
    else
    {
    	/*! pe_ReadStatus is NULL, return E_NOT_OK */
    	t_logOut_status=E_NOT_OK;
    }

    /* return code  */
	return(t_logOut_status);
}

/**
 * \fn static Std_ReturnType lms151_Permanent_Scan_set(te_WriteStatus *const pe_WriteStatus)
 * \brief this function configure permanent scan
 *
 * \param none
 *
 * \return E_OK if permanent scan is well written (return code of lms151_write is E_OK)
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_Permanent_Scan_set(te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	Std_ReturnType t_request_status;
	uint32 u32_size;

    /* Initialization */

    /* Function Body */
    /* Robustness Test */
    if (NULL_PTR!=pe_WriteStatus)
    {
    	/*! Build and send message sRN LMDscandata */
    	u32_size=sprintf((char *)gau8_FrameSend, "%c%s%c",
    			  gu8_telegram_start,
    			  "sEN LMDscandata 1",
    			  gu8_telegram_end);

    	/*! Write Frequency and Resolution configuration Telegram */
    	t_request_status=lms151_write(gau8_FrameSend, u32_size, pe_WriteStatus);
    }
    else
    {
    	/*! pe_WriteStatus is NULL, return E_NOT_OK */
    	t_request_status=E_NOT_OK;
    }

    /* return code  */
	return(t_request_status);
}

/**
 * \fn  static Std_ReturnType lms151_Permanent_Scan_check(te_ReadStatus *pe_ReadStatus)
 * \brief this function check if permanent scan is well configured
 *
 * \param te_ReadStatus *pe_ReadStatus : read status
 *
 * \return E_OK if permanent scan is well configured and pe_ReadStatus is not NULL
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_Permanent_Scan_check(te_ReadStatus *pe_ReadStatus)
{
	/* Declaration */
	Std_ReturnType t_PermanentScan_status;

	/* Initialization */
	t_PermanentScan_status=E_OK;

	/* Function Body */
	/* Robustness Test */
	if (NULL_PTR!=pe_ReadStatus)
	{
		/*! Read Frequency and Resolution configuration Answer */
		t_PermanentScan_status=lms151_read(pe_ReadStatus);

		if (E_NOT_OK!=t_PermanentScan_status)
		{
			switch (*pe_ReadStatus)
			{
				case E_READ_NO_DATA :
									break;
				case E_READ_FINISH :
									strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType

									/*! Get and compare command received is CmdLMDscandata */
									t_PermanentScan_status|=GetAndCompareString(gau8_CmdLMDscandata);

									/*! Get and compare data received is K_START */
									t_PermanentScan_status|= GetAndCompareData((uint32)K_START);
								break;
				case E_READ_BUSY :
				default :
					break;

			}

		}
		else
		{
			; /*! Do Nothing */
		}
	}
	else
	{
		/*! pe_ReadStatus is NULL, return E_NOT_OK */
		t_PermanentScan_status=E_NOT_OK;
	}

	/* return code  */
	return(t_PermanentScan_status);

}


/**
 * \fn static Std_ReturnType lms151_Scan_Receive(te_ReadStatus *pe_ReadStatus)
 * \brief this function get data from Lidar telegram
 *
 * \param none
 *
 * \return E_OK if data are well received
 * 		   else E_NOT_OK
 */
static Std_ReturnType lms151_Scan_Receive(void)
{
	/* Declaration */
	Std_ReturnType t_scan_status;
	uint32 u32_value;
	uint32 u32_i;
	uint32 u32_j;
	uint32 u32_NumberOfData;
	uint32 u32_Data;
	uint32 u32_NumberChannels8Bit;
	uint32 u32_NumberChannels16Bit;
	te_ContentOutChannel e_ContentOutChannel;
	te_ReadStatus e_ReadStatus;

	/* Initialization */
	t_scan_status=E_OK;
	u32_NumberOfData=0;
	u32_value=0;
	e_ContentOutChannel=E_OUT_DIST1;
	u32_Data=0;
	u32_NumberChannels16Bit=0;
	u32_NumberChannels8Bit=0;

	/* Function Body */
	/*! Read Frequency and Resolution configuration Answer */
	t_scan_status|=lms151_read(&e_ReadStatus);

	if (E_NOT_OK!=t_scan_status)
	{
		switch (e_ReadStatus)
		{
			case E_READ_NO_DATA :
								break;
			case E_READ_FINISH :

								gu32_FramePosition=0;
								/*! Get and compare command receive */
								strtok ((char *)gau8_FrameReceive,(const char *)" "); // CmdType

								strtok(NULL, " "); //Command
								strtok(NULL, " "); //VersionNumber
								strtok(NULL, " "); //DeviceNumber
								strtok(NULL, " "); //Serial number
								strtok(NULL, " "); //DeviceStatus
								strtok(NULL, " "); //MessageCounter
								strtok(NULL, " "); //ScanCounter
								strtok(NULL, " "); //Time since startup
								t_scan_status|= GetData(&gu32_Timestamp); //TransmissionDuration
								if (gu32_Timestamp!=gu32_old_Timestamp)
								{
									gs_LibLidarLms151_scan_data.b_is_new_data=K_TRUE;
									gu32_old_Timestamp=gu32_Timestamp;
								}
								else
								{
									gs_LibLidarLms151_scan_data.b_is_new_data=K_FALSE;
								}
								strtok(NULL, " "); //InputStatus
								strtok(NULL, " "); //OutputStatus
								strtok(NULL, " "); //ReservedByteA
								strtok(NULL, " "); //ScanningFrequency
								strtok(NULL, " "); //MeasurementFrequency
								strtok(NULL, " ");
								strtok(NULL, " ");
								strtok(NULL, " ");
								t_scan_status|= GetData(&u32_value);
								if (u32_value<K_AMOUNT_OF_ENCODERS_MAX)
								{
									for (u32_i = 0; u32_i < u32_value; u32_i++)
									{
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_EncoderPosition);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_EncoderSpeed);
									}
								}
								else
								{
									t_scan_status=E_NOT_OK;
								}

								t_scan_status|= GetData(&u32_NumberChannels16Bit);//(uint32)gau8_NumberChannels16Bit);

								if (u32_NumberChannels16Bit<K_AMOUNT_OF_16BITS_CHANNELS_MAX)
								{
									for (u32_i = 0; u32_i < u32_NumberChannels16Bit; u32_i++)
									{

										t_scan_status|=GetContent(&e_ContentOutChannel);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_ScaleFactor);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_ScaleFactorOffset);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_StartAngle);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_Steps);
										t_scan_status|= GetData(&u32_NumberOfData);

										for (u32_j = 0; u32_j < u32_NumberOfData; u32_j++)
										{
											t_scan_status|= GetData(&u32_Data);//(uint32)gau8_AmountOfData);
											switch(e_ContentOutChannel)
											{
												case E_OUT_DIST1 :
													gs_LibLidarLms151_scan_data.u16_dist_len1 = (uint16)u32_NumberOfData;
													gs_LibLidarLms151_scan_data.u16_dist1[u32_j] = (uint16) u32_Data;
													break;
												case E_OUT_DIST2 :
													gs_LibLidarLms151_scan_data.u16_dist_len2 = (uint16)u32_NumberOfData;
													gs_LibLidarLms151_scan_data.u16_dist2[u32_j] =  (uint16)u32_Data;
													break;
												case E_OUT_RSSI1 :
													gs_LibLidarLms151_scan_data.u16_rssi_len1 = (uint16)u32_NumberOfData;
													gs_LibLidarLms151_scan_data.u16_rssi1[u32_j] =  (uint16)u32_Data;
													break;
												case E_OUT_RSSI2 :
													gs_LibLidarLms151_scan_data.u16_rssi_len2 = (uint16)u32_NumberOfData;
													gs_LibLidarLms151_scan_data.u16_rssi2[u32_j] =  (uint16)u32_Data;
													break;
											}
										}
									}
								}
								else
								{
									t_scan_status=E_NOT_OK;
								}

								t_scan_status|= GetData(&u32_NumberChannels8Bit);//NumberChannels8Bit;

								if (u32_NumberChannels8Bit<K_AMOUNT_OF_8BITS_CHANNELS_MAX)
								{
									for (u32_i = 0; u32_i < u32_NumberChannels8Bit; u32_i++)
									{
										t_scan_status|=GetContent(&e_ContentOutChannel);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_ScaleFactor);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_ScaleFactorOffset);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_StartAngle);
										t_scan_status|= GetData(&u32_value);//(uint32)gau8_Steps);
										t_scan_status|= GetData(&u32_NumberOfData);

										for (u32_j = 0; u32_j < u32_NumberOfData; u32_j++)
										{
											t_scan_status|= GetData(&u32_Data);//(uint32)gau8_AmountOfData);
											switch(e_ContentOutChannel)
											{
												case E_OUT_DIST1 :
													gs_LibLidarLms151_scan_data.u16_dist_len1 = (uint16)u32_NumberOfData;
													gs_LibLidarLms151_scan_data.u16_dist1[u32_j] =  (uint16)u32_Data;
													break;
												case E_OUT_DIST2 :
													gs_LibLidarLms151_scan_data.u16_dist_len2 = (uint16)u32_NumberOfData;
													gs_LibLidarLms151_scan_data.u16_dist2[u32_j] =  (uint16)u32_Data;
													break;
												case E_OUT_RSSI1 :
													gs_LibLidarLms151_scan_data.u16_rssi_len1 = (uint16)u32_NumberOfData;
													gs_LibLidarLms151_scan_data.u16_rssi1[u32_j] =  (uint16)u32_Data;
													break;
												case E_OUT_RSSI2 :
													gs_LibLidarLms151_scan_data.u16_rssi_len2 = (uint16)u32_NumberOfData;
													gs_LibLidarLms151_scan_data.u16_rssi2[u32_j] =  (uint16)u32_Data;
													break;
											}
										}
									}
								}
								else
								{
									;
								}
							break;
			case E_READ_BUSY :
			default :
				break;

		}

	}
	else
	{
		; /*! Do Nothing */
	}

	/* return code  */
	return(t_scan_status);

}

/**
 * \fn  static Std_ReturnType lms151_write( const uint8 * const pu8_BinaryTelegram, const uint16 u16_size)
 * \brief this function write Telegram on Tcp socket driver
 *
 * \param  pu8_BinaryTelegram [out] : Telegram to send
 * 		   u16_size [in] : size of Telegram
 *
 * \return  E_OK if size parameter is equal to data written and no error returned from tcp driver
 * 			else E_NOT_OK
 */
static Std_ReturnType lms151_write( const uint8 * const pu8_BinaryTelegram, const uint32 u32_size, te_WriteStatus *const pe_WriteStatus)
{
    /* Declaration */
	sint32 s32_written;
	Std_ReturnType t_write_status;

    /* Initialization */
	s32_written=0;
	t_write_status=E_OK;

    /* Function Body */
	/*! Robustness test */
	if ((NULL_PTR!=pu8_BinaryTelegram)&&
		(NULL_PTR!=pe_WriteStatus))
	{
		/*! Send Telegram to TcpSocket driver */
		t_write_status|=DrvTcpSocket_Send( gs32_socket_desc , pu8_BinaryTelegram , u32_size , &s32_written ) ;

		/*! Check size given is equal to data written */
		if ((u32_size!=(uint32)s32_written)||
			(s32_written<0))
		{
			t_write_status=E_NOT_OK;
			*pe_WriteStatus=E_WRITE_ERROR;
		}
		else
		{
			/*! If all Datas are not written */
			if (u32_size!=(uint32)s32_written)
			{
				*pe_WriteStatus=E_WRITE_BUSY;
				t_write_status=E_OK;
			}
			else
			{
				/*! if size given is equal to data written return E_OK */
				t_write_status=E_OK;
				*pe_WriteStatus=E_WRITE_FINISH;
			}
		}
	}
	else
	{
		/*! if pu8_BinaryTelegram or pe_WriteStatus pointer are NULL then return E_NOT_OK */
		t_write_status=E_NOT_OK;
	}

    /* return code  */
	return(t_write_status);
}


/**
 * \fn  static Std_ReturnType lms151_read( te_ReadStatus *pe_ReadStatus)
 * \brief this function read trame from Tcp Socket Driver
 *
 * \param  te_ReadStatus *pe_ReadStatus : read status
 *
 * \return  E_OK if size parameter is equal to data read and no error returned from tcp driver
 * 			else E_NOT_OK
 */
static Std_ReturnType lms151_read( te_ReadStatus * const pe_ReadStatus)
{
    /* Declaration */
	sint32 s32_read;
	E_DRVTCPSOCKET_ERROR t_error;
	Std_ReturnType t_read_status;

    /* Initialization */
	s32_read=0;
	t_read_status=E_OK;

    /* Function Body */
	/*! Robustness test */
	if (NULL_PTR!=pe_ReadStatus)
	{
		*pe_ReadStatus=E_READ_NO_DATA;

		/*! Receive Telegram from TcpSocket driver */
		t_read_status|=DrvTcpSocket_Receive( gs32_socket_desc , &gs_socket_configuration , gau8_TemporaryFrameReceive , (uint32)K_FRAME_SIZE_MAX , &s32_read , &t_error ) ;

		if (((uint32)abs(s32_read)<K_FRAME_SIZE_MAX)&&
			(s32_read>0)&&
			(E_DRVTCPSOCKET_ERROR_NOERROR==t_error))
		{
			memcpy(&gau8_FrameReceive[gu32_FramePosition],gau8_TemporaryFrameReceive,s32_read);
			gu32_FramePosition=gu32_FramePosition+s32_read;
			if (gau8_FrameReceive[gu32_FramePosition-1]==gu8_telegram_end)
			{
				/*! if data in frame received is equal to gu8_telegram_end, set pe_ReadStatus data to E_READ_FINISH */
				*pe_ReadStatus=E_READ_FINISH;
				printf("lms151_read E_READ_FINISH \n");
				gu32_FrameSize=gu32_FrameSize+s32_read;
				printf("Lms 151 Frame nb bytes read : gu32_FrameSize : %i\n",gu32_FrameSize);
				printf("Lms 151 Frame nb bytes read : gu32_FramePosition : %i\n",gu32_FramePosition);
				gu32_FrameSize=0;
			}
			else
			{
				*pe_ReadStatus=E_READ_BUSY;
				printf("lms151_read E_READ_BUSY \n");
				gu32_FrameSize=gu32_FrameSize+s32_read;
			}
		}
		else
		{
			if (s32_read>=0)
			{
				t_read_status=E_OK;
			}
			else
			{
				t_read_status=E_NOT_OK; /* Error Case */
				printf("lms151_read Error Case \n");
			}
		}
	}
	else
	{
		/*! if pu8_BinaryTelegram pointer is NULL then return E_NOT_OK */
		t_read_status=E_NOT_OK;
	}
    /* return code  */
	return(t_read_status);
}

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType LibLidarLms151_Initialize(void) 
 * \brief this function initialize module
 *
 * \param  none
 *
* \return Std_ReturnType :
*         - E_OK if SrvConf_Get_Lms151TcpSocket, LibLidarLms151_Open and SrvConf_Get_Lms151OutputMode return functions are E_OK
*         - E_NOT_OK if not
 */
Std_ReturnType LibLidarLms151_Initialize(void) 
{
    /* Declaration */     
    Std_ReturnType t_Initialize_Status; 

    /* Initialization */
    gt_ConfLibLidarLms151_Status=E_NOT_OK;
	gs32_socket_desc=E_DRVTCPSOCKET_DESCRIPTOR_CLOSED;
	ge_InternalStateMachine=E_STATE_LMS151_INITIALIZATION;
	ge_DeviceStatus=E_LIBLMS151_STATUS_UNDEFINED;
	ge_LibLidarLms151_error=E_LIBLMS151_ERR_NO_ERROR;
	gu32_Timestamp=0;
	gb_is_new_data=K_FALSE;
	gu32_FrameSize=0;

    /* Function Body */
	fd_Lms = fopen( K_FILE_LMS , "wb") ;
	t_Initialize_Status=E_OK;

	gt_InitLibLidarLms151_Status=t_Initialize_Status;

    /* return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Configure( const ts_LibLidarLms151_conf * const ps_LibLidarLms151_conf,
 *											 const ts_LibLidarLms151_dataconf * const  ps_LibLidarLms151_dataconf,
 *											 const ts_LibLidarLms151_socket * const  ps_LibLidarLms151_socket)
 * \brief this function Configure module
 *
 * \param const ts_LibLidarLms151_laserconf * const ps_LibLidarLms151_laserconf : laser configuration
 * 		  const ts_LibLidarLms151_dataconf * const  ps_LibLidarLms151_dataconf : data (returned by laser) configuration
 * 		  const ts_LibLidarLms151_socket * const  ps_LibLidarLms151_socket : socket laser configuration
 *
* \return Std_ReturnType :
*         - E_OK if module is initialized and ps_LibLidarLms151_laserconf, ps_LibLidarLms151_dataconf, ps_LibLidarLms151_socket are not NULL
*         - E_NOT_OK if not
 */
Std_ReturnType LibLidarLms151_Configure( const ts_LibLidarLms151_Configuration * const ps_LibLidarLms151_Configuration)
{
    /* Declaration */
    Std_ReturnType t_Configuration_Status;

    /* Initialization */

    /* Function Body */
    if( (E_NOT_OK != gt_InitLibLidarLms151_Status) &&
		(NULL_PTR != ps_LibLidarLms151_Configuration))
    {
    	/*! Set Laser configuration */
		gs_configuration.u32_scaning_frequency=ps_LibLidarLms151_Configuration->s_LibLidarLms151_laserconf.u32_scaning_frequency;
    	gs_configuration.u32_angle_resolution=ps_LibLidarLms151_Configuration->s_LibLidarLms151_laserconf.u32_angle_resolution;
    	gs_configuration.s32_start_angle=ps_LibLidarLms151_Configuration->s_LibLidarLms151_laserconf.s32_start_angle;
    	gs_configuration.s32_stop_angle=ps_LibLidarLms151_Configuration->s_LibLidarLms151_laserconf.s32_stop_angle;

    	/*! Set data configuration */
    	gs_DataContentConfiguration.b_comment=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.b_comment;
    	gs_DataContentConfiguration.b_device_name=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.b_device_name;
    	gs_DataContentConfiguration.b_position=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.b_position;
    	gs_DataContentConfiguration.b_remission=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.b_remission;
    	gs_DataContentConfiguration.b_time=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.b_time;
    	gs_DataContentConfiguration.e_resolution=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.e_resolution;
    	memcpy(gs_DataContentConfiguration.au8_output_rate,ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.au8_output_rate , sizeof(ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.au8_output_rate) );
    	gs_DataContentConfiguration.u8_data_channel_0=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.u8_data_channel_0;
    	gs_DataContentConfiguration.u8_data_channel_1=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.u8_data_channel_1;
    	gs_DataContentConfiguration.u8_encoder_0=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.u8_encoder_0;
    	gs_DataContentConfiguration.u8_encoder_1=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.u8_encoder_1;
    	gs_DataContentConfiguration.u8_unit=ps_LibLidarLms151_Configuration->s_LibLidarLms151_dataconf.u8_unit;

    	/*! Set socket configuration */
    	memcpy(gs_socket_configuration.au8_serverAddress,ps_LibLidarLms151_Configuration->s_LibLidarLms151_socket.au8_serverAddress , sizeof(ps_LibLidarLms151_Configuration->s_LibLidarLms151_socket.au8_serverAddress) ) ;
		gs_socket_configuration.u32_server_port= ps_LibLidarLms151_Configuration->s_LibLidarLms151_socket.u32_server_port;
		gs_socket_configuration.b_blocking_mode = ps_LibLidarLms151_Configuration->s_LibLidarLms151_socket.b_blocking_mode;
		gs_socket_configuration.u32_timeout_sec = ps_LibLidarLms151_Configuration->s_LibLidarLms151_socket.u32_timeout_sec;

		t_Configuration_Status = E_OK ;
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Configuration_Status=E_NOT_OK;
    	ge_LibLidarLms151_error=E_LIBLMS151_ERR_NOT_CONFIGURED;
    }

    gt_ConfLibLidarLms151_Status=t_Configuration_Status;

    /* return code */
    return(t_Configuration_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Manage(void)
 * \brief this function is the central point of the library, it manage the laser
 *
 * \param  none
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType LibLidarLms151_Manage(void)
{
    /* Declaration */
    Std_ReturnType t_Manage_Status;
	struct timespec clock1, clock2;

    /* Initialization */
    te_ReadStatus e_ReadStatus;
    te_WriteStatus e_WriteStatus;
	t_Manage_Status=E_NOT_OK;

    /* Reset new data */
    gs_LibLidarLms151_scan_data.b_is_new_data = K_FALSE ;

    /* Function Body */
    /*! Check Module is Initialized and configured */
    if((E_NOT_OK!=gt_InitLibLidarLms151_Status)&&
		(E_NOT_OK!=gt_ConfLibLidarLms151_Status))
    {
    	switch(ge_InternalStateMachine)
    	{
			case E_STATE_LMS151_INITIALIZATION :
												/*! Module Initialized, it has to be configured */
												/*! set ge_InternalStateMachine to E_LIBLMS151_STATUS_CONFIGURATION */
												/*! set ge_Mode to E_MODE_LOGIN */
												/*! set ge_SubState to E_IDLE */
												ge_InternalStateMachine = E_STATE_LMS151_CONFIGURATION;
												ge_Mode=E_MODE_LOGIN;
												ge_SubState=E_IDLE;
											    t_Manage_Status=E_OK;
												break;
			case E_STATE_LMS151_UNDEFINED :
												t_Manage_Status=E_NOT_OK;
												break;
			case E_STATE_LMS151_CONFIGURATION :
			    								t_Manage_Status=E_OK;
												switch(ge_Mode)
												{
													case E_MODE_LOGIN:
																switch (ge_SubState)
																{
																	case E_IDLE :	/*! log into laser for parameters modifications */
																					t_Manage_Status=lms151_logIn_set(&e_WriteStatus);
																					if(E_NOT_OK!=t_Manage_Status)
																					{
																						if (E_WRITE_FINISH==e_WriteStatus)
																						{
																							ge_SubState=E_READ;
																						}
																						else
																						{
																							if (E_WRITE_ERROR==e_WriteStatus)
																							{
																								ge_SubState=E_ERROR;
																							}
																							else
																							{
																								 ;/*! Do Nothing */
																							}
																						}
																					}
																					break;
																	case E_READ :	/*! Check login command has been well sent */
																					t_Manage_Status=lms151_logIn_check(&e_ReadStatus);
																					if(E_NOT_OK!=t_Manage_Status)
																					{
																						/*! if check is ok, set ge_mode to E_MODE_SET_FREQUENCY_AND_RESOLUTION and ge_SubState to E_IDLE */
																						if (E_READ_FINISH==e_ReadStatus)
																						{
																							ge_SubState=E_IDLE;
#ifdef K_CONFIGURATION_DESACTIVATED
																							ge_Mode=E_MODE_START_MEAS;
#else
																							ge_Mode=E_MODE_SET_FREQUENCY_AND_RESOLUTION;
#endif
																						}
																						else
																						{
																							 ;
																						}
																					}
																					else
																					{
																						ge_SubState=E_ERROR;
																					}
																					break;
																	case E_ERROR :
																	default :
																					ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
																			    	ge_LibLidarLms151_error=E_LIBLMS151_ERR_LOGIN;
																			    	t_Manage_Status=E_NOT_OK;
																					break;
																}
																break;
													case E_MODE_SET_FREQUENCY_AND_RESOLUTION:
														switch (ge_SubState)
														{
															case E_IDLE :	/*! Configure laser Frequency and resolution */
																			t_Manage_Status=lms151_FrequencyAndResolution_set(&e_WriteStatus);
																			if(E_NOT_OK!=t_Manage_Status)
																			{
																				if (E_WRITE_FINISH==e_WriteStatus)
																				{
																					ge_SubState=E_READ;
																					gu32_FramePosition=0;
																				}
																				else
																				{
																					if (E_WRITE_ERROR==e_WriteStatus)
																					{
																						ge_SubState=E_ERROR;
																					}
																					else
																					{
																						 ;/*! Do Nothing */
																					}
																				}
																			}
																			else
																			{
																				;
																			}
																			break;
															case E_READ :	/*! Check laser frequency and resolution configuration well done */
																			t_Manage_Status=lms151_FrequencyAndResolution_check(&e_ReadStatus);
																			if(E_NOT_OK!=t_Manage_Status)
																			{
																				if (E_READ_FINISH==e_ReadStatus)
																				{
																					/*! if check is ok, set ge_mode to E_MODE_CONFIGURE_SCAN_DATA_CONTENT and ge_SubState to E_IDLE */
																					ge_SubState=E_IDLE;
																					ge_Mode=E_MODE_CONFIGURE_SCAN_DATA_CONTENT;
																				}
																				else
																				{
																					 ;
																				}
																			}
																			else
																			{
																				ge_SubState=E_ERROR;
																			}
																			break;
															case E_ERROR :
															default :
																			ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
																	    	ge_LibLidarLms151_error=E_LIBLMS151_ERR_SET_FREQUENCY_AND_RESOLUTION;
																	    	t_Manage_Status=E_NOT_OK;
																			break;
														}
														break;
													case E_MODE_CONFIGURE_SCAN_DATA_CONTENT:
														switch (ge_SubState)
														{
															case E_IDLE :   /*! Configure scan data content */
																			t_Manage_Status=lms151_configure_ScanDataContent_set(&e_WriteStatus);
																			if(E_NOT_OK!=t_Manage_Status)
																			{
																				if (E_WRITE_FINISH==e_WriteStatus)
																				{
																					ge_SubState=E_READ;
																					gu32_FramePosition=0;
																				}
																				else
																				{
																					if (E_WRITE_ERROR==e_WriteStatus)
																					{
																						ge_SubState=E_ERROR;
																					}
																					else
																					{
																						 ;/*! Do Nothing */
																					}
																				}
																			}
																			break;
															case E_READ : 	/*! Check scan data content configuration has been well done */
																			t_Manage_Status=lms151_configure_ScanDataContent_check(&e_ReadStatus);
																			if(E_NOT_OK!=t_Manage_Status)
																			{
																				if (E_READ_FINISH==e_ReadStatus)
																				{
																					/*! if check is ok, set ge_mode to E_MODE_CONFIGURE_SCAN_DATA_OUTPUT and ge_SubState to E_IDLE */
																					ge_SubState=E_IDLE;
																					ge_Mode=E_MODE_CONFIGURE_SCAN_DATA_OUTPUT;
																				}
																				else
																				{
																					 ;
																				}
																			}
																			else
																			{
																				ge_SubState=E_ERROR;
																			}
																			break;
															case E_ERROR :
															default :
																			ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
																	    	ge_LibLidarLms151_error=E_LIBLMS151_ERR_CONFIGURE_SCAN_DATA_CONTENT;
																	    	t_Manage_Status=E_NOT_OK;
																			break;
														}
														break;
													case E_MODE_CONFIGURE_SCAN_DATA_OUTPUT:
														switch (ge_SubState)
														{
															case E_IDLE :	/*! configure laser scan data output */
																			t_Manage_Status=lms151_configure_ScanDataOutput_set(&e_WriteStatus);
																			if(E_NOT_OK!=t_Manage_Status)
																			{
																				if (E_WRITE_FINISH==e_WriteStatus)
																				{
																					ge_SubState=E_READ;
																					gu32_FramePosition=0;
																				}
																				else
																				{
																					if (E_WRITE_ERROR==e_WriteStatus)
																					{
																						ge_SubState=E_ERROR;
																					}
																					else
																					{
																						 ;/*! Do Nothing */
																					}
																				}
																			}
																			break;
															case E_READ :	/*! check scan data output laser command has been well sent */
																			t_Manage_Status=lms151_configure_ScanDataOutput_check(&e_ReadStatus);
																			if(E_NOT_OK!=t_Manage_Status)
																			{
																				if (E_READ_FINISH==e_ReadStatus)
																				{
																					/*! if check is ok, set ge_mode to E_MODE_STORE_PARAMETERS and ge_SubState to E_IDLE */
																					ge_SubState=E_IDLE;
																					ge_Mode=E_MODE_STORE_PARAMETERS;
																				}
																				else
																				{
																					 ;
																				}
																			}
																			else
																			{
																				ge_SubState=E_ERROR;
																			}
																			break;
															case E_ERROR :
															default :
																			ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
																	    	ge_LibLidarLms151_error=E_LIBLMS151_ERR_CONFIGURE_SCAN_DATA_OUTPUT;
																	    	t_Manage_Status=E_NOT_OK;
																			break;
														}
														break;
													case E_MODE_STORE_PARAMETERS:
														switch (ge_SubState)
														{
															case E_IDLE :   /*! Store parameters in laser eeprom */
																			t_Manage_Status=lms151_store_parameters_set(&e_WriteStatus);
																			if(E_NOT_OK!=t_Manage_Status)
																			{
																				if (E_WRITE_FINISH==e_WriteStatus)
																				{
																					ge_SubState=E_READ;
																					gu32_FramePosition=0;
																				}
																				else
																				{
																					if (E_WRITE_ERROR==e_WriteStatus)
																					{
																						ge_SubState=E_ERROR;
																					}
																					else
																					{
																						 ;/*! Do Nothing */
																					}
																				}
																			}
																			break;
															case E_READ :	/* check store parameters command has been well sent */
																			t_Manage_Status=lms151_store_parameters_check(&e_ReadStatus);
																			if(E_NOT_OK!=t_Manage_Status)
																			{
																				if (E_READ_FINISH==e_ReadStatus)
																				{
																					/*! if check is ok, set ge_mode to E_MODE_START_MEAS and ge_SubState to E_IDLE */
																					ge_SubState=E_IDLE;
																					ge_Mode=E_MODE_START_MEAS;
																				}
																				else
																				{
																					 ;
																				}
																			}
																			else
																			{
																				ge_SubState=E_ERROR;
																			}
																			break;
															case E_ERROR :
															default :
																			ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
																	    	ge_LibLidarLms151_error=E_LIBLMS151_ERR_STORE_PARAMETERS;
																	    	t_Manage_Status=E_NOT_OK;
																			break;
														}
														break;

												case E_MODE_START_MEAS:
													switch (ge_SubState)
													{
														case E_IDLE :	/*! Start Measurements */
																		t_Manage_Status=lms151_StartMeas_set(&e_WriteStatus);
																		if(E_NOT_OK!=t_Manage_Status)
																		{
																			if (E_WRITE_FINISH==e_WriteStatus)
																			{
																				ge_SubState=E_READ;
																				gu32_FramePosition=0;
																			}
																			else
																			{
																				if (E_WRITE_ERROR==e_WriteStatus)
																				{
																					ge_SubState=E_ERROR;
																				}
																				else
																				{
																					 ;/*! Do Nothing */
																				}
																			}
																		}
																		break;
														case E_READ :	/*! Check start measurements command has been well sent */
																		t_Manage_Status=lms151_StartMeas_check(&e_ReadStatus);
																		if(E_NOT_OK!=t_Manage_Status)
																		{
																			if (E_READ_FINISH==e_ReadStatus)
																			{
																				/*! if check is ok, set ge_mode to E_MODE_ASK_STATUS and ge_SubState to E_IDLE and ge_InternalStateMachine to E_READY */
																				ge_SubState=E_IDLE;
																				ge_Mode=E_MODE_ASK_STATUS;
																				ge_InternalStateMachine=E_STATE_LMS151_READY;
																			}
																			else
																			{
																				 ;
																			}
																		}
																		else
																		{
																			ge_SubState=E_ERROR;
																		}
																		break;
														case E_ERROR :
														default :
																		ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
																    	ge_LibLidarLms151_error=E_LIBLMS151_ERR_START_MEAS;
																    	t_Manage_Status=E_NOT_OK;
																		break;
													}
													break;

//													case E_MODE_SET_TIMESTAMP:
//														switch (ge_SubState)
//														{
//															case E_IDLE :
//																			t_Manage_Status=lms151_Timestamp_set(&e_WriteStatus);
//																			if(E_NOT_OK!=t_Manage_Status)
//																			{
//																				if (E_WRITE_FINISH==e_WriteStatus)
//																				{
//																					ge_SubState=E_READ;
//																					gu32_FramePosition=0;
//																				}
//																				else
//																				{
//																					if (E_WRITE_ERROR==e_WriteStatus)
//																					{
//																						ge_SubState=E_ERROR;
//																					}
//																					else
//																					{
//																						 ;/*! Do Nothing */
//																					}
//																				}
//																			}
//																			break;
//															case E_READ :
//																			t_Manage_Status=lms151_Timestamp_check(&e_ReadStatus);
//																			if(E_NOT_OK!=t_Manage_Status)
//																			{
//																				if (E_READ_FINISH==e_ReadStatus)
//																				{
//																					ge_SubState=E_IDLE;
//																					ge_Mode=E_MODE_START;
//																				}
//																				else
//																				{
//																					 ;
//																				}
//																			}
//																			else
//																			{
//																				ge_SubState=E_ERROR;
//																			}
//																			break;
//															case E_ERROR :
//															default :
//																			ge_InternalStateMachine=E_LIBLMS151_STATUS_UNDEFINED;
//											    							ge_LibLidarLms151_error=E_LIBLMS151_ERR_SET_TIMESTAMP;
//											    							t_Manage_Status=E_NOT_OK;
//																			break;
//														}
//														break;

													case E_MODE_UNDEFINED:
													default :
												    	t_Manage_Status=E_NOT_OK;
														break;
												}
												break;
				case E_STATE_LMS151_READY :
								switch(ge_Mode)
								{
									case E_MODE_ASK_STATUS :
											switch (ge_SubState)
											{
												case E_IDLE :	/*! Ask laser state */
																t_Manage_Status=lms151_AskDeviceStatus_set(&e_WriteStatus);
																if(E_NOT_OK!=t_Manage_Status)
																{
																	if (E_WRITE_FINISH==e_WriteStatus)
																	{
																		ge_SubState=E_READ;
																		gu32_FramePosition=0;
																	}
																	else
																	{
																		if (E_WRITE_ERROR==e_WriteStatus)
																		{
																			ge_SubState=E_ERROR;
																		}
																		else
																		{
																			 ;/*! Do Nothing */
																		}
																	}
																}
																break;
												case E_READ :   /*! Check laser ask state command has been well sent */
																t_Manage_Status=lms151_AskDeviceStatus_check(&e_ReadStatus);
																if(E_NOT_OK!=t_Manage_Status)
																{
																	if (E_READ_FINISH==e_ReadStatus)
																	{
																		if (ge_DeviceStatus==E_LIBLMS151_STATUS_READY_FOR_MEASUREMENT)
																		{
																			/*! if check is ok, set ge_mode to E_MODE_LOGOUT and ge_SubState to E_IDLE */
																			ge_SubState=E_IDLE;
																			ge_Mode=E_MODE_LOGOUT;
																		}
																		else
																		{
																			ge_SubState=E_IDLE;
																		}
																	}
																	else
																	{
																		 ;
																	}
																}
																else
																{
																	ge_SubState=E_ERROR;
																}
																break;
												case E_ERROR :
												default :
																ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
																ge_LibLidarLms151_error=E_LIBLMS151_ERR_ASK_STATUS;
														    	t_Manage_Status=E_NOT_OK;
																break;
											}
											break;
										case E_MODE_LOGOUT:
											switch (ge_SubState)
											{
												case E_IDLE :  /*! log out to run laser */
																t_Manage_Status=lms151_logOut_set(&e_WriteStatus);
																if(E_NOT_OK!=t_Manage_Status)
																{
																	if (E_WRITE_FINISH==e_WriteStatus)
																	{
																		ge_SubState=E_READ;
																		gu32_FramePosition=0;
																	}
																	else
																	{
																		if (E_WRITE_ERROR==e_WriteStatus)
																		{
																			ge_SubState=E_ERROR;
																		}
																		else
																		{
																			 ;/*! Do Nothing */
																		}
																	}
																}
																break;
												case E_READ :	/*! Check log out command has been well sent */
																t_Manage_Status=lms151_logOut_check(&e_ReadStatus);
																if(E_NOT_OK!=t_Manage_Status)
																{
																	if (E_READ_FINISH==e_ReadStatus)
																	{
																		/*! if check is ok, set ge_mode to E_MODE_SCAN_CONTINUOUS and ge_SubState to E_IDLE and ge_InternalStateMachine to E_STATE_SCAN */
																		ge_SubState=E_IDLE;
																		ge_Mode=E_MODE_SCAN_CONTINUOUS;
																		ge_InternalStateMachine=E_STATE_LMS151_SCAN;
																	}
																	else
																	{
																		 ;
																	}
																}
																else
																{
																	ge_SubState=E_ERROR;
																}
																break;
												case E_ERROR :
												default :
																ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
																ge_LibLidarLms151_error=E_LIBLMS151_ERR_LOGOUT;
														    	t_Manage_Status=E_NOT_OK;
																break;
											}
											break;
										case E_MODE_UNDEFINED :
										default :
									    				t_Manage_Status=E_NOT_OK;
														break;
								}
								break;
			case E_STATE_LMS151_SCAN:
							switch(ge_Mode)
							{
								case E_MODE_SCAN_CONTINUOUS :
										switch (ge_SubState)
										{
											case E_IDLE :	/*! Configure permanent scan */
															t_Manage_Status=lms151_Permanent_Scan_set(&e_WriteStatus);
															if(E_NOT_OK!=t_Manage_Status)
															{
																if (E_WRITE_FINISH==e_WriteStatus)
																{
																	ge_SubState=E_READ;
																	gu32_FramePosition=0;
																}
																else
																{
																	if (E_WRITE_ERROR==e_WriteStatus)
																	{
																		ge_SubState=E_ERROR;
																	}
																	else
																	{
																		 ;/*! Do Nothing */
																	}
																}
															}
															break;
											case E_READ :   /*! check permanent scan well configured */
															t_Manage_Status=lms151_Permanent_Scan_check(&e_ReadStatus);
															if(E_NOT_OK!=t_Manage_Status)
															{
																if (E_READ_FINISH==e_ReadStatus)
																{
																	/*! if check is ok, set ge_mode to E_MODE_GET_DATA and ge_SubState to E_IDLE and gu32_FramePosition to 0 */
																	ge_SubState=E_IDLE;
																	ge_Mode=E_MODE_GET_DATA;
																	gu32_FramePosition=0;
																}
																else
																{
																	;
																}
															}
															else
															{
																ge_SubState=E_IDLE;
															}
															break;
											case E_ERROR :
											default :
														 	ge_LibLidarLms151_error=E_LIBLMS151_ERR_SCAN_CONTINUOUS;
															ge_InternalStateMachine=E_STATE_LMS151_UNDEFINED;
													    	t_Manage_Status=E_NOT_OK;
															break;
										}
										break;
									case E_MODE_GET_DATA :
											/*! Receive data from laser */
											t_Manage_Status=lms151_Scan_Receive();
											break;
									case E_MODE_UNDEFINED :
									default :
								    	    t_Manage_Status=E_NOT_OK;
											break;
							}
							break;
				default :
						break;
    	}
    }
    else
    {
    	/*! Module is not initialized, Halt */
    	t_Manage_Status=E_NOT_OK;
    }

    /* return code */
    return(t_Manage_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Open(void)
 * \brief this function will open Tcp Socket needed for communication with Lms151
 *
 * \param  none
 *
* \return Std_ReturnType :
*         - E_NOT_OK if module is not initialized or Tcp socket return an error
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Open(void)
{
    /* Declaration */     
    Std_ReturnType t_Open_Status;
	E_DRVTCPSOCKET_ERROR t_error ;

    /* Initialization */
	t_error=E_DRVTCPSOCKET_ERROR_CONNECT_INVALID_SOCKET_ERROR;

    /* Function Body */
    if(E_NOT_OK!=gt_InitLibLidarLms151_Status)
    {
    	t_Open_Status= DrvTcpSocket_Open( &gs_socket_configuration , &gs32_socket_desc , &t_error ) ;
    }
    else
    {
    	/*! Module is not initialized, Halt */
    	t_Open_Status=E_NOT_OK;
    }

    /* return code */
    return(t_Open_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Close(void) 
 * \brief this function will close Tcp Socket needed for communication with Lms151
 *
 * \param  none
 *
* \return Std_ReturnType :
*         - E_NOT_OK if module is not initialized or Tcp socket return an error
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Close(void) 
{
    /* Declaration */     
    Std_ReturnType t_Close_Status; 
	E_DRVTCPSOCKET_ERROR t_error ;

    /* Initialization */

    /* Function Body */
    if(E_NOT_OK!=gt_InitLibLidarLms151_Status)
    {
    	t_Close_Status = DrvTcpSocket_Close( &gs32_socket_desc , &t_error ) ;
    	if( NULL_PTR != fd_Lms )
    	{
    		fclose( fd_Lms ) ;
    	}
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Close_Status=E_NOT_OK; 
    }

    /* return code */
    return(t_Close_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Timestamp(uint32 *pu32_Timestamp)
 * \brief this function will return Laser data Timestamp
 *
 * \param  uint32 *pu32_Timestamp : timestamp parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pu32_Timestamp is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Timestamp(uint32 *const pu32_Timestamp)
{
    /* Declaration */     
    Std_ReturnType t_Get_Timestamp_Status; 

    /* Initialization */

    /* Function Body */
    if((E_NOT_OK!=gt_InitLibLidarLms151_Status)&&
	   (NULL_PTR!=pu32_Timestamp))
    {
    	t_Get_Timestamp_Status=E_OK; 
    	*pu32_Timestamp=gu32_Timestamp;
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Get_Timestamp_Status=E_NOT_OK; 
    }

    /* return code */
    return(t_Get_Timestamp_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Scan(ts_LibLidarLms151_scan_data *ps_LibLidarLms151_scan_data)
 * \brief this function will return scan values
 *
 * \param  ts_LibLidarLms151_scan_data *ps_LibLidarLms151_scan_data : scan parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if ps_LibLidarLms151_scan_data is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Scan(ts_LibLidarLms151_scan_data *const ps_LibLidarLms151_scan_data)
{
    /* Declaration */     
    Std_ReturnType t_Get_ScanDatas_Status; 

    /* Initialization */

    /* Function Body */
    if((E_NOT_OK!=gt_InitLibLidarLms151_Status)&&
		(NULL_PTR!=ps_LibLidarLms151_scan_data))
    {
    	memcpy(ps_LibLidarLms151_scan_data,&gs_LibLidarLms151_scan_data,sizeof(ts_LibLidarLms151_scan_data));
    	t_Get_ScanDatas_Status=E_OK; 
    	gs_LibLidarLms151_scan_data.b_is_new_data = K_FALSE ;
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Get_ScanDatas_Status=E_NOT_OK; 
    }

    /* return code */
    return(t_Get_ScanDatas_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_InternalState(te_InternalStateMachine *pe_State)
 * \brief this function will return internal state
 *
 * \param te_InternalStateMachine *pe_State : state parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pe_State is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_InternalState(te_InternalStateMachine *const pe_State)
{
    /* Declaration */
    Std_ReturnType t_GetState_Status;

    /* Initialization */

    /* Function Body */
    if((E_NOT_OK!=gt_InitLibLidarLms151_Status)&&
		(NULL_PTR!=pe_State))
    {
    	*pe_State=ge_InternalStateMachine;
    	t_GetState_Status=E_OK;
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_GetState_Status=E_NOT_OK;
    }

    /* return code */
    return(t_GetState_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Status(te_LibLidarLms151_device_status *pe_LibLidarLms151_device_status)
 * \brief this function will return laser status
 *
 * \param  te_LibLidarLms151_device_status *pe_LibLidarLms151_device_status : device status parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pe_LibLidarLms151_device_status is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Status(te_LibLidarLms151_device_status *const pe_LibLidarLms151_device_status)
{
    /* Declaration */
    Std_ReturnType t_GetState_Status;

    /* Initialization */

    /* Function Body */
    if((E_NOT_OK!=gt_InitLibLidarLms151_Status)&&
		(NULL_PTR!=pe_LibLidarLms151_device_status))
    {
    	*pe_LibLidarLms151_device_status=ge_DeviceStatus;
    	t_GetState_Status=E_OK;
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_GetState_Status=E_NOT_OK;
    }

    /* return code */
    return(t_GetState_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Mode(te_mode *pe_Mode)
 * \brief this function will return global mode
 *
 * \param  te_mode *pe_Mode : mode parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pe_Mode is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Mode(te_mode *const pe_Mode)
{
    /* Declaration */
    Std_ReturnType t_GetMode_Status;

    /* Initialization */

    /* Function Body */
    if((E_NOT_OK!=gt_InitLibLidarLms151_Status)&&
		(NULL_PTR!=pe_Mode))
    {
    	*pe_Mode=ge_Mode;
    	t_GetMode_Status=E_OK;
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_GetMode_Status=E_NOT_OK;
    }

    /* return code */
    return(t_GetMode_Status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Error(te_LibLidarLms151_error * const pe_LibLidarLms151_error)
 * \brief this function will return module errors
 *
 * \param  te_LibLidarLms151_error * const pe_LibLidarLms151_error : error pointer parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pe_LibLidarLms151_error is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Error(te_LibLidarLms151_error * const pe_LibLidarLms151_error)
{
    /* Declaration */
    Std_ReturnType t_GetError_Status;

    /* Initialization */

    /* Function Body */
    if(NULL_PTR!=pe_LibLidarLms151_error)
    {
    	*pe_LibLidarLms151_error=ge_LibLidarLms151_error;
    	t_GetError_Status=E_OK;
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_GetError_Status=E_NOT_OK;
    }

    /* return code */
    return(t_GetError_Status);
}




