#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Point_set_3/Point_set_processing_3.h>

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
              << " -i  --input      Input file" << std::endl
              << " -o  --output     Output file in PLY format (default = standard output)" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 12)" << std::endl
              << " -f  --fitting    Degree of fitting (default = 2)" << std::endl;
    return EXIT_SUCCESS;
  }
  
  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 12);
  unsigned int fitting = args.get_uint ('f', "fitting", 2);

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  CGAL::jet_estimate_normals<Concurrency_tag> (points, nb_neighbors, fitting);

  CGALApps::write_point_set (args, points);
  
  return EXIT_SUCCESS;
}

