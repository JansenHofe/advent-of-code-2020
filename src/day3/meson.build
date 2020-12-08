
day3_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day3_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 3 executes successfully', day3_exe, args: day3_input, suite: 'basic')

if valgrind.found()
    test('Day 3 memory leak check', valgrind, args: [ valgrind_args, day3_exe, day3_input], suite: 'memcheck')
endif