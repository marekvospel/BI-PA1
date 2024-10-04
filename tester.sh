#!/bin/env bash

# # Progtest tester
# this command should be started inside the folder, where a.out can be found. I might make this also compile the file, but right now I'm pretty lazy :D

print_red="\033[31m"
print_green="\033[32m"
print_reset="\033[0m"

mkdir -p tester/{CZE,ENG}
for file in ./sample/{CZE,ENG}/*_in.txt; do
  langId="${file:9:-7}"
  cat "$file" | ./a.out > "tester/${langId}_out.txt"
  if ! $(diff "tester/${langId}_out.txt" "./sample/${langId}_out.txt" 2>/dev/null >/dev/null); then
    printf "${print_red}X Test ${langId} failed!$print_reset\n"
    diff --color -u "tester/${langId}_out.txt" "./sample/${langId}_out.txt"
  fi
done
