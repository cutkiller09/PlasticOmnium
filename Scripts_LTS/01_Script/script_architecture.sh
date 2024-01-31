#//bin/sh

awk -f Generation_architecture.awk ../02_Canevas/*.c 

#COM Cmake Generation 
ls -R ../03_Project_Skeleton/02_Components/ | grep : | grep inc | cut -f'1' -d':' | grep -v est > Headers.txt 
find ../03_Project_Skeleton/02_Components/ -name *.c | grep -v est | grep -v build > Sources.txt



sed -e 's|../03_Project_Skeleton/02_Components|${PROJECT_SOURCE_DIR}|g' Headers.txt > Headers2.txt 
sed -e 's|../03_Project_Skeleton/02_Components|${PROJECT_SOURCE_DIR}|g' Sources.txt > Sources2.txt 
     
file1=../03_Project_Skeleton/02_Components/01_HighLevel/CMakeLists.txt  

#sed 's/\(.*\)%HEADERS\(.*\)/echo '\''\1'\''$(cat '"$Header_files"')'\''\2'\''/e' "$file2" 

awk 'NR==FNR{rep=(NR>1?rep RS:"") $0; next} {gsub(/%HEADERS/,rep)}1' Headers2.txt $file1 > tmp && mv tmp $file1
awk 'NR==FNR{rep=(NR>1?rep RS:"") $0; next} {gsub(/%SOURCES/,rep)}1' Sources2.txt $file1 > tmp && mv tmp $file1

rm *.txt 
