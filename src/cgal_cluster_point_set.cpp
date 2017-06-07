#include "Args.h"
#include "types.h"
#include "io.h"

#include "cluster_point_set.h"


#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Cluster Point Set" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Clusters a point set given a tolerance and a minimum number of points."
              << std::endl << std::endl
              << " -v  --verbose     Display info to stderr" << std::endl
              << " -i  --input       Input file" << std::endl
              << " -o  --output      Output file in PLY format (default = standard output)" << std::endl
              << " -c  --cluster     Maximum distances between connected points (default = 0.1)" << std::endl
              << " -m  --min-points  Minimum number of points in shape (default = 1% of total)" << std::endl
              << "Output:" << std::endl
              << " -F  --filter      Keep all clusters with enough points (default = true)" << std::endl
              << " -B  --biggest     Only keep biggest component (default = false)" << std::endl
              << " -L  --label       Keep all points and save one label per cluster (default = false)" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  double cluster = args.get_double ('c', "cluster", 0.1);
  std::size_t min_points = args.get_size_t ('m', "min_points", (std::numeric_limits<std::size_t>::max)());

  int method = 0;
  if (args.get_bool('B', "biggest"))
    method = 1;
  else if (args.get_bool('L', "label"))
    method = 2;

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Cluster Point Set" << std::endl;
    t.start();
  }
  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  if (min_points == std::numeric_limits<std::size_t>::max())
    min_points = std::size_t(points.size() * 0.01);
  
  if (verbose)
  {
    std::cerr << " * cluster = " << cluster << std::endl
              << " * min-points = " << min_points << std::endl
              << " * output = ";
    if (method == 1)
      std::cerr << "biggest" << std::endl;
    else if (method == 2)
      std::cerr << "label" << std::endl;
    else
      std::cerr << "filter" << std::endl;

  }
  
  CGALApps::cluster_point_set (points, cluster, min_points, method);

  if (verbose && (method == 0 || method == 1))
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

