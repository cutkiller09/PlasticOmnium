#! /usr/bin/awk -f

BEGIN {
    FS="|" #Changement du s�parateur de champ
} 
{
   gsub(/(^ *)|( *$)/,"",$1)  # Suppression des espaces

   while ($1 != "#FinTraitement")
   {     
		if ($1 == "#Product")   
        {    
            getline 
            getline 
			
	        #$NAME  
            Product_Name=$1
            gsub(/(^ *)|( *$)/,"",Product_Name)  	# Suppression des espaces  
			
			system("mkdir ../03_Generation_Source")	
			system("mkdir ../03_Generation_Source/01_Architecture")	
			system("mkdir ../03_Generation_Source/01_Architecture/images")	
			system("mkdir ../03_Generation_Source/02_Code")	
			
			system ("cp Templates/*SWAD*.docx  ../03_Generation_Source/01_Architecture/SWAD_"Product_Name"_OEM_PROJECTID.docx")		
		}		  
		else
		{   
			if ($1 == "#Component")   
			{    
				getline 
				getline 
				
				#$NAME 
				Component_Name=$1
				gsub(/(^ *)|( *$)/,"",Component_Name)  	# Suppression des espaces    

				#Creation du fichier Source			 
				Composant = Component_Name".c"  
				DossierSortie = "../03_Generation_Source/02_Code" 		 
				system("mkdir "DossierSortie"/"Component_Name)	
				system("mkdir "DossierSortie"/"Component_Name"/doc")
				system("mkdir "DossierSortie"/"Component_Name"/inc")
				system("mkdir "DossierSortie"/"Component_Name"/src")
				system("mkdir "DossierSortie"/"Component_Name"/test")
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Unit.c >" DossierSortie"/"Component_Name"/src/"Component_Name".c" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Unit.h >" DossierSortie"/"Component_Name"/inc/"Component_Name".h" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/CMakeLists.txt >" DossierSortie"/"Component_Name"/CMakeLists.txt" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Doxyfile.in >" DossierSortie"/"Component_Name"/Doxyfile.in" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Readme >" DossierSortie"/"Component_Name"/Readme" )		
				system ("cp Templates/*SWDD*.docx " DossierSortie"/"Component_Name"/doc/SWDD_"Component_Name"_OEM_PROJECTID.docx")		
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/TI_Template.c >" DossierSortie"/"Component_Name"/test/TI_"Component_Name".c" )						 						 
			}     
			else
			{          

				if ($1 == "#Units")   
				{  
					getline 				
					getline 
					while ($1 != "#FinComponent") 
					{      

						#$UNIT | $FUNCTION  
						Unit=$1     
						gsub(/(^ *)|( *$)/,"",Unit)  	# Suppression des espaces  
						Function=$2 	
						gsub(/(^ *)|( *$)/,"",Function)  	# Suppression des espaces  
						
						system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Function.c >>" DossierSortie"/"Component_Name"/src/"Component_Name".c" )					
						system ("echo 'Std_ReturnType "Unit"_"Function"(void);' >>" DossierSortie"/"Component_Name"/inc/"Component_Name".h")
						getline 
					}											
				}    
				else
				{ 			
				}    
			}	

		}
		getline
		gsub(/(^ *)|( *$)/,"",$1)  # Suppression des espaces   
   }
}
