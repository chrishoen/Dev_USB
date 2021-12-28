cmake_minimum_required (VERSION 3.8)

project (serialmaster CXX)
my_find_src_files (src_files serialmaster)
add_executable (serialmaster ${src_files})
my_add_compile_options (serialmaster)
my_add_pch (serialmaster)

my_inc_import_RisLib(serialmaster)
my_inc_import_CommonLib(serialmaster)

my_lib_import_CommonLib(serialmaster)
my_lib_import_RisLib(serialmaster)

if(MSVC)
   install(TARGETS serialmaster DESTINATION "C:/aaa_prime/Example")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS serialmaster DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(serialmaster)
endif()

