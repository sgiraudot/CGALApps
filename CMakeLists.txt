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

set(needed_cxx_features cxx_rvalue_references cxx_variadic_templates)
find_package(LASLIB)
if (LASLIB_FOUND)
  include(${LASLIB_USE_FILE})
  include_directories(${LASLIB_INCLUDE_DIR})
  include_directories(${LASZIP_INCLUDE_DIR})
else()
  message(STATUS "NOTICE : LASlib not found, LAS IO won't be available.")
endif()


include( CGAL_CreateSingleSourceCGALProgram )

create_single_source_cgal_program( "src/cgal_advancing_front_surface_reconstruction.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_cluster_point_set.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_compute_average_spacing.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_convert.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_grid_simplify_point_set.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_jet_estimate_normals.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_jet_smooth_point_set.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_mst_orient_normals.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_poisson_surface_reconstruction.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_random_simplify_point_set.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_remove_outliers.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_scale_space_surface_reconstruction.cpp" CXX_FEATURES ${needed_cxx_features} )
create_single_source_cgal_program( "src/cgal_shape_detection.cpp" CXX_FEATURES ${needed_cxx_features} )

if (LASLIB_FOUND)
  target_link_libraries(cgal_advancing_front_surface_reconstruction PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_compute_average_spacing PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_convert PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_cluster_point_set PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_grid_simplify_point_set PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_jet_estimate_normals PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_jet_smooth_point_set PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_mst_orient_normals PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_poisson_surface_reconstruction PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_random_simplify_point_set PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_remove_outliers PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_scale_space_surface_reconstruction PRIVATE ${LASLIB_LIBRARIES})
  target_link_libraries(cgal_shape_detection PRIVATE ${LASLIB_LIBRARIES})
endif()

install(TARGETS
  cgal_advancing_front_surface_reconstruction
  cgal_compute_average_spacing
  cgal_convert
  cgal_cluster_point_set
  cgal_grid_simplify_point_set
  cgal_jet_estimate_normals
  cgal_jet_smooth_point_set
  cgal_mst_orient_normals
  cgal_poisson_surface_reconstruction
  cgal_random_simplify_point_set
  cgal_remove_outliers
  cgal_scale_space_surface_reconstruction
  cgal_shape_detection
  RUNTIME DESTINATION bin)
