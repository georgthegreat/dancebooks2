#.rst:
# FindPostgreSQL
# --------------
#
# Find the PQXX installation.
#
# This module defines
#
# ::
#
#   PQXX_LIBRARIES - the PQXX libraries needed for linking
#   PQXX_INCLUDE_DIRS - the directories of the PQXX headers

#
# Look for an installation.
#

find_path(PQXX_INCLUDE_DIR
    NAMES pqxx/pqxx
    DOC "The pqxx include directory"
)

find_library(PQXX_LIBRARY
    NAMES pqxx
    DOC "The pqxx library"
)
if (NOT PQXX_INCLUDE_DIR OR NOT PQXX_INCLUDE_DIR)
    message(ERROR "Cannot find pqxx")
endif()

# Did we find anything?
include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(PQXX
                                  REQUIRED_VARS PQXX_LIBRARY PQXX_INCLUDE_DIR)

# Now try to get the include and library path.
if(PQXX_FOUND)
  set(PQXX_INCLUDE_DIRS ${PQXX_INCLUDE_DIR})
  set(PQXX_LIBRARIES ${PQXX_LIBRARY})
endif()

mark_as_advanced(PQXX_INCLUDE_DIR PQXX_LIBRARY)
