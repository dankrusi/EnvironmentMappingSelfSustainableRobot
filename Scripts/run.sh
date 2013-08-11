#!/bin/sh

# Run the init script to load settings and environment info
SCRIPTS=`dirname $(readlink -f $0)`
. $SCRIPTS/init.sh

# Show splash
echo ""
echo `basename $(readlink -f $0)`
echo "======================================"

# If component not specified, run all
if [ $# -ne 1 ]; then set $1 "all"; fi

run() {
	echo "Running $1..."
	if [ $VERBOSE = true ]; then
		echo "$EMSS/bin/emss$1"
		$EMSS/bin/emss$1
	else
		$EMSS/bin/emss$1
		#$EMSS/bin/emss$1 >> /dev/null 2>> /dev/null # don't suppress this output
	fi
}

# Run all the requested components
if [ $1 = "Interface" -o $1 = "all" ]; then run "Interface"; fi

