
configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day8_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 8', day8_exe, args: meson.current_build_dir() + '/input.txt')