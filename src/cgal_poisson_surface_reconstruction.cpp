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
  bool verbose;
  double epsilon;
  std::string ifilename;
  std::string ofilename;
  double spacing;
  double sm_angle;
  double sm_radius;
  double sm_distance;

  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in OFF format", ofilename, "", "stdout");
  args.add_option ("spacing,s", "Size parameter", spacing, 0.1);
  args.add_option ("angle,a", "Bound of minimum facet angle in degrees", sm_angle, 20.0);
  args.add_option ("radius,r", "Bound of radius of Delaunay balls w.r.t. spacing", sm_radius, 30.0);
  args.add_option ("distance,d", "Bound of center-center distances w.r.t. spacing", sm_distance, 0.375);

  if(!args.parse(argc, argv))
  {
    std::cout << "-----------------------------------------" << std::endl
              << "[CGALApps] Poisson Surface Reconstruction" << std::endl
              << "-----------------------------------------" << std::endl << std::endl
              << "Reconstructs a surface based on an implicit function whose gradient fit the input normal vectors."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }

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

  CGALApps::read_point_set (ifilename, points);
    
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
  
  CGALApps::write_surface (ofilename, mesh);
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

