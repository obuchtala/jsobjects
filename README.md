Library "jsobjects"
===================

This library provides an abstraction over JavaScriptCore and V8
to simplify development of extensions for those engines.

Instead of the classical approach with SWIG, here the data originates
from Javascript. Bidirectional data flow is simplified.
A C++ only implementation is provided to allow a pure C++ only testing
environment.

This work is considered to be a basis for implementation of SWIG's
"director" feature for the Javascript module.

Installation
============

Requirements:
 - CMake
 - autoconf
 - automake
 - git
 - svn

Preparation - OSX
.................

Note: on newer OSX versions you have to install automake etc. by yourself.
This easiest done with Macports.

    sudo port install autoconf
    sudo port install automake
    sudo port install pcre

Clone repository
................

	git clone git://github.com/oliver----/jsobjects.git
	cd jsobjects

Prepare externals
.................

    mkdir build-externals
    cd build-externals
    cmake -DENABLE_SWIG=ON -DENABLE_JSC=ON -DENABLE_V8=ON -DEXTERNALS_ONLY=ON ..
    make

Build
.....

    mkdir build
    cd build
    cmake -DENABLE_SWIG=ON -DENABLE_JSC=ON -DENABLE_V8=ON -DCMAKE_BUILD_TYPE=Debug ..
    make

