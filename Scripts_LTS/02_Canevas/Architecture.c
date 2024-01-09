#################################################################################################################################################### 
#! les '#!' correspondent a des commentaires
#! les '#' correspondent a des mots clefs
#! les '|' correspondent a des delimitateurs
#! !!!!! TRES IMPORTANT !!!!!
#! !!!!! NE PAS METTRE  LA DATE AU FORMAT XX/YY/DDDD!!!!!
#! !!!!! REMPLACER "=" par "\x3d"  "&" par "\x26" "/" par "\x2f"!!!!
#################################################################################################################################################### 

#Product
#$NAME | $PROJECT_ID | $OEM
INVERTER | 1272489 | IVECO

#Component_COM
#$NAME
upg_app

#Units
#$UNIT | $FUNCTION  | $PARAMETER  | $ROLE 
Application | Init_Hook  | void
Application | Background_Hookgure | void
#FinComponent

#Component_COM
#$NAME
AC_Management

#Units
#$UNIT | $FUNCTION  | $PARAMETER | $ROLE 
MAc | eInit_Exe | void
MAc | eClainInProgress_Get  | void
MAc | eTask_Exe  | void
#FinComponent

#Component_COM
#$NAME
Uds_Server

#Units
#$UNIT | $FUNCTION | $PARAMETER | $ROLE 
UDSSVR | TpfAUds_iService | TstUdsSvr_Frame*, TstUdsSvr_Frame*, TU8 *session, TU8 *security
UDSSvr | getversion | void
UDSSvr | CreateSvr | TstUDSSvr_svr_handle** handle, TstUDSSvr_svr_setup *setup
UDSSvr | LoadCallBackFunc_Exe | TpfAUds_iService pService, TeiAUds_eSID ucSid
UDSSvr | DestroySvr | TstUDSSvr_svr_handle* handle
UDSSvr | Initstartlevel | TstUDSSvr_svr_handle* handle, TU8 u8initsession, TU8 u8initsecurity
UDSSvr | StartSvr | TstUDSSvr_svr_handle* handle
UDSSvr | StopSvr | TstUDSSvr_svr_handle* handle
UDSSvr | task  |void
UDSSvr | isIdle | TU8 indexHandle
UDSSvr | GetSvrSession | TstUDSSvr_svr_handle* handle, TU8 *session
#FinComponent

#Component_COM
#$NAME
Uds_App

#Units
#$UNIT | $FUNCTION | $PARAMETER | $ROLE  
UDSApp | eInit | TU8 u8initsession,TU8 u8initsecurity 
UDSApp | iMem_Init | TstListMemoryZone * pMemoryZone, TstListMemoryZone * pCyberMemoryZone
UDSApp | iMem_Erase | TstUdsSvr_Frame* in, TstUdsSvr_Frame *out, unsigned char *session, unsigned char *security
UDSApp | iMem_CheckDep | TstUdsSvr_Frame* in, TstUdsSvr_Frame *out, unsigned char *session, unsigned char *security
UDSApp | iMem_ReqDownLoad | TstUdsSvr_Frame* in, TstUdsSvr_Frame *out, unsigned char *session, unsigned char *security
UDSApp | iMem_DataTransReq | TstUdsSvr_Frame* in, TstUdsSvr_Frame *out, unsigned char *session, unsigned char *security
UDSApp | iMem_DataTransfExit | TstUdsSvr_Frame* in, TstUdsSvr_Frame *out, unsigned char *session, unsigned char *security
#FinComponent

#Component_COM
#$NAME
Rdbi_App

#Units
#$UNIT | $FUNCTION | $PARAMETER 
ARdbi | iRdbi_Exe | TstUdsSvr_Frame* in, TstUdsSvr_Frame *out, TU8 *session 
ARdbi | iWdbiExe | TU8* Fpu8FrameResponse, TU16 Fu16DataIdSet, TU16 Fu16BufferResponseSize, TU16 *Fpu16ResponseLength 
#FinComponent



#Component_DRV
#$NAME
Drv_Can

#Units
#$UNIT | $FUNCTION  | $PARAMETER | $ROLE 
DCan | eInit_Exe| TU16 Fu16CtrlId
DCan | eConfigBaudRate_Exe|TU16 Fu16CtrlId,TU16 Fu16Baudrate
DCan | eSetBitTime_Exe|TU16 Fu16CtrlId , TU32 Fu32BitTime
DCan | eInitialisationEvents_Exe| TU16 Fu16CtrlId, TU16 Fu16ProtoColId,
DCan | eSetupGlobalIrq_Exe| TU16 Fu16CtrlId,TU16 Fu16EnableMask
DCan | eMaskGlobalIrq_Exe| TU16 Fu16CtrlId
DCan | eRestoreGlobalIrq_Exe| TU16 Fu16CtrlId
DCan | eRXMask_Set|TU16 Fu16CtrlId,TU16 Fu16ObjType, TU32 Fu32RxMask
DCan | eRXMask_Get|TU16 Fu16CtrlId , TU16 Fu16ObjType
DCan | eAssignProtocol_Exe|TU16 Fu16CtrlId, TU16 Fu16MBId, TU16 Fu16ProtoColId
DCan | eSetupMB_Exe| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eClearMB_Exe| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eSetupMBIrqEnable_Exe| TU16 Fu16CtrlId, TU16 Fu16MBId, TU16 Fu16Enabled
DCan | eMBPutData_Exe| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eMBGetData_Exe| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eReadMBIdent_Exe| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eMBState_Get| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eMBTx_Req| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eMBTxAbort_Exe| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eMBRemote_Req| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eMBRemoteAbort_Exe| TU16 Fu16CtrlId, TU16 Fu16MBId
DCan | eBusOff_Exe| TU16 Fu16CtrlId
DCan | eBusOn_Exe| TU16 Fu16CtrlId
DCan | eBusOffState_Get|TU16 Fu16CtrlId
DCan | eError_Get| TU16 Fu16CtrlId
DCan | eMaxMBNumber_Set| TU16 Fu16CtrlId, TU16 Fu16MBNb
DCan | eMaxMBNumber_Get| TU16 Fu16CtrlId 
DCan | eWriteMBIdent_Exe|TU16 Fu16CtrlId, TU16 Fu16MBId, TU32 Fu32Ident
#FinComponent

#FinTraitement
