#!/bin/sh

########################################
# Script for running the timed updates #
# for MoSync documentation             #
#######################################

# This script runs every 6 hours as determined
# by the crontab entry:
# 0 9,12,15,18 * * * /home/benchua/dev/documentation/mosync/docs/timedgen.sh > /dev/null 2>&1

# This entry says that this script will run 
# every 3 hours ( 0900, 1200, 1600 and 1800)

# The same functionality can also be achieved without 'cron'
# and that is using the 'at' daemon.
# Uncomment the last line of this script to perpetuate the
# running of the script using 'at'.
#
# Note: the script must be run at least once before to activate
# the perpetuation using 'at'

# If this script is not working, be sure the check cron permissions
# in 'cron.allow' or 'cron.deny'.

##########################################

# DOCSDIR points to the checkout directory
# of the MoSync trunk on your system
DOCSDIR=/home/benchua/dev/documentation/mosync/

# Move into the checkout directory
cd $DOCSDIR

# Update the entire checkout trunk
svn update

# This script expects a 'docs/' directory to 
# be present under the DOCSDIR tree
cd $DOCSDIR/docs

# Run the genidl.sh script to generate maapi.h in ../tools/idl
./genidl.sh

# Generate panics.html
./genPanic.sh

# .. and also expects gendox.sh to be in it
./gendox.sh

# record the timestamp of this scheduled generation 
# in ./gentime
# ./gentime is used in displaying the latest generated
# date in $DOCSDIR/docs/index.php
# Bash will write it, PHP will read it, Javascript will parse and display it =)
echo `date +%s` > $DOCSDIR/docs/gentime

# If you wish to schedule this script using the 'at' daemon
# instead of 'cron', uncomment the line below this block and 
# run the script once to register it with the 'at' daemon.
# Once again, if it's not running at the specified time, check
# for permissions in 'cron.allow', 'cron.deny', 'at.allow', 'at.deny'

# at now +6hours < $DOCSDIR/docs/timedgen.sh
