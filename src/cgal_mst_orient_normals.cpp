#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/mst_orient_normals.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  bool verbose;
  double epsilon;
  std::string ifilename;
  std::string ofilename;
  unsigned int nb_neighbors;

  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in PLY format", ofilename, "", "stdout");
  args.add_option ("neighbors,n", "Number of nearest neighbors used", nb_neighbors, 24);
  
  if(!args.parse(argc, argv))
  {
    std::cout << "------------------------------" << std::endl
              << "[CGALApps] MST Orient Normals" << std::endl
              << "------------------------------" << std::endl << std::endl
              << "Orients the normal vectors of a point set based on a minimum spanning tree."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] MST Orient Normals" << std::endl
              << " * neighbors = " << nb_neighbors << std::endl;
    t.start();
  }

  Point_set points;

  CGALApps::read_point_set (ifilename, points);
    
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

  CGALApps::write_point_set (ofilename, points);

  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

