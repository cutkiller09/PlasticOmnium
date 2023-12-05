#################################################################################################################################################### 
#! les '#!' correspondent a des commentaires
#! les '#' correspondent a des mots clefs
#! les '|' correspondent a des delimitateurs
#! !!!!! TRES IMPORTANT !!!!!
#! !!!!! NE PAS METTRE  LA DATE AU FORMAT XX/YY/DDDD!!!!!
#! !!!!! REMPLACER "=" par "\x3d"  "&" par "\x26" "/" par "\x2f"!!!!
#################################################################################################################################################### 

#Product
#$NAME
INVERTER

#Component
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

#Component
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

#FinTraitement 
