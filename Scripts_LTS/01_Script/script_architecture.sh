#//bin/sh

awk -f Generation_architecture.awk ../02_Canevas/*.c 

#COM Cmake Generation 
ls -R ../03_Project_Skeleton2 | grep : | grep inc | cut -f'1' -d':' | grep -v test > Headers.txt
ls -R ../03_Project_Skeleton2 | grep : | grep src | cut -f'1' -d':' | grep -v test > Sources.txt
     
file1=../03_Project_Skeleton2/02_Components/01_COM/CMakeLists.txt
file2=../03_Project_Skeleton2/02_Components/01_COM/CMakeLists_test.txt

#sed 's/\(.*\)%HEADERS\(.*\)/echo '\''\1'\''$(cat '"$Header_files"')'\''\2'\''/e' "$file2" 

awk 'NR==FNR{rep=(NR>1?rep RS:"") $0; next} {gsub(/%HEADERS/,rep)}1' Headers.txt $file1 > tmp && mv tmp $file1
awk 'NR==FNR{rep=(NR>1?rep RS:"") $0; next} {gsub(/%SOURCES/,rep)}1' Sources.txt $file1 > tmp && mv tmp $file1 
