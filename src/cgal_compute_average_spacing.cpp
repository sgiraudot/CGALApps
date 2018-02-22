#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/compute_average_spacing.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Compute Average Spacing" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Reads a point set and estimates the average spacing based on a set of nearest"
              << std::endl << "neighbors."
              << std::endl << std::endl
              << " -v  --verbose    Display info to stderr" << std::endl
              << " -i  --input      Input file" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 6)" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 6);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Compute Average Spacing" << std::endl
              << " * neighbors = " << nb_neighbors << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }
  
  FT average_spacing = CGAL::compute_average_spacing<Concurrency_tag>
    (points, nb_neighbors);
  
  std::cout << average_spacing << std::endl;
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

