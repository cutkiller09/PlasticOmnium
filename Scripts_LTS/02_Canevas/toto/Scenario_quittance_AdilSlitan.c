/******************************************************************************/
/*                                                                            */
/* Copyright by     :   Diehl Avionik Systeme GmbH Ueberlingen                */
/*                                                                            */
/* Module Name      :   DMNG_A653_API_FUNCTIONS                               */
/*                                                                            */
/* Created By       :   M.J. Kingston                                         */
/*                                                                            */
/******************************************************************************/

/********************/
/* IMPORTED MODULES */
/********************/

#include "cod_dmng_a653_api_functions.h"
#include "cod_dmng_spi_global_data.h"
#include "cod_dmng_fmgr_dmng_api.h"

/*******************/
/* LOCAL CONSTANTS */
/*******************/

/***************/
/* LOCAL TYPES */
/***************/

/**************/
/* LOCAL DATA */
/**************/

/***************************/
/* EXPORT DATA DEFINITIONS */
/***************************/

/********************************/
/* LOCAL OPERATION DECLARATIONS */
/********************************/

/*******************************/
/* LOCAL OPERATION DEFINITIONS */
/*******************************/

/********************************/
/* EXPORT OPERATION DEFINITIONS */
/********************************/

/******************************************************************************/
/* Description:                                                               */
/*                                                                            */
/* Performs the ISSP_GET_BITE_MESSAGES SPI Service, returning the R-BITE      */
/* messages for the Instrumentation Services Partition.                       */
/*                                                                            */
/******************************************************************************/

T_A653_SVC_STATUS DMNG_MANAGE_ISSP_GET_BITE_MESSAGES (
   /* IN/OUT */ T_PTR const P_SVC_PARAM)
{
   /************/
   /* LOCALS   */
   /************/

   struct T_DMNG_SPI_ISSP_GET_BITE_MSGS_PARAM_BLOCK * p_Param_Block;
   T_A653_SVC_STATUS                                  e_Svc_Status;
   T_BOOL                                             b_Okay;
   T_UINT32                                           i_Service_Group;

   /************/
   /* BODY     */
   /************/

   /* Check to see if the specified parameter block is accessible in read/ */
   /* write mode by calling the SSI.A653_HBOX_IS_USER_ACCESS_AUTHORIZED    */
   /* service                                                              */

   b_Okay = A653_HBOX_IS_USER_ACCESS_AUTHORIZED (
      /* IN P_ADDRESS     */ (T_ADDR) P_SVC_PARAM,
      /* IN I_SIZE        */
         sizeof (struct T_DMNG_SPI_ISSP_GET_BITE_MSGS_PARAM_BLOCK),
      /* IN E_ACCESS_TYPE */ K_A653_READ_WRITE_ACCESS);

   /* If access to the parameter block is not authorised then report an */
   /* illegal access error via the return code                          */

   if (b_Okay EQ K_FALSE)
   {
      /* Paramter block access is not authorised */

      /* Set the service return code to report an illegal access error */

      e_Svc_Status = K_A653_SVC_ILLEGAL_ACCESS;

   }

   /* Else, the P_BUFFER check can be performed */

   else
   {

      /* Convert the specified parameter block into the appropriate     */
      /* service parameter block for this issp get bite message service */

      p_Param_Block =
         (struct T_DMNG_SPI_ISSP_GET_BITE_MSGS_PARAM_BLOCK *) P_SVC_PARAM;

      /* Check to see if the P_BUFFER output paramter from the       */
      /* parameter block is accessible in read/write mode by calling */
      /* the SSI.A653_HBOX_IS_USER_ACCESS_AUTHORIZED service         */

      b_Okay = A653_HBOX_IS_USER_ACCESS_AUTHORIZED (
         /* IN P_ADDRESS     */ (T_ADDR) p_Param_Block->P_BUFFER,
         /* IN I_SIZE        */ sizeof (T_A653_DRV_BITE_MESSAGE)
                                   *
                                p_Param_Block->I_NUM_MESSAGES,
         /* IN E_ACCESS_TYPE */ K_A653_READ_WRITE_ACCESS);

      /* If access to the P_BUFFER output paramter from the parameter */
      /* block is not authorised then report an illegal access error  */
      /* via the return code                                          */

      if (b_Okay EQ K_FALSE)
      {
         /* Paramter buffer access is not authorised */

         /* Report an illegal access error via the service */
         /* return code and the parameter block            */

         e_Svc_Status = K_A653_SVC_ILLEGAL_ACCESS;

      }

      /* Else, the PI_NUM_WRITTEN check can be performed */

      else
      {
         /* Paramter access is authorised */

         /* Check to see if the PI_NUM_WRITTEN output paramter from the */
         /* parameter block is accessible in read/write mode by calling */
         /* the SSI.A653_HBOX_IS_USER_ACCESS_AUTHORIZED service         */

         b_Okay = A653_HBOX_IS_USER_ACCESS_AUTHORIZED (
            /* IN P_ADDRESS     */ (T_ADDR) p_Param_Block->PI_NUM_WRITTEN,
            /* IN I_SIZE        */ sizeof (T_UINT32),
            /* IN E_ACCESS_TYPE */ K_A653_READ_WRITE_ACCESS);

         /* If access to the PI_NUM_WRITTEN output paramter from the */
         /* parameter block is not authorised then report an illegal */
         /* access error via the return code                         */

         if (b_Okay EQ K_FALSE)
         {
            /* Set the service return code to report an illegal access error */

            e_Svc_Status = K_A653_SVC_ILLEGAL_ACCESS;
         }

         /* Else, the service group check can be performed */

         else
         {
            /* Paramter access is authorised */

            /* Read the service permissions for the current partition    */
            /* by calling the SSI.A653_HBOX_GET_PERMITTED_SERVICE_GROUPS */
            /* service                                                   */

            i_Service_Group = A653_HBOX_GET_PERMITTED_SERVICE_GROUPS ();

            /* If the SSI.K_A653_IS_SERVICES bit is not set in the     */
            /* permitted service groups then a permission denied error */
            /* needs to be reported via the service return code        */

            if ((i_Service_Group Bitwise_AND K_A653_IS_SERVICES) EQ 0ul)
            {
               /* Service permission is not authorised */

               /* Report an illegal access error via the service */
               /* return code and the parameter block            */

               e_Svc_Status = K_A653_SVC_PERMISSION_DENIED;

            }

            /* Else, the current partition has permission to call */
            /* this service                                       */

            else
            {
               /* Service group access is authorised */

               /* The SPI service part has been processed without any errors */
               /* so set the service return code to report no error          */

               e_Svc_Status = K_A653_SVC_NO_ERROR;

               /* Call the Failure manager to get the Messages */

               FMGR_GET_IS_QUEUE (
                  /* IN  I_NUM_MESSAGES */ p_Param_Block->I_NUM_MESSAGES,
                  /* OUT P_BUFFER       */ p_Param_Block->P_BUFFER,
                  /* OUT PI_NUM_WRITTEN */ p_Param_Block->PI_NUM_WRITTEN);

            } /* End of else, service permission is authorised */

         } /* End of else, the service group check can be performed */

      } /* End of else parameter buffer access authorised */

   } /* end of else parameter access authorised */

   /* Return the final result via the service return code */

   return e_Svc_Status;

} /* End of DMNG_MANAGE_ISSP_GET_BITE_MESSAGES */
