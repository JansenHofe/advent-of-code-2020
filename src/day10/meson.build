
day10_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day10_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 10 executes successfully', day10_exe, args: day10_input, suite: 'basic')

if valgrind.found()
    test('Day 10 memory leak check', valgrind, args: [ valgrind_args, day10_exe, day10_input], suite: 'memcheck')
endif