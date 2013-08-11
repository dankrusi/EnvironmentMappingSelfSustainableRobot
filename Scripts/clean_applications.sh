#!/bin/sh

# Run the init script to load settings and environment info
SCRIPTS=`dirname $(readlink -f $0)`
. $SCRIPTS/init.sh

# Show splash
echo ""
echo `basename $(readlink -f $0)`
echo "======================================"

# Clean applications to make sure they
# will be re-linked properly with new lib
cd $EMSS/bin
rm -f emss* #Applications start with emssXXXXX while libs start with libemssXXXXXX...

