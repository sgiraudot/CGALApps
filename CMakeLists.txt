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

find_package( TBB )
if( TBB_FOUND )
  include(${TBB_USE_FILE})
  list(APPEND CGAL_3RD_PARTY_LIBRARIES ${TBB_LIBRARIES})
else()
  message(STATUS "TBB not found: parallel processing won't be used.")
endif()

find_package(Eigen3 3.1.0)
if (EIGEN3_FOUND)
  include( ${EIGEN3_USE_FILE} )
endif()



include( CGAL_CreateSingleSourceCGALProgram )

create_single_source_cgal_program( "src/cgal_advancing_front_surface_reconstruction.cpp" )
create_single_source_cgal_program( "src/cgal_compute_average_spacing.cpp" )
create_single_source_cgal_program( "src/cgal_convert.cpp" )
create_single_source_cgal_program( "src/cgal_grid_simplify_point_set.cpp" )
create_single_source_cgal_program( "src/cgal_jet_estimate_normals.cpp" )
create_single_source_cgal_program( "src/cgal_jet_smooth_point_set.cpp" )
create_single_source_cgal_program( "src/cgal_mst_orient_normals.cpp" )
create_single_source_cgal_program( "src/cgal_poisson_surface_reconstruction.cpp" )
create_single_source_cgal_program( "src/cgal_remove_outliers.cpp" )
create_single_source_cgal_program( "src/cgal_shape_detection.cpp" )


install(TARGETS
  cgal_advancing_front_surface_reconstruction
  cgal_compute_average_spacing
  cgal_convert
  cgal_grid_simplify_point_set
  cgal_jet_estimate_normals
  cgal_jet_smooth_point_set
  cgal_mst_orient_normals
  cgal_poisson_surface_reconstruction
  cgal_remove_outliers
  cgal_shape_detection
  RUNTIME DESTINATION bin)
