
day12_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day12_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 12 executes successfully', day12_exe, args: day12_input, suite: 'basic')

if valgrind.found()
    test('Day 12 memory leak check', valgrind, args: [ valgrind_args, day12_exe, day12_input], suite: 'memcheck')
endif