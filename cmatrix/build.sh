#! /bin/env bash

curr_dir=$(pwd)

if [[ "$#" -eq "1" ]]; then
  if [[ "clean" = "$1" ]]; then
    if [[ -d "$curr_dir/output" ]]; then
      rm -r "$curr_dir/output"
    fi

    if [[ -d "$curr_dir/src/.cache" ]]; then
      rm -r "$curr_dir/src/.cache"
    fi
  else
    echo "wrong command!"
    echo "you may want: ./build.sh clean"
  fi
else
  if [[ -d "$curr_dir/output" ]]; then
    pushd "$curr_dir/output" &>/dev/null
    meson compile
    popd &>/dev/null
  else
    # do default config
    source "$curr_dir/configure.sh" debug
    # redo build
    source "$curr_dir/build.sh"
  fi
fi
