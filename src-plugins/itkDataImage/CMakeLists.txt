################################################################################
#
# medInria
#
# Copyright (c) INRIA 2013 - 2014. All rights reserved.
# See LICENSE.txt for details.
# 
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

project(itkDataImagePlugin)

## #############################################################################
## Setup version numbering
## #############################################################################

set(${PROJECT_NAME}_VERSION ${MEDINRIA_VERSION})

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UP)
add_definitions(-D${PROJECT_NAME_UP}_VERSION="${${PROJECT_NAME}_VERSION}")


## #############################################################################
## Resolve dependencies
## #############################################################################

find_package(dtk REQUIRED)
include(${dtk_USE_FILE})
include(dtkPlugin)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if (ITK_USE_SYSTEM_GDCM)
  add_definitions(-DITK_USE_SYSTEM_GDCM)
endif()

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


## #############################################################################
## List Sources
## #############################################################################

list_source_files(${PROJECT_NAME}
  . # current dir
  datas
  interactors
  navigators
  readers
  writers
  )



## #############################################################################
## include directorie.
## #############################################################################

list_header_directories_to_include(${PROJECT_NAME}
  ${${PROJECT_NAME}_HEADERS}
  )

include_directories(${${PROJECT_NAME}_INCLUDE_DIRS}
  ${MEDINRIA_INCLUDE_DIRS}
  )

## #############################################################################
## add library
## #############################################################################

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_CFILES})

## #############################################################################
## Link
## #############################################################################

target_link_libraries(${PROJECT_NAME}
  ${DCMTK_LIBRARIES}
  ${QT_LIBRARIES}
  ${DTK_LIBRARIES}
  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}
  medCore
  medImageIO
  medLog
  medVtkInria
  QVTK
  )

# needed because of link  with medImageIO
if(APPLE)
  FixDCMTKMacLink(${PROJECT_NAME} ${DCMTK_LIBRARIES})
endif(APPLE)


## #############################################################################
## Install rules
## #############################################################################

set_plugin_install_rules(${PROJECT_NAME})


## #############################################################################
## Build tests
## #############################################################################

if(MEDINRIA_BUILD_TESTS)
  add_subdirectory(tests)
endif()

