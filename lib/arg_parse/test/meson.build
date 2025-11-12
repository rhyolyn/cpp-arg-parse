gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

test_sources = [
  'test_argument.cpp',
  'test_argument_parser.cpp',
  'tdd.cpp'
]

test_exe = executable(
  'arg_parse_test',
  test_sources,
  dependencies: [
    gtest_dep,
    gtest_main_dep,
    arg_parse_dep,
    common_dep
  ],
  include_directories: [arg_parse_inc, common_inc]
)

test('arg_parse_tests', test_exe)