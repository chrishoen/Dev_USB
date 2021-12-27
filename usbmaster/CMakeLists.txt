cmake_minimum_required (VERSION 3.8)

project (usbmaster CXX)
my_find_src_files (src_files usbmaster)
add_executable (usbmaster ${src_files})
my_add_compile_options (usbmaster)
my_add_pch (usbmaster)

my_inc_import_RisLib(usbmaster)
my_inc_import_CommonLib(usbmaster)

my_lib_import_CommonLib(usbmaster)
my_lib_import_RisLib(usbmaster)

if(MSVC)
   install(TARGETS usbmaster DESTINATION "C:/aaa_prime/Example")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS usbmaster DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(usbmaster)
endif()

