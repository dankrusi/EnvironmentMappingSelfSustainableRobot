#!/bin/sh

# Load custom settings
. $SCRIPTS/settings.sh

# Determine if settings are to be overrided

# Show splash
if [ !$SPLASHED ]; then
	echo ""
	echo "======================================"
	echo "=           emss Framework           ="
	echo "======================================"
	echo "Location: $EMSS"
	echo "Target: $TARGET"
	echo "Verbose: $VERBOSE"
	SPLASHED=true; export SPLASHED
fi;
