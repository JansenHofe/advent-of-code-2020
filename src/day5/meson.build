
day5_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day5_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 5 executes successfully', day5_exe, args: day5_input, suite: 'basic')

if valgrind.found()
    test('Day 5 memory leak check', valgrind, args: [ valgrind_args, day5_exe, day5_input], suite: 'memcheck')
endif