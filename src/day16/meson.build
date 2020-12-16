
day16_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day16_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 16 executes successfully', day16_exe, args: day16_input, suite: 'basic')

if valgrind.found()
    test('Day 16 memory leak check', valgrind, args: [ valgrind_args, day16_exe, day16_input], suite: 'memcheck')
endif