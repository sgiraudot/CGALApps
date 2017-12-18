#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/IO/output_surface_facets_to_polyhedron.h>
#include <CGAL/poisson_surface_reconstruction.h>
#include <CGAL/property_map.h>
#include <CGAL/Real_timer.h>

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

  if (!(CGAL::poisson_surface_reconstruction_delaunay
        (points.begin(), points.end(),
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

