#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  bool verbose;
  double epsilon;
  std::string ifilename;
  std::string ofilename;

  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in PLY format", ofilename, "", "stdout");
  args.add_option ("epsilon,e", "Length of a grid cell", epsilon, 0.1);

  if(!args.parse(argc, argv))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Grid Simplify Point Set" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Simplifies a point set based on a regular 3D grid."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }
  
  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Grid Simplify Point Set" << std::endl
              << " * epsilon = " << epsilon << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (ifilename, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  points.remove (CGAL::grid_simplify_point_set (points, epsilon), points.end());

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

