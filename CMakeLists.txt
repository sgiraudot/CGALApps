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

find_package(Eigen3 3.1.0)
if (EIGEN3_FOUND)
  include( ${EIGEN3_USE_FILE} )
endif()

set(needed_cxx_features cxx_rvalue_references cxx_variadic_templates)
set(linked_libraries)
set(compile_definitions)

find_package(LASLIB)
if (LASLIB_FOUND)
  set(linked_libraries ${linked_libraries} ${LASLIB_LIBRARIES})
  include(${LASLIB_USE_FILE})
  include_directories(${LASLIB_INCLUDE_DIR})
  include_directories(${LASZIP_INCLUDE_DIR})
else()
  message(STATUS "NOTICE : LASlib not found, LAS IO won't be available.")
endif()


include( CGAL_CreateSingleSourceCGALProgram )

# Classification examples
set(targets
  cgal_advancing_front_surface_reconstruction
  cgal_cluster_point_set
  cgal_compute_average_spacing
  cgal_convert
  cgal_grid_simplify_point_set
  cgal_jet_estimate_normals
  cgal_jet_smooth_point_set
  cgal_mst_orient_normals
  cgal_poisson_surface_reconstruction
  cgal_random_simplify_point_set
  cgal_remove_outliers
  cgal_scale_space_surface_reconstruction
  cgal_shape_detection)

# Creating targets with correct libraries and flags
foreach(target ${targets})
  create_single_source_cgal_program( "src/${target}.cpp" CXX_FEATURES ${needed_cxx_features} )
  if(TARGET ${target})
    target_link_libraries(${target} PUBLIC ${linked_libraries})
    if(TBB_FOUND)
      CGAL_target_use_TBB(${target})
    endif()
  endif()
endforeach()

install(TARGETS ${targets} RUNTIME DESTINATION bin)
