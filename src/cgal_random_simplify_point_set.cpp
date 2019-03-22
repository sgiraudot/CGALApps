#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/random_simplify_point_set.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  bool verbose;
  std::string ifilename;
  std::string ofilename;
  double percent;
  
  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in PLY format", ofilename, "", "stdout");
  args.add_option ("percent,p", "Removed percentage", percent, 50);

  if(!args.parse(argc, argv))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Random Simplify Point Set" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Simplifies a point set randomly."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Random Simplify Point Set" << std::endl
              << " * percent = " << percent << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (ifilename, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }
  
  points.remove (CGAL::random_simplify_point_set (points, percent), points.end());

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

