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
Service_Lidar

#Units
#$UNIT | $FUNCTION  
SrvLidar | Initialize  
SrvLidar | Configure
SrvLidar | Manage
SrvLidar | Get_Data
SrvLidar | Get_State
SrvLidar | Close
SrvLidar | Get_Error
#FinComponent

#Component_COM
#$NAME
Service_Configuration

#Units
#$UNIT | $FUNCTION  
srvConf | Manage 
srvConf | Get
srvConf | Set
srvConf | Initialize
srvConf | Get_Error
#FinComponent 

#Component_CORE
#$NAME
Lib_Toto

#Units
#$UNIT | $FUNCTION  
LibToto | Initialize  
LibToto | Configure
LibToto | Manage
LibToto | Get_Data
LibToto | Get_State
LibToto | Close
LibToto | Get_Error
#FinComponent

#Component_CORE
#$NAME
Lib_Titi

#Units
#$UNIT | $FUNCTION  
Lib_Titi | Manage 
Lib_Titi | Get
Lib_Titi | Set
Lib_Titi | Initialize
Lib_Titi | Get_Error
#FinComponent 

#Component_DRV
#$NAME
Driver_Can

#Units
#$UNIT | $FUNCTION  
DrvCan | Manage 
DrvCan | Get
DrvCan | Set
DrvCan | Initialize
DrvCan | Get_Error
#FinComponent

#Component_DRV
#$NAME
Driver_Eth

#Units
#$UNIT | $FUNCTION  
DrvEth | Manage 
DrvEth | Get
DrvEth | Set
DrvEth | Initialize
DrvEth | Get_Error
#FinComponent

#FinTraitement 
