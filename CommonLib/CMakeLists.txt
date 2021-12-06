cmake_minimum_required (VERSION 3.8)

project (CommonLib CXX)

my_find_src_files (src_files CommonLib)
add_library (CommonLib STATIC ${src_files})
my_add_compile_options (CommonLib)
my_add_pch (CommonLib)

target_include_directories(CommonLib PUBLIC ${PROJECT_SOURCE_DIR})

my_inc_import_RisLib(CommonLib)
