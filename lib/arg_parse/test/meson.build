gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')

test_exe = executable(
  'arg_parse_test',
  'test_arg_parse.cpp',
  dependencies: [gtest_dep],
  link_with: arg_parse_lib,
  include_directories: ['../src']
)

test('basic', test_exe)	