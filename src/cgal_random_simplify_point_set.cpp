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
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Random Simplify Point Set" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Simplifies a point set randomly."
              << std::endl << std::endl
              << " -v  --verbose  Display info to stderr" << std::endl
              << " -i  --input    Input file" << std::endl
              << " -o  --output   Output file in PLY format (default = standard output)" << std::endl
              << " -p  --percent  Removed percentage (default = 50)" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  double percent = args.get_double ('p', "percent", 50.);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Random Simplify Point Set" << std::endl
              << " * percent = " << percent << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }
  
  CGAL::random_simplify_point_set (points, percent);

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

