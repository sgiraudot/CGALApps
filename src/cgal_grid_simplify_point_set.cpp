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
              << "[CGALApps] Grid Simplify Point Set" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Simplifies a point set based on a regular 3D grid."
              << std::endl << std::endl
              << " -i  --input    Input file" << std::endl
              << " -o  --output   Output file in PLY format (default = standard output)" << std::endl
              << " -e  --epsilon  Length of a grid cell (default = 0.1)" << std::endl;
    return EXIT_SUCCESS;
  }

  double epsilon = args.get_double ('e', "epsilon", 0.1);

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }
  
  CGAL::grid_simplify_point_set (points, epsilon);

  CGALApps::write_point_set (args, points);
  
  return EXIT_SUCCESS;
}

