#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/remove_outliers.h>
#include <CGAL/Real_timer.h>

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
              << " -v  --verbose    Display info to stderr" << std::endl
              << " -i  --input      Input file" << std::endl
              << " -o  --output     Output file in PLY format (default = standard output)" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 6)" << std::endl
              << " -p  --percent    Percentage threshold (default = 100)" << std::endl
              << " -d  --distance   Distance threshold (default = 0.1)" << std::endl;
      
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 6);
  double percent = args.get_double ('p', "percent", 100);
  double distance = args.get_double ('d', "distance", 0.1);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Remove Outliers" << std::endl
              << " * neighbors = " << nb_neighbors << std::endl
              << " * percent = " << percent << std::endl
              << " * distance = " << distance << std::endl;
    t.start();
  }

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  points.remove_from
    (CGAL::remove_outliers
     (points, nb_neighbors,
      CGAL::parameters::point_map (points.point_map())
      .threshold_percent(percent)
      .threshold_distance(distance)));

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

