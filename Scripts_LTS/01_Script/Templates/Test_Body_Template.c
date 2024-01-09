        /*************************************************************************************/
        /* CASE $UNIT_$FUNCTION :  Nominal test case	                                     */
        Prv_Init_Variables();								     
        t_main_Status=$UNIT_$FUNCTION($PARAMETER);
        MACRO_ASSERT_TEST( u16_i , t_main_Status, E_OK ) ;  
        u16_i++;

        /*************************************************************************************/
        /* CASE $UNIT_$FUNCTION :  Robustness test case					     */
        Prv_Init_Variables();		
        t_main_Status=$UNIT_$FUNCTION($PARAMETER));
        MACRO_ASSERT_TEST( u16_i , t_main_Status, E_NOT_OK ) ;  
        u16_i++;
  
