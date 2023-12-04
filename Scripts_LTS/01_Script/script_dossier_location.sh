#!/bin/sh

awk -f Generation_dossier_location.awk ../02_Canevas/*.c
cd ../03_Generation_Source/
texi2pdf *.tex
rm -rf *.log 
rm -rf *.aux*
