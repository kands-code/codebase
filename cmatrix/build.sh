#! /bin/env bash

rootDir=$(pwd)

if [[ "$#" -eq "1" ]]; then
  if [[ "clean" = "$1" ]]; then
    if [[ -d "$rootDir/output" ]]; then
      rm -r "$rootDir/output"
    fi

    if [[ -d "$rootDir/src/.cache" ]]; then
      rm -r "$rootDir/src/.cache"
    fi
  else
    echo "Wrong Command!"
    echo "You May Want: ./build.sh clean"
  fi
else
  if [[ -d "$rootDir/output" ]]; then
    cmake --build "$rootDir/output"
  else
    # do default config
    source "$rootDir/configure.sh" linux debug
    # redo build
    source "$rootDir/build.sh"
  fi
fi
