#!/bin/sh

BUILD_TYPE=$1
BUILD_ACTION=$2

if [ "$1" == debug ];then
	BUILD_TYPE=Debug
elif [ "$1" == release ];then
	BUILD_TYPE=Release
else
	BUILD_TYPE=Release
fi 

if [ "$2" == rebuild ];then
	BUILD_ACTION=rebuild
elif [ "$2" == build ];then
	BUILD_ACTION=build
else
	BUILD_ACTION=build
fi 

BUILD_DEPEND=linux64

export BUILD_TYPE
export BUILD_ACTION
export BUILD_DEPEND

mkdir -p ../build_$BUILD_DEPEND\_gcc

cd ../build_$BUILD_DEPEND\_gcc

echo "#######################$BUILD_TYPE $BUILD_ACTION $BUILD_DEPEND#################################"

if [ "$BUILD_ACTION" == rebuild ];then
		rm * -rf;
fi

cmake -DCMAKE_TOOLCHAIN_FILE=../script/gcc_$BUILD_DEPEND.cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE .. 
	
if [ "$BUILD_ACTION" == rebuild ];then
		make clean;
fi

make -j4;

cd -;