#include "Args.h"
#include "types.h"
#include "io.h"

#include "cluster_point_set.h"


#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  bool verbose;
  std::string ifilename;
  std::string ofilename;
  double cluster;
  std::size_t min_points;
  std::string format;
  
  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in PLY format", ofilename, "", "stdout");
  args.add_option ("cluster,c", "Maximum distances between connected points", cluster, 0.1);
  args.add_option ("min-points,m", "Minimum number of point in shape", min_points, 0, "1% of all");
  args.add_option ("mode,m", "Output mode: [filter/biggest/label]", format, "filter");
  
  if(!args.parse(argc, argv))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Cluster Point Set" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Clusters a point set given a tolerance and a minimum number of points."
              << std::endl << args.help() << std::endl
              << "Output modes:" << std::endl
              << "  filter:  Keep all clusters with enough points (default mode)" << std::endl
              << "  biggest: Only keep biggest component" << std::endl
              << "  label:   Keep all points and save one label per cluster" << std::endl;
    return EXIT_SUCCESS;
  }

  int method = -1;
  if (format == "filter")
    method = 0;
  else if (format == "biggest")
    method = 1;
  else if (format == "label")
    method = 2;
  else
  {
    std::cerr << "Error: unknown mode \"" << format << "\"" << std::endl;
    return EXIT_FAILURE;
  }

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Cluster Point Set" << std::endl;
    t.start();
  }
  Point_set points;

  CGALApps::read_point_set (ifilename, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  if (min_points == 0)
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

  CGALApps::write_point_set (ofilename, points);

  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }

  return EXIT_SUCCESS;
}

