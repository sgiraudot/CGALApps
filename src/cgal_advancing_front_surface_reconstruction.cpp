#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Advancing_front_surface_reconstruction.h>

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
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "-------------------------------------------------" << std::endl
              << "[CGALApps] Advancing Front Surface Reconstruction" << std::endl
              << "-------------------------------------------------" << std::endl << std::endl
              << "Reconstructs an interpolating surface based on a 3D Delaunay triangulation."
              << std::endl << std::endl
              << " -i  --input   Input file" << std::endl
              << " -o  --output  Output file in OFF format (default = standard output)" << std::endl
              << " -r  --radius  Radius ratio bound (default = 5.0)" << std::endl
              << " -b  --beta    Beta angle in radiants (default = 0.52)" << std::endl
              << " -l  --length  Maximum length of a facet (default = no limit)" << std::endl;
    return EXIT_SUCCESS;
  }

  double radius = args.get_double ('r', "radius", 5.0);
  double beta = args.get_double ('b', "beta", 0.52);
  double length = args.get_double ('l', "length", 0.0);

  Point_set points;

  CGALApps::read_point_set (args, points);
    
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

  CGALApps::write_surface (args, points, facets);
  
  return EXIT_SUCCESS;
}

