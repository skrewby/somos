#!/usr/bin/env bash

helpFunction() {
  echo ""
  echo "Usage: $0 <command>"
  echo -e "\tcommand: Tells the script what to do. Options:"
  echo -e "\t\tsomos: Runs the emulator"
  echo -e "\t\tbuild: Builds and compiles the emulator"
  echo -e "\t\ttests: Uses GTest to run all unit tests"
  echo -e "\t\thelp:  Shows this prompt"
}

if [[ $1 == "help" || -z $1 ]]; then
  helpFunction
elif [[ $1 == "somos" ]]; then
  (cd build/src ; ./somos )
elif [[ $1 == "build" ]]; then
  (cmake -S . -B build)
  (cmake --build build)
elif [[ $1 == "tests" ]]; then
  (cd build/tests ; ctest)
fi
