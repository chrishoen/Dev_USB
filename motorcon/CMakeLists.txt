cmake_minimum_required (VERSION 3.8)

project (motorcon CXX)
my_find_src_files (src_files motorcon)
add_executable (motorcon ${src_files})
my_add_compile_options (motorcon)
my_add_pch (motorcon)

my_inc_import_RisLib(motorcon)
my_inc_import_libusb(testlibusb)

my_lib_import_RisLib(motorcon)
my_lib_import_libusb(testlibusb)

my_post_build_bin(motorcon)
