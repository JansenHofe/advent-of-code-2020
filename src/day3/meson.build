
configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day3_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 3', day3_exe, args: meson.current_build_dir() + '/input.txt')