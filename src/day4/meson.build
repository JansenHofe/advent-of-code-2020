
day4_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day4_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 4 executes successfully', day4_exe, args: day4_input, suite: 'basic')

if valgrind.found()
    test('Day 4 memory leak check', valgrind, args: [ valgrind_args, day4_exe, day4_input], suite: 'memcheck')
endif