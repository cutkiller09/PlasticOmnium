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
			
			#$PRENOM_LOCATAIRE | $NOM_LOCATAIRE  
            Prenom_Locataire=$1
            gsub(/(^ *)|( *$)/,"",Prenom_Locataire)  	# Suppression des espaces 
            Nom_Locataire=$2
            gsub(/(^ *)|( *$)/,"",Nom_Locataire)  		# Suppression des espaces 
            Date_Anniversaire=$3 

			DossierSortie = "../03_Generation_Source" 			 
        }     
        else
        {        
			if ($1 == "#Donnees_bien")   
			{    
				getline 
				getline 

				#$ADRESSE_BIEN  
				Adresse_Bien=$1
				#gsub(/(^ *)|( *$)/,"",Adresse_Bien)  	# Suppression des espaces      		
				
				#Creation du fichier Source			 
				FichierSortie_Cpt_Eau = "Releve_Cpt_Eau_"Nom_Locataire".tex"  

				system ("sed  -e 's/$PRENOM_LOCATAIRE/"Prenom_Locataire"/g' -e 's/$NOM_LOCATAIRE/"Nom_Locataire"/g' -e 's/$ADRESSE_BIEN/"Adresse_Bien"/g' Releve_Cpt_Eau.tex >" DossierSortie"/"FichierSortie_Cpt_Eau)
			} 
		}	
		getline

		gsub(/(^ *)|( *$)/,"",$1)  # Suppression des espaces   
	}
	 
}
