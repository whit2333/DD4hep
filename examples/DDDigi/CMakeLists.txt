#==========================================================================
#  AIDA Detector description implementation 
#--------------------------------------------------------------------------
# Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
# All rights reserved.
#
# For the licensing terms see $DD4hepINSTALL/LICENSE.
# For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
#
#==========================================================================
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
include ( ${DD4hep_DIR}/cmake/DD4hep.cmake )

#-----------------------------------------------------------------------------------
dd4hep_configure_output ()
dd4hep_package ( DDDigi MAJOR 0 MINOR 0 PATCH 1
  USES  [DD4hep REQUIRED COMPONENTS DDCore DDDigi]
  )
set(DDDigiexamples_INSTALL  ${CMAKE_INSTALL_PREFIX}/examples/DDDigi)
#
dd4hep_set_compiler_flags()
#
dd4hep_add_plugin( DDDigiExampleLib
  SOURCES   src/*.cpp
  USES      [ROOT   REQUIRED COMPONENTS Geom GenVector RIO]
            BOOST
  )
#
dd4hep_install_dir(scripts DESTINATION ${DDDigiexamples_INSTALL})
#
dd4hep_configure_scripts (DDDigi DEFAULT_SETUP WITH_TESTS)
#
# Test HepMC input reader
dd4hep_add_test_reg( DDDigi_framework
  COMMAND    "${CMAKE_INSTALL_PREFIX}/bin/run_test_DDDigi.sh"
  EXEC_ARGS  python ${DDDigiexamples_INSTALL}/scripts/TestFramework.py
  REQUIRES   DDDigi
  REGEX_PASS "\\+\\+\\+ 10 Events out of 10 processed."
  REGEX_FAIL "Error;ERROR;Exception"
  )
#
