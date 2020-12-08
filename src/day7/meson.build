
day7_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day7_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 7 executes successfully', day7_exe, args: day7_input, suite: 'basic')

if valgrind.found()
    test('Day 7 memory leak check', valgrind, args: [ valgrind_args, day7_exe, day7_input], suite: 'memcheck')
endif