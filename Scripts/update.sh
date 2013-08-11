#!/bin/sh

# Run the init script to load settings and environment info
SCRIPTS=`dirname $(readlink -f $0)`
. $SCRIPTS/init.sh

# Show splash
echo ""
echo `basename $(readlink -f $0)`
echo "======================================"

# If component not specified, update all
if [ $# -ne 1 ]; then set $1 "all"; fi

update() {
	echo "Updating $1..."
	if [ $VERBOSE = true ]; then
		echo "cd $EMSS/$1"
		cd $EMSS/$1
		echo "svn up"
		svn up
	else
		cd $EMSS/$1
		svn up >> /dev/null 2>> /dev/null
	fi
}

# Update all the requested components
if [ $1 = "Core" -o $1 = "all" ]; then update "Core"; fi
if [ $1 = "Interface" -o $1 = "all" ]; then update "Interface"; fi
if [ $1 = "Resources" -o $1 = "all" ]; then update "Resources"; fi

