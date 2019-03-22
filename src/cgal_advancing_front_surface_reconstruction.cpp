#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Advancing_front_surface_reconstruction.h>
#include <CGAL/Real_timer.h>

struct Length {

  double bound;

  Length(double bound)
    : bound(bound)
  {}

  template <typename AdvancingFront, typename Cell_handle>
  double operator() (const AdvancingFront& adv, Cell_handle& c,
                     const int& index) const
  {
    if(bound == 0){
      return adv.smallest_radius_delaunay_sphere (c, index);
    }

    double d  = 0;
    d = sqrt(squared_distance(c->vertex((index+1)%4)->point(),
                              c->vertex((index+2)%4)->point()));
    if(d>bound) return adv.infinity();
    d = sqrt(squared_distance(c->vertex((index+2)%4)->point(),
                              c->vertex((index+3)%4)->point()));
    if(d>bound) return adv.infinity();
    d = sqrt(squared_distance(c->vertex((index+1)%4)->point(),
                              c->vertex((index+3)%4)->point()));
    if(d>bound) return adv.infinity();

    return adv.smallest_radius_delaunay_sphere (c, index);
  }
};

int main (int argc, char** argv)
{
  bool verbose;
  std::string ifilename;
  std::string ofilename;
  double radius;
  double beta;
  double length;

  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in OFF format", ofilename, "", "stdout");
  args.add_option ("radius,r", "Radius ratio bound", radius, 5.0);
  args.add_option ("beta,b", "Beta angle in radiants", beta, 0.52);
  args.add_option ("length,l", "Maximum length of a facet", length, 0., "no limit");

  if(!args.parse(argc, argv))
  {
    std::cout << "-------------------------------------------------" << std::endl
              << "[CGALApps] Advancing Front Surface Reconstruction" << std::endl
              << "-------------------------------------------------" << std::endl << std::endl
              << "Reconstructs an interpolating surface based on a 3D Delaunay triangulation."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }


  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Advancing Front Surface Reconstruction" << std::endl
              << " * radius = " << radius << std::endl
              << " * beta = " << beta << std::endl
              << " * length = " << length << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (ifilename, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  Length priority(length);
  
  std::vector<Facet> facets;
  CGAL::advancing_front_surface_reconstruction(points.points().begin(),
                                               points.points().end(),
                                               std::back_inserter(facets),
                                               priority, radius, beta);

  if (verbose)
    std::cerr << facets.size() << " facet(s) created." << std::endl;
  
  CGALApps::write_surface (ifilename, points, facets);

  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

