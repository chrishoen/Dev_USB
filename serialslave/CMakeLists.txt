cmake_minimum_required (VERSION 3.8)

project (serialslave CXX)
my_find_src_files (src_files serialslave)
add_executable (serialslave ${src_files})
my_add_compile_options (serialslave)
my_add_pch (serialslave)

my_inc_import_RisLib(serialslave)
my_lib_import_RisLib(serialslave)

my_post_build_bin(serialslave)
