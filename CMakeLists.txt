project( CGALApps )
cmake_minimum_required(VERSION 2.8.11)

include_directories(include)

find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  
endif()

include( ${CGAL_USE_FILE} )

find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()  
endif()

include( CGAL_CreateSingleSourceCGALProgram )

create_single_source_cgal_program( "src/cgal_compute_average_spacing.cpp" )
create_single_source_cgal_program( "src/cgal_grid_simplify_point_set.cpp" )


