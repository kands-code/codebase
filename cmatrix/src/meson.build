project('cmatrix', 'c',
  version: '0.1.0',
  license: 'GPL-3.0-or-later',
  default_options : ['c_std=c17'],
)

# install headers
header_dir = include_directories('include')
install_subdir('include', install_dir: '')

cc = meson.get_compiler('c')
cc_deps = [
  cc.find_library('m', required : true),
]

subdir('matrix')

executable('app', 'main.c',
  include_directories: header_dir,
  dependencies: cc_deps,
  link_with: matrixlib,
)