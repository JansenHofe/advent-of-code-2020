
configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day7_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 7', day7_exe, args: meson.current_build_dir() + '/input.txt')