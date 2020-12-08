
day2_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day2_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 2 executes successfully', day2_exe, args: day2_input, suite: 'basic')

if valgrind.found()
    test('Day 2 memory leak check', valgrind, args: [ valgrind_args, day2_exe, day2_input], suite: 'memcheck')
endif