#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Point_set_3/Point_set_processing_3.h>

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Remove Outliers" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Removes the outliers of a point set based on the local average squared distance."
              << std::endl << std::endl
              << " -i  --input      Input file" << std::endl
              << " -o  --output     Output file in PLY format (default = standard output)" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 6)" << std::endl
              << " -p  --percent    Percentage threshold (default = 1.0)" << std::endl
              << " -d  --distance   Distance threshold (default = 0.1)" << std::endl;
      
    return EXIT_SUCCESS;
  }

  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 6);
  double percent = args.get_double ('p', "percent", 1.0);
  double distance = args.get_double ('d', "distance", 0.1);

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  points.remove_from
    (CGAL::remove_outliers
     (points.begin(), points.end(), points.point_map(), nb_neighbors, percent, distance));

  CGALApps::write_point_set (args, points);
  
  return EXIT_SUCCESS;
}

