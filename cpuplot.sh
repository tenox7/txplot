#!/bin/ksh
#
# tty plot cpu usage from last n minutes, assuming
# sar(1) is running on 5 minute interval...
#

[ "$#" -lt "1" ] && { echo "Usage: $0 minutes"; exit 1; }

# Set fixed scale to 100%
export TXPLOT_SCALE=100

while :
do
	txplot `sar | awk '/^[0-9]+:/ { print 100-$5 }' | tail -$(($1/5))`
	sleep 60
done
