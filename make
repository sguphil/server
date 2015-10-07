#!/bin/sh
arg=$1
pwd=`pwd`
if [ $arg = "all" ]; then 
	echo "============start make AccSvr ==============="
	cd AccSvr
	make clean
	make
	echo "============start make testclient============"
	cd ../testclient
	make clean
	make
	echo "============start make dbserver=============="
	cd ../DBSvr
	make clean
	make
elif [ $arg = "clean" ]; then
	cd $pwd
	cd AccSvr
	make clean
	cd $pwd
	cd testclient
	make clean
elif [ $arg = "accsvr" ]; then
	cd $pwd 
	cd AccSvr
	make clean
	make
elif [ $arg = "tc" ]; then
	cd $pwd
	cd testclient
	make clean
	make
else
	echo "unknow args:"{$arg}
fi


