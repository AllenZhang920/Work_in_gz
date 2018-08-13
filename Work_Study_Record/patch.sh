#!/bin/bash

ls -l $1  > patch_list
PATCH_LIST="patch_list"
while read LINE
do
  if [ "x$LINE" = "x" ] ; then
    echo "LINE is empty."
    continue;
  fi
  FIRST_CHAR=${LINE:0:1}
  if [ $FIRST_CHAR = "#" ] ; then
    echo "it's a comment"
  else
    TARGET=${LINE##*\ }
	echo "TAGET:$TARGET"
	patch  -p1 < $1/$TARGET 2>&1 | tee patch.log
  fi
done < $PATCH_LIST
