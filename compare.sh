#!/bin/sh

ARGS=$5
PATH_DIR=$1
FILE_TRACE=$2
CMD1=$3
CMD2=$4

if [ ! -d $PATH_DIR ]; then
	echo "argument 1 is not a directory"
	exit 1
fi
/bin/rm $FILE_TRACE
for i in `find $PATH_DIR -name "*" -type f`; do
	echo  "\n$i" >> $FILE_TRACE
	$CMD1 $ARGS $i > diff1 #2>&1
	$CMD2 $ARGS $i > diff2 #2>&1
	diff diff1 diff2 >> $FILE_TRACE 2>&1
	#sleep 1
done