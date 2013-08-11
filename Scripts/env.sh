#!/bin/sh

# Run the init script to load settings and environment info
SCRIPTS=`dirname $(readlink -f $0)`
. $SCRIPTS/init.sh

# Show splash
echo ""
echo `basename $(readlink -f $0)`
echo "======================================"

# Show qt version
qmake --version | grep "Using Qt"

# Show core lib version

