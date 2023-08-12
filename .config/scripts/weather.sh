#!/bin/bash

WEA=$(curl -s wttr.in/85705?format=%t)

ICON="⛅️"
ICON2="🙁 "

LENGTH=${#WEA}
#echo $LENGTH
FIRST_WORD=$(echo $WEA | awk '{print $1}')
#echo $FIRST_WORD
# echo $FIRST_WORD
if [ $LENGTH -gt 6 ];then
    printf "[%s%s]\n" "$ICON2" "Outage"
else
    printf "[%s%s]\n" "$ICON" "$WEA"
fi 



