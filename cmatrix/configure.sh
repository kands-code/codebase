#! /bin/env bash

rootDir=$(pwd)

if [[ "$#" -gt "2" ]]; then
  echo "Too Many Arguments!"
  echo "Usage: ./configure.sh [arch] [type]"
  echo "    arch: linux or win"
  echo "    type: release or debug"
  exit 1
elif [[ "$#" -lt "2" ]]; then
  echo "Too Less Arguments!"
  echo "Usage: ./configure.sh [arch] [type]"
  echo "    arch: linux or win"
  echo "    type: release or debug"
  exit 1
else
  # get generator
  case "$1" in
  "win")
    gen="MinGW Makefiles"
    ;;
  "linux")
    gen="Unix Makefiles"
    ;;
  *)
    echo "Wrong Platform!"
    exit 1
    ;;
  esac

  # get build type
  case "$2" in
  "release")
    buildType="Release"
    ;;
  "debug")
    buildType="Debug"
    ;;
  *)
    echo "Wrong Build Type!"
    exit 1
    ;;
  esac

  # start configuration
  cmake -B output -S "$rootDir/src/" \
    -DCMAKE_BUILD_TYPE="$buildType" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
    -Wdev -Wdeprecated \
    -G "$gen"
  # for clangd
  if [ -f "$rootDir/output/compile_commands.json" ]; then
    cp "$rootDir/output/compile_commands.json" "$rootDir/src/"
  else
    echo "Generate compile_commands.json Failed!"
  fi
fi
