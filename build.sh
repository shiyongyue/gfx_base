#!/bin/bash
reset
CC=gcc
TARGET=gfx
TOP="${PWD}"


CPPFLAGS="-g -O2 -DNDEBUG -D_GUN_SOURCE"
#CPPFLAGS="-g -W -Wall -Werror -O2 -DNDEBUG -D_GUN_SOURCE"

########################################################
filenames=`find src -name "*.c"` ### 搜索當前目錄下所有cpp文件
echo "$filenames"
replace=".o"
r=""
objs=""
for i in $filenames; do
	r="${i/.c/$replace} "   ###.c擴展名改爲.o
	r="objs/"${r##*/}				 ###刪除文件名中的路徑
	a=`stat -c %Y $i`
	if [ -f $r ]; then
		b=`stat -c %Y $r`
	else
		b=0
	fi
	if [ $a -le $b ]; then
			echo "no compile need"
		else
			echo "compile $i start"
			$CC -c $i $CPPFLAGS -o $r 
			echo "compile $i done"
	fi
	objs=${objs}$r			 ### 鏈接所有.o文件
done
echo "$objs"
######################################################
##link
#echo "only compile ,no do link"
now=`date "+%Y-%m-%d %T"`
echo "link start $now"
$CC $objs -o $TARGET -lX11 -lXext 
now=`date "+%Y-%m-%d %T"`
echo "link done $now"
#rm $objs
./$TARGET
