
configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day2_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 2', day2_exe, args: meson.current_build_dir() + '/input.txt')