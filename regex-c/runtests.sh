#!/bin/sh

sed '/^[# ].*$/d' < tests | while read A B C; do
   printf '%-36s' "$A $B $C"
   ./retool $A \'$B\' \'$C\'
   if [ $? -eq 0 ]; then
      echo -e "\033[42m pass \033[0m"
   else
      echo -e "\033[41m fail \033[0m"
   fi
done
