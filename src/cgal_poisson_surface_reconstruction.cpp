#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/IO/output_surface_facets_to_polyhedron.h>
#include <CGAL/Poisson_reconstruction_function.h>
#include <CGAL/property_map.h>
#include <CGAL/Real_timer.h>

template <typename PointInputIterator,
          typename PointMap,
          typename NormalMap,
          typename PolygonMesh,
          typename Tag = CGAL::Manifold_with_boundary_tag>
bool
poisson_surface_reconstruction (PointInputIterator begin,
                                PointInputIterator end,
                                PointMap point_map,
                                NormalMap normal_map,
                                PolygonMesh& output_mesh,
                                double spacing,
                                double sm_angle = 20.0,
                                double sm_radius = 30.0,
                                double sm_distance = 0.375,
                                Tag tag = Tag())
{
  typedef typename boost::property_traits<PointMap>::value_type Point;
  typedef typename CGAL::Kernel_traits<Point>::Kernel Kernel;
  typedef typename Kernel::Sphere_3 Sphere;
    
  typedef CGAL::Poisson_reconstruction_function<Kernel> Poisson_reconstruction_function;
  typedef CGAL::Surface_mesh_default_triangulation_3 STr;
  typedef CGAL::Surface_mesh_complex_2_in_triangulation_3<STr> C2t3;
  typedef CGAL::Implicit_surface_3<Kernel, Poisson_reconstruction_function> Surface_3;
    
  Poisson_reconstruction_function function(begin, end, point_map, normal_map);
  if ( ! function.compute_implicit_function() ) 
    return false;

  Point inner_point = function.get_inner_point();
  Sphere bsphere = function.bounding_sphere();
  double radius = std::sqrt(bsphere.squared_radius());

  double sm_sphere_radius = 5.0 * radius;
  double sm_dichotomy_error = sm_distance * spacing / 1000.0;
    
  Surface_3 surface(function,
                    Sphere (inner_point, sm_sphere_radius * sm_sphere_radius),
                    sm_dichotomy_error / sm_sphere_radius);

  CGAL::Surface_mesh_default_criteria_3<STr> criteria (sm_angle,
                                                       sm_radius * spacing,
                                                       sm_distance * spacing);

  STr tr;
  C2t3 c2t3(tr);
    
  CGAL::make_surface_mesh(c2t3,
                          surface,
                          criteria,
                          tag);

  if(tr.number_of_vertices() == 0)
    return false;

  CGAL::output_surface_facets_to_polyhedron(c2t3, output_mesh);

  return true;
}

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "-----------------------------------------" << std::endl
              << "[CGALApps] Poisson Surface Reconstruction" << std::endl
              << "-----------------------------------------" << std::endl << std::endl
              << "Reconstructs a surface based on an implicit function whose gradient fit the input normal vectors."
              << std::endl << std::endl
              << " -v  --verbose   Display info to stderr" << std::endl
              << " -i  --input     Input file" << std::endl
              << " -o  --output    Output file in OFF format (default = standard output)" << std::endl
              << " -s  --spacing   Size parameter (default = 0.1)" << std::endl
              << " -a  --angle     Bound of minimum facet angle in degrees (default = 20.0)" << std::endl
              << " -r  --radius    Bound of radius of Delaunay balls w.r.t. to spacing (default = 30.0)" << std::endl
              << " -d  --distance  Bound of center-center distances w.r.t. to spacing (default = 0.375)" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  double spacing = args.get_double ('s', "spacing", 0.1);
  double sm_angle = args.get_double ('a', "angle", 20.0);
  double sm_radius = args.get_double ('r', "radius", 30.0);
  double sm_distance = args.get_double ('d', "distance", 0.375);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Poisson Surface Reconstruction" << std::endl
              << " * spacing = " << spacing << std::endl
              << " * angle = " << sm_angle << std::endl
              << " * radius = " << sm_radius << std::endl
              << " * distance = " << sm_distance << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  if (!(points.has_normal_map()))
  {
    std::cerr << "Error: point set does not have normal vectors." << std::endl;
    return EXIT_FAILURE;
  }

  Polyhedron mesh;

  if (!(poisson_surface_reconstruction (points.begin(), points.end(),
                                        points.point_map(), points.normal_map(),
                                        mesh, spacing, sm_angle, sm_radius, sm_distance)))
  {
    std::cerr << "Error: reconstruction failed." << std::endl;
    return EXIT_FAILURE;
  }

  if (verbose)
    std::cerr << mesh.size_of_facets() << " facet(s) created." << std::endl;
  
  CGALApps::write_surface (args, mesh);
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

