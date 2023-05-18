#!/bin/env bash

curr_dir=$(pwd)

function help_prompt() {
  if [[ ! -z "$1" ]]; then
    echo "$1"
  fi
  echo "usage: ./configure.sh [type]"
  echo "  type:"
  echo "    release: [rR]ealse"
  echo "    debug: [dD]ebug"
  if [[ -z "$1" ]]; then
    exit 1
  else
    exit 0
  fi
}

if [[ "$#" -lt "1" ]]; then
  help_prompt "need arguments!"
else
  # get build type
  case "$1" in
  [rR]*)
    build_type="release"
    ;;
  [dD]*)
    build_type="debug"
    ;;
  *)
    echo "wrong build type!"
    exit 1
    ;;
  esac

  # start configuration
  if [[ -d "$curr_dir/output" ]]; then
    # remove previous configuration
    rm -r "$curr_dir/output"
  fi
  meson setup "$curr_dir/output" "$curr_dir/src" \
    --native-file "$curr_dir/src/compiler.conf" \
    --buildtype "$build_type" \
    --werror --wipe
  # for clangd
  if [ -f "$curr_dir/output/compile_commands.json" ]; then
    cp "$curr_dir/output/compile_commands.json" "$curr_dir/src/"
  else
    echo "generate compile_commands.json failed!"
  fi
fi
