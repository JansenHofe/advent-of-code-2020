util_lib_sources = [
    'readFile.c'
]

util_include_dir = include_directories('.')

util_lib = static_library(
    'util',
    util_lib_sources
)

util_dep = declare_dependency(
    link_with: util_lib,
    include_directories: util_include_dir
)

