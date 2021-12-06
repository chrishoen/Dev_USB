cmake_minimum_required (VERSION 3.8)

project (serialecho CXX)
my_find_src_files (src_files serialecho)
add_executable (serialecho ${src_files})
my_add_compile_options (serialecho)
my_add_pch (serialecho)

my_inc_import_RisLib(serialecho)
my_lib_import_RisLib(serialecho)


add_custom_command(
  TARGET  serialecho
  POST_BUILD
  COMMAND rsync -azv --chmod=ugo=rwx serialecho rsync://debian@devtgt/prime/bin
  WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_BBB_KbdModal/serialecho)
