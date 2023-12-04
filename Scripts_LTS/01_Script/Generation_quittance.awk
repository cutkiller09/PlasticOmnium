#! /usr/bin/awk -f

BEGIN {
    FS="|" #Changement du séparateur de champ
} 
{
   #gsub(/(^ *)|( *$)/,"",$1)  # Suppression des espaces

   while ($1 != "#FinTraitement")
   {    
        if ($1 == "#Donnees_Locataire")   
        {    
            getline 
            getline 
			
			#$PRENOM_LOCATAIRE | $NOM_LOCATAIRE | $DATE_ANNIVERSAIRE | $LIEU_NAISSANCE | $NATIONALITE 
            Prenom_Locataire=$1
            gsub(/(^ *)|( *$)/,"",Prenom_Locataire)  	# Suppression des espaces 
            Nom_Locataire=$2
            gsub(/(^ *)|( *$)/,"",Nom_Locataire)  		# Suppression des espaces 
            Date_Anniversaire=$3 
            Lieu_Naissance=$4  
            Nationalite=$5
            #gsub(/(^ *)|( *$)/,"",Nationalite) 			# Suppression des espaces  

			DossierSortie = "../03_Generation_Source" 			 
        }     
        else
        {        
			if ($1 == "#Donnees_bien")   
			{    
				getline 
				getline 

				#$ADRESSE_BIEN | $DESIGNATION | $TYPE 
				Adresse_Bien=$1     
				Designation=$2 	 			
			}    
			else
			{
				if ($1 == "#Duree")   
				{         
					getline  
					getline  
					
					#$JOUR_ENTREE | $MOIS_ENTREE | $ANNEE_ENTREE | $ANNEE_SORTIE_PREVUE | $DUREE_BAIL | $JOUR_SORTIE | $MOIS_SORTIE | $ANNEE_SORTIE_EFFECTIVE
					Jour_Entree=$1
					#gsub(/(^ *)|( *$)/,"",Jour_Entree)  			# Suppression des espaces     
					Mois_Entree=$2
					#gsub(/(^ *)|( *$)/,"",Mois_Entree)  			# Suppression des espaces 					
					Annee_Entree=$3
					#gsub(/(^ *)|( *$)/,"",Annee_Entree)  			# Suppression des espaces     
					Annee_Sortie_Prevue=$4
					#gsub(/(^ *)|( *$)/,"",Annee_Sortie_Prevue)  	# Suppression des espaces   
					Duree_Bail=$5
					#gsub(/(^ *)|( *$)/,"",Duree_Bail)  				# Suppression des espaces 		 		 
				}   
				else
				{				
					if ($1 == "#Loyer")   
					{   						      
						getline  	 
						getline  	 
						
						#$LOYER_HORS_CHARGES | $LOYER_CHARGES_COMPRISES | $CHARGES 
						Loyer_Hors_cc=$1
						#gsub(/(^ *)|( *$)/,"",Loyer_Hors_cc)  	# Suppression des espaces     
						Loyer_cc=$2
						#gsub(/(^ *)|( *$)/,"",Loyer_cc)  		# Suppression des espaces 					
						Charges=$3
						#gsub(/(^ *)|( *$)/,"",Charges)  		# Suppression des espaces    	
					}
					else
					{		
					
						if ($1 == "#Revision_Loyer")   
						{      
							getline  	 
							getline  	 
							
							#$INDICE_INSEE | $TRIMESTRE_INSEE
							Indice_Irl=$1
							#gsub(/(^ *)|( *$)/,"",Indice_Irl)  		# Suppression des espaces     
							Trimestre_Irl=$2 
						}
						else
						{
							if ($1 == "#Donnees_Bailleur") 
							{ 
								getline  	 
								getline  	 
								
								#$IDENTITE_BAILLEUR | $ADRESSE_BAILLEUR
								Identite_Bailleur=$1     
								Adresse_Bailleur=$2 	 
							}
							else
							{							
								if ($1 == "#Donnees_Quittance") 
								{ 
									getline  	 
									getline  	 
									
									#$MOIS_QUITTANCE | $ANNEE_QUITTANCE		
									mois_quittance=$1
									gsub(/(^ *)|( *$)/,"",mois_quittance)	# Suppression des espaces     
									annee_quittance=$2
									gsub(/(^ *)|( *$)/,"",annee_quittance)	# Suppression des espaces 
									
									#Creation du fichier Source			 
									FichierSortie_Quittance = "Quittance_"Nom_Locataire"-"mois_quittance"-"annee_quittance".tex" 
									#Quittance		 
									Quittance = "Quittance.tex"
												
									system ("sed -e 's/$MOIS_QUITTANCE/"mois_quittance"/g' -e 's/$ANNEE_QUITTANCE/"annee_quittance"/g' -e 's/$IDENTITE_BAILLEUR/"Identite_Bailleur"/g' -e 's/$ADRESSE_BAILLEUR/"Adresse_Bailleur"/g' -e 's/$PRENOM_LOCATAIRE/"Prenom_Locataire"/g' -e 's/$NOM_LOCATAIRE/"Nom_Locataire"/g' -e 's/$DATE_ANNIVERSAIRE/"Date_Anniversaire"/g' -e 's/$LIEU_NAISSANCE/"Lieu_Naissance"/g' -e 's/$NATIONALITE/"Nationalite"/g' -e 's/$ADRESSE_BIEN/"Adresse_Bien"/g' -e 's/$DESIGNATION/"Designation"/g' -e 's/$JOUR_ENTREE/"Jour_Entree"/g' -e 's/$MOIS_ENTREE/"Mois_Entree"/g' -e 's/$ANNEE_ENTREE/"Annee_Entree"/g' -e 's/$ANNEE_SORTIE_PREVUE/"Annee_Sortie_Prevue"/g' -e 's/$DUREE_BAIL/"Duree_Bail"/g'	-e 's/$CHARGES/"Charges"/g' -e 's/$LOYER_CHARGES_COMPRISES/"Loyer_cc"/g' -e 's/$LOYER_HORS_CHARGES/"Loyer_Hors_cc"/g' -e 's/$INDICE_INSEE/"Indice_Irl"/g' -e 's/$TRIMESTRE_INSEE/"Trimestre_Irl"/g' Quittance.tex >" DossierSortie"/"FichierSortie_Quittance)
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
	 
}
