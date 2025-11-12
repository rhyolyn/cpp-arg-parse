arg_parse_inc = include_directories('include')

arg_parse_srcs = [
  'src/argument_parser.cpp',
]

arg_parse_lib = library('arg_parse',
    sources: arg_parse_srcs,
    include_directories: [arg_parse_inc],
    dependencies: [common_dep],
    cpp_args: '-DAPI_DLL_EXPORTS',
    install: true,
    install_dir: join_paths(get_option('prefix'))
)

arg_parse_dep = declare_dependency(
    link_with: arg_parse_lib,
    include_directories: arg_parse_inc,
    dependencies: [common_dep]
)

subdir('test')