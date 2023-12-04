#!/bin/sh 

#generate specific Latex files 
for fichier in `ls ../02_Canevas/*_quittance*`
do
        echo "Fichier trouvé : $fichier"
		awk -f Releve_cpt_eau.awk $fichier
done

cd ../03_Generation_Source/ 

#Convert into pdf
texi2pdf *.tex

#delete temporary files 
rm -rf *.tex
rm -rf *.log
rm -rf *.aux