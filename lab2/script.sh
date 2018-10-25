#!/bin/bash 

$1 > file1.txt

for i in 'seq 1 100';
do
	$1 > file2.txt
done

result=$(diff file1.txt file2.txt)

if [ $? -eq 0 ]
then
	echo "OK"
else
	echo "Error"
fi
