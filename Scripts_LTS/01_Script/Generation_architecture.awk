#! /usr/bin/awk -f

BEGIN {
    FS="|" #Changement du s�parateur de champ
} 
{ 
   gsub(/(^ *)|( *$)/,"",$1)  # Suppression des espaces
   
	Dossier_Source = "../03_Project_Skeleton" 	
	Dossier_Doc = Dossier_Source"/01_Elements"
	Dossier_SWE1_Specification = Dossier_Doc"/01_SWE1_Specification"
	Dossier_SWE2_Architecture = Dossier_Doc"/02_SWE2_Architecture" 
	Dossier_SWE5_Integration_Tests = Dossier_Doc"/03_SWE5_Integration_Tests" 
	Dossier_SWE6_Qualification_Tests = Dossier_Doc"/04_SWE6_Qualification_Tests"
	Dossier_Code = Dossier_Source"/02_Components" 
	Dossier_Settings = Dossier_Source"/03_Settings"	 
	Dossier_Libs = Dossier_Source"/04_Libs"	 
	Dossier_High = Dossier_Code"/01_HighLevel" 	
	Dossier_Mid = Dossier_Code"/02_Middleware" 	
	Dossier_Drv = Dossier_Code"/03_Driver" 	

   while ($1 != "#FinTraitement")
   {     
		if ($1 == "#Product")   
        {    
            getline 
            getline 
			
	        #$NAME  
            Product_Name=$1
            gsub(/(^ *)|( *$)/,"",Product_Name)  	# Suppression des espaces  
            Project_ID=$2
            gsub(/(^ *)|( *$)/,"",Project_ID)  	# Suppression des espaces  
            Oem=$3
            gsub(/(^ *)|( *$)/,"",Oem)  	# Suppression des espaces  
			
			system("mkdir " Dossier_Source)	
			system("mkdir " Dossier_Doc)	
			system("mkdir " Dossier_Code)	
			system("mkdir " Dossier_Settings)	
			system("mkdir " Dossier_SWE1_Specification)	
			system("mkdir " Dossier_SWE2_Architecture)	
			system("mkdir " Dossier_SWE6_Qualification_Tests)
			system("mkdir " Dossier_SWE6_Qualification_Tests"/doc")	 
			system("mkdir " Dossier_SWE5_Integration_Tests)	  
			system("mkdir " Dossier_SWE5_Integration_Tests"/doc")	
			system("mkdir " Dossier_SWE5_Integration_Tests"/src")	
			system("mkdir " Dossier_SWE5_Integration_Tests"/inc")	
			system("mkdir " Dossier_SWE5_Integration_Tests"/stubs")	 
			system("mkdir " Dossier_SWE2_Architecture"/images")	 	 
			system("mkdir " Dossier_High)		 
			system("mkdir " Dossier_Mid)		 
			system("mkdir " Dossier_Drv)		 
			 
			system ("cp Templates/*SWQT_PROD*.xlsx "Dossier_SWE6_Qualification_Tests"/SWQT_"Product_Name"_"Oem"_"Project_ID".xlsx")	
			system ("cp Templates/*SWQT_summary_PROD*.xlsx "Dossier_SWE6_Qualification_Tests"/SWQT_summary_"Product_Name"_"Oem"_"Project_ID".xlsx")							
			system ("cp Templates/*SWE6.docx " Dossier_SWE6_Qualification_Tests)						
			system ("cp Templates/TEMPLATE_SWAD*.docx "Dossier_SWE2_Architecture"/SWAD_"Product_Name"_"Oem"_"Project_ID".docx")	 
			system ("cp Templates/*SWRMP*.docx "Dossier_SWE1_Specification"/SWRMP_"Product_Name"_"Oem"_"Project_ID".docx")
			system ("cp Templates/*SWE6.docx " Dossier_SWE5_Integration_Testss"/doc/")						
			system ("cp Templates/SoftwareTestPlan.xlsm " Dossier_SWE5_Integration_Tests"/doc/SWE5_Integration_Tests_Plan_"Product_Name"_"Oem"_"Project_ID".xlsm")			
			system ("cp Templates/Global_Review_Checklist_V1.0.xlsm " Dossier_SWE5_Integration_Tests"/doc/SWE5_Checklist_Review_"Product_Name"_"Oem"_"Project_ID".xlsm")
			system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/CMakeLists_PROJECT.txt >" Dossier_Source"/CMakeLists.txt" )
		
		}		  
		else
		{   
			if ($1 == "#Component_HighLevel")   
			{    
				getline 
				getline 
				
				#$NAME 
				Component_Name=$1
				gsub(/(^ *)|( *$)/,"",Component_Name)  	# Suppression des espaces    

				#Creation du fichier Source			  
				Dossier_Test_TU = "test/01_SWE4_Units_Tests"
				Composant = Component_Name".c"  	
				Dossier_Component = Dossier_High"/"Component_Name 
				Dossier_Units_tests = Dossier_Component"/"Dossier_Test_TU
				Dossier_Units_tests_doc = Dossier_Units_tests"/doc"
				Dossier_Lib_tests = Dossier_Component"/test/lib" 
				system("mkdir " Dossier_Component)	
				system("mkdir " Dossier_Component"/doc")
				system("mkdir " Dossier_Component"/doc/Doxygen")
				system("mkdir " Dossier_Component"/inc")
				system("mkdir " Dossier_Component"/src")
				system("mkdir " Dossier_Component"/test")
				system("mkdir " Dossier_Lib_tests)
				system("mkdir " Dossier_Lib_tests"/inc")
				system("mkdir " Dossier_Lib_tests"/src") 
				system("mkdir " Dossier_Units_tests)
				system("mkdir " Dossier_Units_tests"/stubs")
				system("mkdir " Dossier_Units_tests"/stubs/inc")
				system("mkdir " Dossier_Units_tests"/stubs/src")
				system("mkdir " Dossier_Units_tests"/inc")
				system("mkdir " Dossier_Units_tests"/src")
				system("mkdir " Dossier_Units_tests_doc)
				system("mkdir " Dossier_Units_tests_doc"/coverage")
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Unit.c >" Dossier_Component"/src/"Component_Name".c" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Unit.h >" Dossier_Component"/inc/"Component_Name".h" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$NAME/"Product_Name"/g' Templates/CMakeLists_COMPONENT.txt >" Dossier_Code"/CMakeLists.txt" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/CMakeLists_UNIT.txt >" Dossier_Component"/CMakeLists.txt" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Doxyfile.in >" Dossier_Component"/Doxyfile.in" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Readme >" Dossier_Component"/Readme" )		
				system ("cp Templates/*SWDD*.docx " Dossier_Component"/doc/SWDD_"Product_Name"_"Oem"_"Project_ID".docx")	 
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Test_Template.c >" Dossier_Units_tests"/src/TU_"Component_Name".c" )
				system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/CoverageReport.sh >" Dossier_Component"/CoverageReport.sh" )
				system ("cp Templates/Test_Report.c " Dossier_Lib_tests"/src" )			
				system ("cp Templates/Test_Report.h " Dossier_Lib_tests"/inc" )							 						 
			}     
			else
			{          

				if ($1 == "#Units")   
				{  
					getline 				
					getline 
					while ($1 != "#FinComponent") 
					{      

						#$UNIT | $FUNCTION  | $PARAMETER   
						Unit=$1     
						gsub(/(^ *)|( *$)/,"",Unit)  	# Suppression des espaces  
						Function=$2 	
						gsub(/(^ *)|( *$)/,"",Function)  	# Suppression des espaces  
						Parameter=$3
						gsub(/(^ *)|( *$)/,"",Parameter)  	# Suppression des espaces  
						
						system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' -e 's/$PARAMETER/"Parameter"/g' Templates/Template_Function.c >>" Dossier_Component"/src/"Component_Name".c" )		 	
						system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' -e 's/$PARAMETER/"Parameter"/g' Templates/Test_Body_Template.c >>" Dossier_Units_tests"/src/TU_"Component_Name".c" )					
						system ("echo 'Std_ReturnType "Unit"_"Function"(void);' >>" Dossier_Component"/inc/"Component_Name".h") 
						getline 
					}				 						
					system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' Templates/Test_End_Template.c >>" Dossier_Units_tests"/src/TU_"Component_Name".c" )		
					system ("cp Templates/Global_Review_Checklist_V1.0.xlsm " Dossier_Units_tests"/doc/SWE4_Checklist_Review_"Product_Name"_"Oem"_"Project_ID".xlsm")
					system ("cp Templates/SoftwareTestPlan.xlsm " Dossier_Units_tests"/doc/SWE4_Units_Tests_Plan_"Product_Name"_"Composant"_"Oem"_"Project_ID".xlsm")						
					system ("cp Templates/*SWE4.docx " Dossier_Units_tests"/doc/")						
				}    
				else
				{ 		
					if ($1 == "#Component_Middleware")   
					{    
						getline 
						getline 
						
						#$NAME 
						Component_Name=$1
						gsub(/(^ *)|( *$)/,"",Component_Name)  	# Suppression des espaces    

						#Creation du fichier Source			 
						Composant = Component_Name".c"  
						Dossier_Base = Dossier_Mid"/Base" 	 
						Dossier_Component = Dossier_Mid"/"Component_Name
						Dossier_Integration_tests = Dossier_Component"/test/02_SWE5_Integration_Tests"
						Dossier_Units_tests = Dossier_Component"/test/01_SWE4_Units_Tests" 
						Dossier_Lib_tests = Dossier_Component"/test/lib" 
						system("mkdir "Dossier_Base)	 		 	 
						system("mkdir "Dossier_Base"/inc")	 		 	 
						system("mkdir "Dossier_Base"/src")		 		 	 
						system("mkdir "Dossier_Component)	
						system("mkdir "Dossier_Component"/doc")
						system("mkdir "Dossier_Component"/inc")
						system("mkdir "Dossier_Component"/src")
						system("mkdir "Dossier_Component"/test")
						system("mkdir " Dossier_Lib_tests) 
						system("mkdir " Dossier_Lib_tests"/inc")
						system("mkdir " Dossier_Lib_tests"/src") 
						system("mkdir " Dossier_Units_tests)
						system("mkdir " Dossier_Units_tests"/stubs")
						system("mkdir " Dossier_Units_tests"/stubs/inc")
						system("mkdir " Dossier_Units_tests"/stubs/src")
						system("mkdir " Dossier_Units_tests"/inc")
						system("mkdir " Dossier_Units_tests"/src")
						system("mkdir " Dossier_Units_tests_doc)
						system("mkdir " Dossier_Units_tests_doc"/coverage")
						system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Unit.c >" Dossier_Component"/src/"Component_Name".c" )
						system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Unit.h >" Dossier_Component"/inc/"Component_Name".h" )
						system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/CMakeLists_UNIT.txt >" Dossier_Component"/CMakeLists.txt" )
						system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Doxyfile.in >" Dossier_Component"/Doxyfile.in" )
						system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Readme >" Dossier_Component"/Readme" )		
						system ("cp Templates/*SWDD*.docx " Dossier_Component"/doc/SWDD_"Product_Name"_"Oem"_"Project_ID".docx")		  
						system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Test_Template.c >" Dossier_Units_tests"/src/TU_"Component_Name".c" )	
						system ("cp Templates/Test_Report.c " Dossier_Lib_tests"/src" )			
						system ("cp Templates/Test_Report.h " Dossier_Lib_tests"/inc" )			 		
						system ("cp Templates/main_types.h " Dossier_Base"/inc" )						 						 					 						 
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
								Parameter=$3 	 
								gsub(/(^ *)|( *$)/,"",Parameter)  	# Suppression des espaces  
								
								system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' -e 's/$PARAMETER/"Parameter"/g' Templates/Template_Function.c >>" Dossier_Component"/src/"Component_Name".c" )		 		
								system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' -e 's/$PARAMETER/"Parameter"/g' Templates/Test_Body_Template.c >>" Dossier_Units_tests"/src/TU_"Component_Name".c" )					
								system ("echo 'Std_ReturnType "Unit"_"Function"(void);' >>" Dossier_Component"/inc/"Component_Name".h") 
								getline 
							}				 							
							system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' Templates/Test_End_Template.c >>" Dossier_Units_tests"/src/TU_"Component_Name".c" )								
						}
						else
						{
							if ($1 == "#Component_Driver")   
							{    
								getline 
								getline 
								
								#$NAME 
								Component_Name=$1
								gsub(/(^ *)|( *$)/,"",Component_Name)  	# Suppression des espaces    

								#Creation du fichier Source			 
								Composant = Component_Name".c"   
								Dossier_Component = Dossier_Drv"/"Component_Name
								Dossier_Integration_tests = Dossier_Component"/test/02_SWE5_Integration_Tests"
								Dossier_Units_tests = Dossier_Component"/test/01_SWE4_Units_Tests" 
								Dossier_Lib_tests = Dossier_Component"/test/lib"   
								system("mkdir "Dossier_Component)	
								system("mkdir "Dossier_Component"/doc")
								system("mkdir "Dossier_Component"/inc")
								system("mkdir "Dossier_Component"/src")
								system("mkdir "Dossier_Component"/test")
								system("mkdir " Dossier_Lib_tests) 
								system("mkdir " Dossier_Lib_tests"/inc")
								system("mkdir " Dossier_Lib_tests"/src") 
								system("mkdir " Dossier_Units_tests)
								system("mkdir " Dossier_Units_tests"/stubs")
								system("mkdir " Dossier_Units_tests"/stubs/inc")
								system("mkdir " Dossier_Units_tests"/stubs/src")
								system("mkdir " Dossier_Units_tests"/inc")
								system("mkdir " Dossier_Units_tests"/src")
								system("mkdir " Dossier_Units_tests_doc)
								system("mkdir " Dossier_Units_tests_doc"/coverage")
								system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Unit.c >" Dossier_Component"/src/"Component_Name".c" )
								system ("sed -e 's/$COMPONENT/"Component_Name"/g' -e 's/$FUNCTION/"Function"/g' Templates/Template_Unit.h >" Dossier_Component"/inc/"Component_Name".h" )
								system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/CMakeLists_UNIT.txt >" Dossier_Component"/CMakeLists.txt" )
								system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Doxyfile.in >" Dossier_Component"/Doxyfile.in" )
								system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Readme >" Dossier_Component"/Readme" )		
								system ("cp Templates/*SWDD*.docx " Dossier_Component"/doc/SWDD_"Product_Name"_"Oem"_"Project_ID".docx")		  
								system ("sed -e 's/$COMPONENT/"Component_Name"/g' Templates/Test_Template.c >" Dossier_Units_tests"/src/TU_"Component_Name".c" )	
								system ("cp Templates/Test_Report.c " Dossier_Lib_tests"/src" )			
								system ("cp Templates/Test_Report.h " Dossier_Lib_tests"/inc" )	 					 						 					 						 
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
										Parameter=$3 	 
										gsub(/(^ *)|( *$)/,"",Parameter)  	# Suppression des espaces  
										
										system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' -e 's/$PARAMETER/"Parameter"/g' Templates/Template_Function.c >>" Dossier_Component"/src/"Component_Name".c" )		 		
										system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' -e 's/$PARAMETER/"Parameter"/g' Templates/Test_Body_Template.c >>" Dossier_Units_tests"/src/TU_"Component_Name".c" )					
										system ("echo 'Std_ReturnType "Unit"_"Function"(void);' >>" Dossier_Component"/inc/"Component_Name".h") 
										getline 
									}				 							
									system ("sed -e 's/$UNIT/"Unit"/g' -e 's/$FUNCTION/"Function"/g' Templates/Test_End_Template.c >>" Dossier_Units_tests"/src/TU_"Component_Name".c" )								
								}
							}
						}
					}		
				}    
			}	

		}
		getline
		gsub(/(^ *)|( *$)/,"",$1)  # Suppression des espaces   

   }

#system ("ls -R ../03_Project_Skeleton | grep : | grep inc | cut -f'1' -d':' | grep -v test > Headers.txt")
#system ("ls -R ../03_Project_Skeleton | grep : | grep src | cut -f'1' -d':' | grep -v test > Sources.txt")
#../03_Project_Skeleton/02_Components/01_HighLevel/Service_Configuration/inc\\r\\n../03_Project_Skeleton/02_Components/01_HighLevel/Service_Configuration/test/01_SWE4_Units_Tests/inc
    
#file2=Dossier_High"/CMakeLists2.txt"
#file1=Dossier_High"/CMakeLists.txt"
 
#system ("awk 'NR==FNR{rep=(NR>1?rep RS:"") $0; next} {gsub(/%HEADERS/,rep)}1' Headers.txt "file1" > tmp && mv tmp "file1)
#system ("awk 'NR==FNR{rep=(NR>1?rep RS:"") $0; next} {gsub(/%SOURCES/,rep)}1' Sources.txt "file1" > tmp && mv tmp "file1)
     
}

