
day17_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day17_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 17 executes successfully', day17_exe, args: day17_input, suite: 'basic')

if valgrind.found()
    test('Day 17 memory leak check', valgrind, args: [ valgrind_args, day17_exe, day17_input], suite: 'memcheck')
endif