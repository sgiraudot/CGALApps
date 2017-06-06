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
    std::cout << "------------------------------" << std::endl
              << "[CGALApps] MST Orient Normals" << std::endl
              << "------------------------------" << std::endl << std::endl
              << "Orients the normal vectors of a point set based on a minimum spanning tree."
              << std::endl << std::endl
              << " -v  --verbose    Display info to stderr" << std::endl
              << " -i  --input      Input file" << std::endl
              << " -o  --output     Output file in PLY format (default = standard output)" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 24)" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 24);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] MST Orient Normals" << std::endl
              << " * neighbors = " << nb_neighbors << std::endl;
    t.start();
  }

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  if (!(points.has_normal_map()))
  {
    std::cerr << "Error: point set does not have normal vectors." << std::endl;
    return EXIT_FAILURE;
  }

  typename Point_set::iterator it = CGAL::mst_orient_normals (points, nb_neighbors);
  if (verbose)
  {
    std::ptrdiff_t nb_unoriented = std::distance (it, points.end());
    if (nb_unoriented != 0)
      std::cerr << nb_unoriented << " point(s) were not properly oriented." << std::endl;
  }

  CGALApps::write_point_set (args, points);

  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

