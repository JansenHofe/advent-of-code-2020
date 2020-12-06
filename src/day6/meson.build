
configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day6_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 6', day6_exe, args: meson.current_build_dir() + '/input.txt')