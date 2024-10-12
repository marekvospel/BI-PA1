#!/usr/bin/env bash

# # Progtest tester
# this command should be started inside the folder, where a.out can be found. I might make this also compile the file, but right now I'm pretty lazy :D
clear

print_red="\033[31m"
print_green="\033[32m"
print_reset="\033[0m"

total_tests=0
correct_tests=0

mkdir -p tester/CZE
for file in ./sample/CZE/*_in.txt; do
  total_tests=$(($total_tests + 1))
  langId="${file:9:-7}"
  cat "$file" | ./a.out > "tester/${langId}_out.txt"
  if ! $(diff "tester/${langId}_out.txt" "./sample/${langId}_out.txt" 2>/dev/null >/dev/null); then
    printf "${print_red}X Test ${langId} failed!$print_reset\n"
    diff --color -u "./sample/${langId}_out.txt" "tester/${langId}_out.txt" 
  else
    correct_tests=$(($correct_tests + 1))
  fi
done

printf "\n\n%s/%s tests have passed!\n\n" "$correct_tests" "$total_tests"
