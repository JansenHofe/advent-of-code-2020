
day13_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day13_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 13 executes successfully', day13_exe, args: day13_input, suite: 'basic')

if valgrind.found()
    test('Day 13 memory leak check', valgrind, args: [ valgrind_args, day13_exe, day13_input], suite: 'memcheck')
endif