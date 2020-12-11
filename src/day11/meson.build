
day11_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day11_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 11 executes successfully', day11_exe, args: day11_input, suite: 'basic')

if valgrind.found()
    test('Day 11 memory leak check', valgrind, args: [ valgrind_args, day11_exe, day11_input], suite: 'memcheck')
endif