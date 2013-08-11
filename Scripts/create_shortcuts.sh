#!/bin/sh

# Run the init script to load settings and environment info
SCRIPTS=`dirname $(readlink -f $0)`
. $SCRIPTS/init.sh

# Show splash
echo ""
echo `basename $(readlink -f $0)`
echo "======================================"

# Delete existing soft links
sudo rm -f /bin/er
sudo rm -f /bin/ebr
sudo rm -f /bin/eubr

# Create soft links
sudo ln -s $SCRIPTS/run.sh /bin/er
echo "Created command line shortcut 'er' to run.sh"
sudo ln -s $SCRIPTS/build_run.sh /bin/ebr
echo "Created command line shortcut 'ebr' to build_run.sh"
sudo ln -s $SCRIPTS/update_build_run.sh /bin/eubr
echo "Created command line shortcut 'eubr' to update_build_run.sh"

