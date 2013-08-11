#!/bin/sh

# Load the environment info based on settings.sh
SCRIPTS=`dirname $(readlink -f $0)`
. $SCRIPTS/settings.sh

# Show splash
echo ""
echo `basename $(readlink -f $0)`
echo "======================================"

# If component not specified, build all
if [ $# -ne 1 ]; then set $1 "all"; fi

build() {
	echo "Building $1..."
	if [ $VERBOSE = true ]; then
		echo "cd $EMSS/$1"
		cd $EMSS/$1
		echo "qmake CONFIG+=debug_and_release"
		qmake CONFIG+=debug_and_release
		echo "make $TARGET"
		make $TARGET
	else
		cd $EMSS/$1
		qmake CONFIG+=debug_and_release >> /dev/null 2>> /dev/null
		make $TARGET >> /dev/null 2>> /dev/null
	fi

	# Clean applications if component is Core to make sure they
	# will be re-linked properly with new lib
	if [ $1 = "Core" ]; then
		cd $EMSS/bin
		rm -f emss* #Applications start with emssXXXXX while libs start with libemssXXXXXX...
	fi
}

# Build all the requested components
if [ $1 = "Core" -o $1 = "all" ]; then build "Core"; fi
if [ $1 = "Interface" -o $1 = "all" ]; then build "Interface"; fi

