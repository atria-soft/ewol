#!/bin/bash

usage()
{
	echo -e "$0 [TAG]"
	echo -e "\t\t[TAG]     : Tag to set on the version"
	exit -1
}

#verify all input parameters :
tag=""
if [ "$1" = "" ] ; then 
	usage
else
	tag="$1"
fi

echo -n $tag > tag

git commit -a -m "create a new release"
git tag $tag
