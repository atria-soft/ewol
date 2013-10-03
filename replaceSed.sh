#!/bin/bash


listFiles=" `find . -name "*.cpp"`  "
listFiles+=" `find . -name "*.c"`  "
listFiles+=" `find . -name "*.h"` "
#listFiles+=" `find . -name "*.mk"` "

echo "Replace : \"$1\" ==> \"$2\""

for iii in $listFiles
do
	echo "* File : "$iii
	sed -i "s|$1|$2|" $iii
done

