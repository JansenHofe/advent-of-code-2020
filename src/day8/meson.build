
day8_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day8_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 8 executes successfully', day8_exe, args: day8_input, suite: 'basic')

if valgrind.found()
    test('Day 8 memory leak check', valgrind, args: [ valgrind_args, day8_exe, day8_input], suite: 'memcheck')
endif