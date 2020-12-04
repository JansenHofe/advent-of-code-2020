
configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day4_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 4', day4_exe, args: meson.current_build_dir() + '/input.txt')