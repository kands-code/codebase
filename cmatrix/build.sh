#! /bin/env bash

curr_dir=$(pwd)
default_pack_dir="pack"
default_pack_name='cmatrix.tar.gz'

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
  if [[ "clean" = "$1" ]]; then
    delete_elems "$curr_dir/output" \
      "$curr_dir/src/.cache" \
      "$curr_dir/$default_pack_dir" \
      "$default_pack_name"
  else
    echo "wrong command!"
    echo "you may want: ./build.sh clean"
  fi
else
  if [[ -d "$curr_dir/output" ]]; then
    meson configure --prefix "$curr_dir/$default_pack_dir" "$curr_dir/output"
    pushd "$curr_dir/output" &>/dev/null
    meson install
    popd &>/dev/null
    tar -czvf "$default_pack_name" -C "$default_pack_dir" '.'
  else
    # do default config
    source "$curr_dir/configure.sh" debug
    # redo build
    source "$curr_dir/build.sh"
  fi
fi
