
day1_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day1_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 1 executes successfully', day1_exe, args: day1_input, suite: 'basic')

if valgrind.found()
    test('Day 1 memory leak check', valgrind, args: [ valgrind_args, day1_exe, day1_input], suite: 'memcheck')
endif