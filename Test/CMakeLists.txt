cmake_minimum_required (VERSION 3.8)

project (Test CXX)

my_find_src_files (src_files Test)
add_executable (Test ${src_files})
my_add_compile_options (Test)
my_add_pch (Test)

my_inc_import_RisLib(Test)
my_lib_import_RisLib(Test)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS Test DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(Test)
endif()
