#!/bin/env bash

curr_dir=$(pwd)
meson_command="compile"
default_pack_dir="pack"
export default_pack_name='cmatrix.tar.gz'

function delete_elems() {
  for elem in "$@"; do
    if [[ -d "$elem" ]]; then
      rm -r "$elem"
    elif [[ -e "$elem" ]]; then
      rm "$elem"
    fi
  done
}

if [[ "$#" -eq "1" ]]; then
  if [[ "$1" = "clean" ]]; then
    delete_elems "$curr_dir/output" \
      "$curr_dir/src/.cache" \
      "$curr_dir/$default_pack_dir" \
      "$default_pack_name"
    exit 0
  elif [[ "$1" = "install" ]]; then
    meson_command="install"
  else
    echo "wrong command!"
    echo "you may want './build.sh clean' or './build.sh install'"
    exit 1
  fi
fi

if [[ -d "$curr_dir/output" ]]; then
  meson configure --prefix "$curr_dir/$default_pack_dir" "$curr_dir/output"
  pushd "$curr_dir/output" &>/dev/null
  meson "$meson_command"
  popd &>/dev/null
  # do check
  if
    type valgrind &>/dev/null
    [[ "$?" == '0' ]]
  then
    touch "$curr_dir/output/check.log"
    valgrind --leak-check=full "$curr_dir/output/app" &>"$curr_dir/output/check.log"
  fi
  # do pack
  if [[ -d "$curr_dir/$default_pack_dir" ]]; then
    tar -czvf "$default_pack_name" -C "$default_pack_dir" '.'
  fi
else
  # do default config
  source "$curr_dir/configure.sh" release
  # redo build
  source "$curr_dir/build.sh" install
fi
