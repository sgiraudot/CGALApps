#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Point_set_3/Point_set_processing_3.h>

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "-----------------------------------" << std::endl
              << "[CGALTools] Compute Average Spacing" << std::endl
              << "-----------------------------------" << std::endl << std::endl
              << "Reads a point set and estimates the average spacing based on a set of nearest"
              << std::endl << "neighbors."
              << std::endl << std::endl
              << " -i  --input      Input file" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 6)" << std::endl;
    return EXIT_SUCCESS;
  }

  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 6);

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
    return EXIT_FAILURE;
  
  FT average_spacing = CGAL::compute_average_spacing<Concurrency_tag>
    (points, nb_neighbors);
  
  std::cout << average_spacing << std::endl;
  
  return EXIT_SUCCESS;
}

