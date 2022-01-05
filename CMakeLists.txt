cmake_minimum_required (VERSION 3.8)

set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/MyOverrides.cmake")
include(MyPrecompiledHeader.cmake)
include(MyCodeMacros.cmake)
include(MyPostBuild.cmake)
include(MyGlobalImports.cmake)
include(MyLocalImports.cmake)
include(MySpecial.cmake)

project ("Dev_USBHost")
my_init_global_import_variables()
my_init_post_build_variables()
my_special1()

add_subdirectory ("CommonLib")
add_subdirectory ("usbmaster")
return()

add_subdirectory ("motorcon")
add_subdirectory ("testlibusb")
return()

add_subdirectory ("cdhdemulator")
return()

add_subdirectory ("ProtoSerial")
return()

add_subdirectory ("serialmaster")
add_subdirectory ("serialslave")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoSerial")
add_subdirectory ("serialecho")
add_subdirectory ("Test")
return()


