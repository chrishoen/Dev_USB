cmake_minimum_required (VERSION 3.8)

project (ProtoSerial CXX)

my_find_src_files (src_files ProtoSerial)
add_executable (ProtoSerial ${src_files})
my_add_compile_options (ProtoSerial)
my_add_pch (ProtoSerial)


my_inc_import_RisLib(ProtoSerial)
my_inc_import_CommonLib(ProtoSerial)

my_lib_import_CommonLib(ProtoSerial)
my_lib_import_RisLib(ProtoSerial)

my_post_build_bin(ProtoSerial)
