#----------------------------------------------------------------------------
# Setup the project
#set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.8...3.18)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(How-To-Geant4)
 

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(How-To-Geant4 How-To-Geant4.cc ${sources} ${headers})
target_compile_features(How-To-Geant4 PRIVATE cxx_std_17)

#if gcc is the compiler, you need to use this because of '#include <filesystem>'
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_link_libraries(How-To-Geant4 ${Geant4_LIBRARIES} stdc++fs)
else()
  target_link_libraries(How-To-Geant4 ${Geant4_LIBRARIES})
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory. This is so that we have a backup of those scripts.
#->REALLY NECESSARY?
# set(How-To-Geant4_SCRIPTS
#     PlotRootFile.py
#     vis.mac
#     run.mac
#     radioactivedecay.mac
#     generalparticlesource.mac
#     scoringmesh.mac
#     Lochflansch.stl
#     Lochflansch.obj
#     Austrittsfenster.obj
#     FaradayCupMobile.obj
#   )

# foreach(_script ${How-To-Geant4_SCRIPTS})
#   configure_file(
#     ${PROJECT_SOURCE_DIR}/${_script}
#     ${PROJECT_BINARY_DIR}/${_script}
#     COPYONLY
#     )
# endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS How-To-Geant4 DESTINATION bin)

#if using Visual Studio, copy the executable from the "release"-folder to the build directory 
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_target_properties(How-To-Geant4 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})
    
  target_compile_options(How-To-Geant4 PRIVATE /W4 )
endif()