# setup project
cmake_minimum_required(VERSION 3.16...3.27)
project(RICH_SIM)

# include Geant4 settings
find_package(Geant4 REQUIRED ui_all vis_all)

# find sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# add executable
add_executable(rich rich.cc ${sources} ${headers})
target_include_directories(rich PRIVATE include)
target_link_libraries(rich PRIVATE ${Geant4_LIBRARIES})

# copy scripts into build execution directory 
# (copied from example B1 provided by Geant4)
set(EXAMPLEB1_SCRIPTS
  init_vis.mac
  run.mac
  vis.mac
  )

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()