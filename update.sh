#!/bin/sh

PROJECT_DIR=$(pwd)

# sanity check: executed in the root folder?
if [ ! -f include/jsobjects.hpp ]; then
	echo "jsobjects/update.sh must be executed in the root dir of the repository."
	exit -1
fi

##########################
# command line options
#

EXTERNALS=/tmp/jsobjects
ENABLE_TESTING=no
ENABLE_CPP=no
VERBOSE=0

function readopts {
  while ((OPTIND<=$#)); do
    if getopts ":d:t:c:hv" opt; then
      case $opt in
      	d) EXTERNALS=$OPTARG;;
        t) ENABLE_TESTING=$OPTARG;;
		c) ENABLE_CPP=$OPTARG;;
		v) VERBOSE=1;;
		h) echo "Usage: update.sh [-d <directory>] [-c yes|no] [-t yes|no] [-v]" $$ exit;;
        *) ;;
      esac
    else
        let OPTIND++
    fi
  done
}

OPTIND=1
readopts "$@"

if [ $VERBOSE == 1 ]; then
	echo "Storing into directory: $EXTERNALS"
	echo "Enable testing?         $ENABLE_TESTING"
	echo "Enable cpp adapter?     $ENABLE_CPP"
fi

#############################
# create output folder
#

if [ -z "$1" ]; then
    EXTERNALS=/tmp/jsobjects_externals
fi

if [ ! -d $EXTERNALS ]; then
	mkdir $EXTERNALS
fi

cd $EXTERNALS

######################
# boost
#

boost_modules="config detail exception smart_ptr algorithm iterator mpl range type_traits preprocessor utility concept function bind format optional"

if [ ! -d boost ]; then
  svn co --depth files http://svn.boost.org/svn/boost/tags/release/Boost_1_50_0/boost
  cd boost
  svn update $boost_modules
  cd ..
fi

######################
# gtest 1.6

if [ $ENABLE_TESTING == yes ]; then
	if [ ! -d gtest ]; then
		svn co "http://googletest.googlecode.com/svn/tags/release-1.6.0" gtest
	fi

	if [ ! -f gtest/Makefile ]; then
		cd gtest
		cmake .
		cd ..
	fi

	if [ ! -f gtest/libgtest.a ]; then
		cd gtest
		make
		cd ..
	fi
fi

######################
# rapidjson

if [ $ENABLE_CPP == yes ]; then
	if [ ! -d rapidjson ]; then
		svn co "http://rapidjson.googlecode.com/svn/trunk" rapidjson
	fi
fi

######################
# build jsobjects
cd $PROJECT_DIR

if [ ! -d build ]; then
	mkdir build
fi
cd build
if [ ! -f CMakeCache.txt ]; then
	cmake -DEXTERNALS_DIR=$EXTERNALS ..
fi
make
