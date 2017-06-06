#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Point_set_3/Point_set_processing_3.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "-------------------------------" << std::endl
              << "[CGALApps] Jet Estimate Normals" << std::endl
              << "-------------------------------" << std::endl << std::endl
              << "Estimates the normal vectors of a point set based on a parametric surface estimation."
              << std::endl << std::endl
              << " -v  --verbose    Display info to stderr" << std::endl
              << " -i  --input      Input file" << std::endl
              << " -o  --output     Output file in PLY format (default = standard output)" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 12)" << std::endl
              << " -f  --fitting    Degree of fitting (default = 2)" << std::endl;
    return EXIT_SUCCESS;
  }
  
  bool verbose = args.get_bool('v', "verbose");
  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 12);
  unsigned int fitting = args.get_uint ('f', "fitting", 2);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Jet Estimate Normals" << std::endl
              << " * neighbors = " << nb_neighbors << std::endl
              << " * fitting = " << fitting << std::endl;
    t.start();
  }

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  CGAL::jet_estimate_normals<Concurrency_tag> (points, nb_neighbors, fitting);

  CGALApps::write_point_set (args, points);
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

