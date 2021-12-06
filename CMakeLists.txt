cmake_minimum_required (VERSION 3.8)

set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/MyOverrides.cmake")
include(MyPrecompiledHeader.cmake)
include(MyCodeMacros.cmake)
include(MyPostBuild.cmake)
include(MyGlobalImports.cmake)
include(MyLocalImports.cmake)

project ("Dev_USB")
my_init_global_import_variables()
my_init_post_build_variables()

add_subdirectory ("CommonLib")
add_subdirectory ("ExampleThread1")
add_subdirectory ("ExampleThread2")
add_subdirectory ("ExampleThread3")
add_subdirectory ("ProtoTcpClient")
add_subdirectory ("ProtoTcpServer")
add_subdirectory ("ProtoUdp")
add_subdirectory ("Test")
return()

add_subdirectory ("CommonLib")
add_subdirectory ("ExampleThread1")
return()

