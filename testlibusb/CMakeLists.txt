cmake_minimum_required (VERSION 3.8)

project (testlibusb CXX)
my_find_src_files (src_files testlibusb)
add_executable (testlibusb ${src_files})
my_add_compile_options (testlibusb)

my_lib_import_libusb(testlibusb)

if(MSVC)
   install(TARGETS testlibusb DESTINATION "C:/aaa_prime/Example")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS testlibusb DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(testlibusb)
endif()

