cmake_minimum_required (VERSION 3.8)

project (cdhdemulator CXX)
my_find_src_files (src_files cdhdemulator)
add_executable (cdhdemulator ${src_files})
my_add_compile_options (cdhdemulator)
my_add_pch (cdhdemulator)

my_inc_import_RisLib(cdhdemulator)
my_lib_import_RisLib(cdhdemulator)

my_post_build_bin(cdhdemulator)
