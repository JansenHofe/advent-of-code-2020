
configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day5_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 5', day5_exe, args: meson.current_build_dir() + '/input.txt')