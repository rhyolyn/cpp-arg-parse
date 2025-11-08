app_srcs = [
  'arg_parse_tester.cpp',
]

executable(
  'arg_parse_tester',
  app_srcs,
  dependencies: arg_parse_dep,
  include_directories: arg_parse_inc,
  install: true,
  install_dir: join_paths(get_option('prefix'))
)