
configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day1_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 1', day1_exe, args: meson.current_build_dir() + '/input.txt')