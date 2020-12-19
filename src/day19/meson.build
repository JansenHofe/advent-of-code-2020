
day19_input = configure_file(input: 'input.txt', output: 'input.txt', copy: true)

day19_exe = executable(
    'puzzle',
    'puzzle.c',
    dependencies: util_dep
)

test('Day 19 executes successfully', day19_exe, args: day19_input, suite: 'basic')

if valgrind.found()
    test('Day 19 memory leak check', valgrind, args: [ valgrind_args, day19_exe, day19_input], suite: 'memcheck')
endif