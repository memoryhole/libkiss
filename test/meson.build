cmocka = dependency('cmocka')

exe = executable('test_libkiss', 'test_libkiss.c',
  link_with: [libkiss],
  dependencies: cmocka,
  include_directories: incdir)

test('test libkiss', exe)
