################################################
## Bring in dependent projects
################################################

if(NOT BUILD_IN_DEPS)
  # NOTE: The `FindXMLRPC.cmake` file does not respect cross-compilation
  # environments or windows builds. so, we make the assumption the includes are
  # in a standard directory in your sysroot (for cross compiling) or included via
  # a custom xmlrpc-c library on Windows (See Windows build instructions).
  if (NOT WIN32 AND NOT CMAKE_CROSSCOMPILING)
    find_package(XMLRPC REQUIRED c++ client)
  endif()

  find_package(glog QUIET CONFIG NAMES google-glog glog)
  if (NOT TARGET glog::glog)
    find_library(LIB_glog NAMES glog)
  else()
    set(LIB_glog glog::glog)
  endif (NOT TARGET glog::glog)

  if(NOT WIN32)
    find_library(LIB_xmlrpcxx NAMES xmlrpc++)
    find_library(LIB_xmlrpc NAMES xmlrpc)
    find_library(LIB_xmlrpc_clientxx NAMES xmlrpc_client++)
    find_library(LIB_xmlrpc_client NAMES xmlrpc_client)
  else()
    find_package(
	  xmlrpc-c CONFIG
	  REQUIRED COMPONENTS xmlrpc xmlrpc_client xmlrpc++ xmlrpc_client++
	)
    set(LIB_xmlrpcxx xmlrpc-c::xmlrpc++)
    set(LIB_xmlrpc xmlrpc-c::xmlrpc)
    set(LIB_xmlrpc_clientxx xmlrpc-c::xmlrpc_client++)
    set(LIB_xmlrpc_client xmlrpc-c::xmlrpc_client)
  endif(NOT WIN32)
else (NOT BUILD_IN_DEPS)
  set( LIB_glog glog)
  set(LIB_xmlrpcxx xmlrpc++)
  set(LIB_xmlrpc xmlrpc)
  set(LIB_xmlrpc_clientxx xmlrpc_client++)
  set(LIB_xmlrpc_client xmlrpc_client)
endif(NOT BUILD_IN_DEPS)
################################################
## Target: libifm3d_device.so
################################################
file(GLOB IFM3D_DEVICE_SOURCES *.cpp  ../../include/ifm3d/device)
add_library(ifm3d_device ${IFM3D_DEVICE_SOURCES})
set_property(TARGET ifm3d_device PROPERTY POSITION_INDEPENDENT_CODE 1)

#------------------
# Compiler settings
#------------------
target_include_directories(ifm3d_device
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${IFM3D_DEVICE_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${IFM3D_DEVICE_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${XMLRPC_INCLUDE_DIRS}
    ${BUILTIN_INCLUDES}
    )

if(WIN32)
   if(${BUILD_SHARED_LIBS})
    target_compile_definitions(
      ifm3d_device PRIVATE
      ASIO_STANDALONE
      IFM3D_DEVICE_DLL_BUILD=1
      )
   else()
    target_compile_definitions(
      ifm3d_device PUBLIC
      ASIO_STANDALONE
      IFM3D_DEVICE_STATIC_LIB=1
      )
   endif()
endif(WIN32)
  
#------------------
# Linker settings
#------------------
target_link_libraries(
  ifm3d_device
  PRIVATE
    ${LIB_glog}
    ${LIB_xmlrpc_clientxx}
    ${LIB_xmlrpc_client}
    ${LIB_xmlrpcxx}
    ${LIB_xmlrpc}
    $<BUILD_INTERFACE:asio>
    $<BUILD_INTERFACE:fmt::fmt-header-only>
  )

set_target_properties(
  ifm3d_device PROPERTIES
  VERSION ${IFM3D_VERSION}
  EXPORT_NAME device
  OUTPUT_NAME ifm3d_device
  LINKER_LANGUAGE CXX
  )
  
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" )
  set_target_properties(ifm3d_device PROPERTIES CXX_VISIBILITY_PRESET hidden)
endif()


#------------------
# Installation
#------------------
install(TARGETS ifm3d_device
  EXPORT ifm3d-device-targets
  RUNTIME DESTINATION ${_bin} COMPONENT device
  LIBRARY DESTINATION ${_lib} COMPONENT device
  ARCHIVE DESTINATION ${_lib} COMPONENT device
  PUBLIC_HEADER DESTINATION ${_include} COMPONENT device
  )

if(BUILD_SDK_PKG)
  install(EXPORT ifm3d-device-targets
    NAMESPACE ifm3d::
    DESTINATION ${CONFIG_PACKAGE_LOCATION}
    COMPONENT device
    )
endif()
