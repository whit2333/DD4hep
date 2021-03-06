#=================================================================================
#
#  AIDA Detector description implementation 
#---------------------------------------------------------------------------------
# Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
# All rights reserved.
#
# For the licensing terms see $DD4hepINSTALL/LICENSE.
# For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
#
#=================================================================================

#---------------------------------------------------------------------------------------------------
if ( DD4hep_DIR )
  set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${DD4hep_DIR}/cmake ) 
endif()
if(CMAKE_INSTALL_PREFIX)
  set ( CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
endif()

#---------------------------------------------------------------------------------------------------
# Main functional include file
if (NOT DD4hepBuild_included)
  include ( DD4hepBuild )
endif()

##---------------------------------------------------------------------------------------------------
##
##  OLDER STUFF: To be kept for backwards compatibility ....
##
#---------------------------------------------------------------------------------------------------
# add_dd4hep_plugin ( libraryName )
#
# generates the rootmap and installs the library
# all other arguments ( SHARED, ${sources} ) are collected in ${ARGN}
#---------------------------------------------------------------------------------------------------

function( add_dd4hep_plugin libraryName )
  ADD_LIBRARY ( ${libraryName}  ${ARGN} )

  if(APPLE)
    dd4hep_generate_rootmap_apple( ${libraryName} )
  else()
    dd4hep_generate_rootmap( ${libraryName} )
  endif()

  install( TARGETS ${libraryName} LIBRARY DESTINATION lib )
endfunction()

#---------------------------------------------------------------------------------------------------
# dd4hep_instantiate_package
# calls all the function/includes/configurations that are needed to be done to create dd4hep plugins
#---------------------------------------------------------------------------------------------------
function ( dd4hep_instantiate_package PackageName )
  MESSAGE (STATUS "Instantiating the DD4hep package ${PackageName}" )

  IF ( NOT ${DD4hep_FOUND} )
    MESSAGE ( FATAL "DD4HEP was not found" )
  ENDIF()

  INCLUDE( DD4hepMacros )

  #---- configure run environment ---------------
  configure_file( ${DD4hep_DIR}/cmake/thisdd4hep_package.sh.in  this${PackageName}.sh @ONLY)

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/this${PackageName}.sh
    DESTINATION bin
    )


endfunction()
#---------------------------------------------------------------------------------------------------
# dd4hep_generate_rootmap(library)
#
# Create the .components file needed by the plug-in system.
#---------------------------------------------------------------------------------------------------
function(dd4hep_generate_rootmap library)
  if(APPLE)
    dd4hep_generate_rootmap_apple( ${library} )
  else()
    dd4hep_generate_rootmap_notapple( ${library} )
  endif()
endfunction()
#---------------------------------------------------------------------------------------------------
function(dd4hep_generate_rootmap_notapple library)
  if ( NOT DD4hep_DIR )
    SET ( DD4hep_DIR ${CMAKE_SOURCE_DIR} )
  endif()
  find_package(ROOT QUIET)
  set(rootmapfile ${CMAKE_SHARED_MODULE_PREFIX}${library}.components)

  set(libname ${CMAKE_SHARED_MODULE_PREFIX}${library}${CMAKE_SHARED_LIBRARY_SUFFIX})

  add_custom_command(OUTPUT ${rootmapfile}
                     DEPENDS ${library}
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -Dlibname=${libname} -Drootmapfile=${rootmapfile}
                             -Dgenmap_install_dir=${LIBRARY_OUTPUT_PATH}
                             -DROOT_VERSION=${ROOT_VERSION}
                             -DDD4hep_DIR=${DD4hep_DIR}
                             -P ${DD4hep_DIR}/cmake/MakeGaudiMap.cmake)
  add_custom_target(Components_${library} ALL DEPENDS ${rootmapfile})
  SET( install_destination "lib" )
  if( CMAKE_INSTALL_LIBDIR )
    SET( install_destination ${CMAKE_INSTALL_LIBDIR} )
  endif()

  install(FILES ${LIBRARY_OUTPUT_PATH}/${rootmapfile}
    DESTINATION ${install_destination}
  )
endfunction()
#
#
#---------------------------------------------------------------------------------------------------
function(dd4hep_generate_rootmap_apple library)
  # for now do the same for apple that is done for the rest
  dd4hep_generate_rootmap_notapple( ${library} )
endfunction()
