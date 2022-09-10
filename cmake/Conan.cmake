# Download automatically, you can also just copy the conan.cmake file
if( NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake" )
  message( STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan" )
  file( DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.16.1/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake" )
endif()

list( APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} )
list( APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} )

include( ${CMAKE_BINARY_DIR}/conan.cmake )
macro( run_conan )


  conan_cmake_configure( REQUIRES
                         ${CONAN_EXTRA_REQUIRES}
                         GENERATORS
                         cmake_find_package
                         OPTIONS
                         ${CONAN_EXTRA_OPTIONS} )

  conan_cmake_autodetect(settings)

  conan_cmake_install(PATH_OR_REFERENCE .
                      BUILD missing
                      SETTINGS ${settings})

#  conan_cmake_run(
#    REQUIRES
#    ${CONAN_EXTRA_REQUIRES}
#    catch2/2.11.0
#    docopt.cpp/0.6.2
#    fmt/6.2.0
#    spdlog/1.5.0
#    OPTIONS
#    ${CONAN_EXTRA_OPTIONS}
#    BASIC_SETUP
#    CMAKE_TARGETS # individual targets to link to
#    BUILD
#    missing)
endmacro()
