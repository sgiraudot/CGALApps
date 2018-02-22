#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "-------------------------------" << std::endl
              << "[CGALApps] Jet Smooth Point Set" << std::endl
              << "-------------------------------" << std::endl << std::endl
              << "Smooths a point set based on a parametric surface estimation."
              << std::endl << std::endl
              << " -v  --verbose    Display info to stderr" << std::endl
              << " -i  --input      Input file" << std::endl
              << " -o  --output     Output file in PLY format (default = standard output)" << std::endl
              << " -n  --neighbors  Number of nearest neighbors used (default = 12)" << std::endl
              << " -f  --fitting    Degree of fitting (default = 2)" << std::endl
              << " -m  --monge      Monge degree (default = 2)" << std::endl
              << " -r  --repeat     Number of iterations (default = 1)" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  unsigned int nb_neighbors = args.get_uint ('n', "neighbors", 12);
  unsigned int fitting = args.get_uint ('f', "fitting", 2);
  unsigned int monge = args.get_uint ('m', "monge", 2);
  std::size_t repeat = args.get_size_t ('r', "repeat", 1);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Jet Smooth Point Set" << std::endl
              << " * nb_neighbors = " << nb_neighbors << std::endl
              << " * fitting = " << fitting << std::endl
              << " * monge = " << monge << std::endl
              << " * repeat = " << repeat << std::endl;
    t.start();
  }

  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  for (std::size_t i = 0; i < repeat; ++ i)
    CGAL::jet_smooth_point_set<Concurrency_tag>
      (points, nb_neighbors,
       CGAL::parameters::point_map (points.point_map())
       .degree_fitting (fitting)
       .degree_monge (monge));

  CGALApps::write_point_set (args, points);
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }

  return EXIT_SUCCESS;
}

