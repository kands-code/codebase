ksl_src = [
  'ksl_input.c',
  'ksl_parser.c',
  'ksl_process.c',
  'utils.c',
]

ksllib = static_library('ksl',
  ksl_src,
  include_directories: header_dir,
  dependencies: cc_deps,
  install: true,
)