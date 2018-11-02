#!/bin/sh
#version-1
check_time_come() 
{
	while ((1))
	do
		date_time=`date +%H`
		if [ "$1" == "${date_time}" ];then
			echo "The hour time come"
			break;
		else
			echo "Wait for next hour"
		fi
		sleep 1h
	done
}

check_time_come $1
date_day=`date +%m%d`
path=/home/allen_zhang/project/DHP2-${date_day}
if [ -d ${path} ];then
	num=2
	while ([ -d "${path}-${num}" ])
	do
		let num++ 
		echo "num is ${num}"
	done
	newpath="${path}-${num}"
	echo "${path} exsit,do you want to add a new project with ${newpath}?"
	echo "Build new project?(y/n)"
	path=${newpath}
	read choose
	if [ "x$choose" == "xy" ];then
		echo "Build new project ${newpath}"	
	else
		echo "Give up to build project"
		exit 0;
	fi
else
	echo "${path} not exsit"
fi
echo "continue do jobs"

cd /home/allen_zhang/project/
mkdir -p ${path}
cd ${path}
repo init -u admin@10.118.81.197:manifest --no-repo-verify
repo sync -f
repo start master --all
cd ${path}/aldk
ln -s configs/test/buffalo/WG9115MAC44-J6/default.mk default.mk
make V=99
