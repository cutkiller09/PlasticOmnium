        /*************************************************************************************/
        /* CASE $UNIT_$FUNCTION :  Nominal test case														*/
        t_main_Status=$UNIT_$FUNCTION();
        MACRO_ASSERT_TI( u16_i , t_main_Status, E_OK ) ;  
        u16_i++;

        /*************************************************************************************/
        /* CASE $UNIT_$FUNCTION :  Robustness test case														*/
        t_main_Status=$UNIT_$FUNCTION();
        MACRO_ASSERT_TI( u16_i , t_main_Status, E_NOT_OK ) ;  
        u16_i++;
  
