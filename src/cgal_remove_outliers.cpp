#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/remove_outliers.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  bool verbose;
  std::string ifilename;
  std::string ofilename;
  unsigned int nb_neighbors;
  double percent;
  double distance;

  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in PLY format", ofilename, "", "stdout");
  args.add_option ("neighbors,n", "Number of nearest neighbors used", nb_neighbors, 6);
  args.add_option ("percent,p", "Percentage threshold", percent, 100);
  args.add_option ("distance,d", "Distance threshold", distance, 0.1);

  if(!args.parse(argc, argv))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Remove Outliers" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Removes the outliers of a point set based on the local average squared distance."
              << std::endl << args.help();
      
    return EXIT_SUCCESS;
  }

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

  CGALApps::read_point_set (ifilename, points);
    
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

  CGALApps::write_point_set (ofilename, points);
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }

  return EXIT_SUCCESS;
}

