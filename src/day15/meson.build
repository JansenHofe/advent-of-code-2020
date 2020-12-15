
day15_exe = executable(
    'puzzle',
    'puzzle.c',
    c_args: '-O3', # increase compiler optimization to reduce runtime, as this puzzle executes rather slow
    dependencies: util_dep,
)

test('Day 15 executes successfully', day15_exe, suite: 'basic')

if valgrind.found()
    test('Day 15 memory leak check', valgrind, args: [ valgrind_args, day15_exe], suite: 'memcheck')
endif