#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/jet_estimate_normals.h>
#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  bool verbose;
  double epsilon;
  std::string ifilename;
  std::string ofilename;
  unsigned int nb_neighbors;
  unsigned int fitting;

  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in PLY format", ofilename, "", "stdout");
  args.add_option ("neighbors,n", "Number of nearest neighbors used", nb_neighbors, 12);
  args.add_option ("fitting,f", "Degree of fitting", fitting, 2);
  
  if(!args.parse(argc, argv))
  {
    std::cout << "-------------------------------" << std::endl
              << "[CGALApps] Jet Estimate Normals" << std::endl
              << "-------------------------------" << std::endl << std::endl
              << "Estimates the normal vectors of a point set based on a parametric surface estimation."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }
  
  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Jet Estimate Normals" << std::endl
              << " * neighbors = " << nb_neighbors << std::endl
              << " * fitting = " << fitting << std::endl;
    t.start();
  }

  Point_set points;

  CGALApps::read_point_set (ifilename, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  points.add_normal_map();
  CGAL::jet_estimate_normals<Concurrency_tag> (points, nb_neighbors,
                                               CGAL::parameters::point_map(points.point_map()).
                                               normal_map(points.normal_map()).
                                               degree_fitting(fitting));

  CGALApps::write_point_set (ofilename, points);
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

