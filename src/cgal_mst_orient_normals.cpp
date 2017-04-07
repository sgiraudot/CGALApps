#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Point_set_3/Point_set_processing_3.h>

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "------------------------------" << std::endl
              << "[CGALApps] MST Orient Normals" << std::endl
              << "------------------------------" << std::endl << std::endl
              << "Orients the normal vectors of a point set based on a minimum spanning tree."
              << std::endl << std::endl
              << " -i  --input      Input file" << std::endl
              << " -o  --output     Output file in PLY format (default = standard output)" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 24)" << std::endl;
    return EXIT_SUCCESS;
  }
  
  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 24);

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  if (!(points.has_normal_map()))
  {
    std::cerr << "Error: point set does not have normal vectors" << std::endl;
    return EXIT_FAILURE;
  }

  CGAL::mst_orient_normals (points, nb_neighbors);

  CGALApps::write_point_set (args, points);
  
  return EXIT_SUCCESS;
}

