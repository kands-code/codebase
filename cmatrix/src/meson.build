project('cmatrix', 'c',
   default_options : ['c_std=c17'],
)

inc = include_directories('../include')

cc = meson.get_compiler('c')
cc_deps = [
  cc.find_library('m', required : true),
]

subdir('matrix')

executable('app', 'main.c',
  include_directories: inc,
  dependencies: cc_deps,
  link_with: matrixlib,
)