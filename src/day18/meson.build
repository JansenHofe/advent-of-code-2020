
day18_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day18_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 18 executes successfully', day18_exe, args: day18_input, suite: 'basic')

if valgrind.found()
    test('Day 18 memory leak check', valgrind, args: [ valgrind_args, day18_exe, day18_input], suite: 'memcheck')
endif