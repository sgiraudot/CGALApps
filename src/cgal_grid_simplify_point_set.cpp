#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/Real_timer.h>

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
              << " -v  --verbose  Display info to stderr" << std::endl
              << " -i  --input    Input file" << std::endl
              << " -o  --output   Output file in PLY format (default = standard output)" << std::endl
              << " -e  --epsilon  Length of a grid cell (default = 0.1)" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  double epsilon = args.get_double ('e', "epsilon", 0.1);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Grid Simplify Point Set" << std::endl
              << " * epsilon = " << epsilon << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }
  
  CGAL::grid_simplify_point_set (points, epsilon);

  if (verbose)
    std::cerr << 100. * points.garbage_size() / (points.size() + points.garbage_size())
              << "% / " << points.garbage_size() << " point(s) removed ("
              << points.size() << " point(s) remaining)." << std::endl;

  CGALApps::write_point_set (args, points);

  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }

  return EXIT_SUCCESS;
}

