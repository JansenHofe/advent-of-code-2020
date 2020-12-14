
day14_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day14_exe = executable(
    'puzzle',
    'puzzle.c',
    link_args: '-lm',
    dependencies: util_dep,
)

test('Day 14 executes successfully', day14_exe, args: day14_input, suite: 'basic')

if valgrind.found()
    test('Day 14 memory leak check', valgrind, args: [ valgrind_args, day14_exe, day14_input], suite: 'memcheck')
endif