#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libconfig::libconfig++" for configuration "Debug"
set_property(TARGET libconfig::libconfig++ APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libconfig::libconfig++ PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libconfig++.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libconfig++.dll"
  )

list(APPEND _cmake_import_check_targets libconfig::libconfig++ )
list(APPEND _cmake_import_check_files_for_libconfig::libconfig++ "${_IMPORT_PREFIX}/lib/libconfig++.lib" "${_IMPORT_PREFIX}/bin/libconfig++.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
