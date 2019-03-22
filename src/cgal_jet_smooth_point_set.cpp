#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  bool verbose;
  std::string ifilename;
  std::string ofilename;
  unsigned int nb_neighbors;
  unsigned int fitting;
  unsigned int monge;
  std::size_t repeat;
  
  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in PLY format", ofilename, "", "stdout");
  args.add_option ("neighbors,n", "Number of nearest neighbors used", nb_neighbors, 12);
  args.add_option ("fitting,f", "Degree of fitting", fitting, 2);
  args.add_option ("monge,m", "Monge degree", monge, 2);
  args.add_option ("repeat,r", "Number of iterations", repeat, 1);
  
  if(!args.parse(argc, argv))
  {
    std::cout << "-------------------------------" << std::endl
              << "[CGALApps] Jet Smooth Point Set" << std::endl
              << "-------------------------------" << std::endl << std::endl
              << "Smooths a point set based on a parametric surface estimation."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }


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

  CGALApps::read_point_set (ifilename, points);
    
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

  CGALApps::write_point_set (ofilename, points);
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }

  return EXIT_SUCCESS;
}

