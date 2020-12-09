
day9_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day9_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 9 executes successfully', day9_exe, args: day9_input, suite: 'basic')

if valgrind.found()
    test('Day 9 memory leak check', valgrind, args: [ valgrind_args, day9_exe, day9_input], suite: 'memcheck')
endif