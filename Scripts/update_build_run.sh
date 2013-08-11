#!/bin/sh

# Run the init script to load settings and environment info
SCRIPTS=`dirname $(readlink -f $0)`
. $SCRIPTS/init.sh

# Just run the scripts sequentially...
$SCRIPTS/update.sh
$SCRIPTS/build.sh
$SCRIPTS/run.sh

