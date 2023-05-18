project('cksl', 'c',
  version: '0.1.0',
  license: 'MIT',
  default_options : ['c_std=c17'],
)

cc = meson.get_compiler('c')
cc_deps = [
  cc.find_library('m', required: true),
  cc.find_library('readline', required: true),
]

# install headers
header_dir = include_directories('include')
install_subdir('include', install_dir: '')

subdir('ksl')

executable('app', 'main.c',
  include_directories: header_dir,
  dependencies: cc_deps,
  link_with: ksllib,
)