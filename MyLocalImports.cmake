#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_CommonLib _target)

   target_link_libraries(${_target} CommonLib)

endfunction()

function(my_inc_import_CommonLib _target)

   target_include_directories(${_target} PRIVATE $<TARGET_PROPERTY:CommonLib,INTERFACE_INCLUDE_DIRECTORIES>)

endfunction()

