
day6_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day6_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 6 executes successfully', day6_exe, args: day6_input, suite: 'basic')

if valgrind.found()
    test('Day 6 memory leak check', valgrind, args: [ valgrind_args, day6_exe, day6_input], suite: 'memcheck')
endif