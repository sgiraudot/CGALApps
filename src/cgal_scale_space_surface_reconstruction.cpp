#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Scale_space_reconstruction_3/Advancing_front_mesher.h>
#include <CGAL/Scale_space_reconstruction_3/Jet_smoother.h>
#include <CGAL/Scale_space_reconstruction_3/Alpha_shape_mesher.h>
#include <CGAL/Scale_space_reconstruction_3/Weighted_PCA_smoother.h>
#include <CGAL/Real_timer.h>

typedef CGAL::Scale_space_surface_reconstruction_3<Kernel> Scale_space;
typedef CGAL::Scale_space_reconstruction_3::Advancing_front_mesher<Kernel> Advancing_front_mesher;
typedef CGAL::Scale_space_reconstruction_3::Alpha_shape_mesher<Kernel> Alpha_shape_mesher;
typedef CGAL::Scale_space_reconstruction_3::Jet_smoother<Kernel> Jet_smoother;
typedef CGAL::Scale_space_reconstruction_3::Weighted_PCA_smoother<Kernel> Weighted_PCA_smoother;

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "-------------------------------------------------" << std::endl
              << "[CGALApps] Scale Space Surface Reconstruction" << std::endl
              << "-------------------------------------------------" << std::endl << std::endl
              << "Reconstructs an interpolating surface based on the a smoothed version on the point set."
              << std::endl << std::endl
              << " -v  --verbose          Display info to stderr" << std::endl
              << " -i  --input            Input file" << std::endl
              << " -o  --output           Output file in OFF format (default = standard output)" << std::endl
              << " -s  --scales           Number of scales used (default = 4)" << std::endl
              << std::endl
              << " -j  --jet              Use jet smoother (default)" << std::endl
              << "Parameters:" << std::endl
              << " -n  --neighbors        Number of nearest neighbors used (default = 12)" << std::endl
              << " -f  --fitting          Degree of fitting (default = 2)" << std::endl
              << " -m  --monge            Monge degree (default = 2)" << std::endl
              << std::endl
              << " -W  --weighted-pca     Use weighted PCA smoother" << std::endl
              << "Parameters:" << std::endl
              << " -n  --neighbors        Number of nearest neighbors used (default = 12)" << std::endl
              << " -S  --samples          Number of samples used to estimate neighborhood radius (default = 300)" << std::endl
              << std::endl
              << " -F  --advancing-front  Use advancing front mesher (default)" << std::endl
              << "Parameters:" << std::endl
              << " -r  --radius           Radius ratio bound (default = 5.0)" << std::endl
              << " -b  --beta             Beta angle in radiants (default = 0.52)" << std::endl
              << " -l  --length           Maximum length of a facet (default = no limit)" << std::endl
              << std::endl
              << " -A  --alpha-shape      Use alpha shape mesher" << std::endl
              << "Parameters:" << std::endl
              << " -r  --radius           Radius of alpha sphere (default = 0.1)" << std::endl
              << " -M  --manifold         Force manifold output (default = false)" << std::endl
              << " -a  --angle            Maximal angle between two facets for manifold output (default = 45)" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  int scales = args.get_size_t ('s', "scales", 4);

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Scale Space Surface Reconstruction" << std::endl
              << " * scales = " << scales << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  Scale_space scale_space (points.points().begin(), points.points().end());

  if (args.get_bool('W', "weighted-pca"))
  {
    unsigned int neighbors = args.get_uint('n', "neighbors", 12);
    unsigned int samples = args.get_uint('S', "samples", 300);

    if (verbose)
      std::cerr << "Using weighted PCA smoother:" << std::endl
                << " * neighbors = " << neighbors << std::endl
                << " * samples = " << samples << std::endl;

    Weighted_PCA_smoother smoother (neighbors, samples);
    scale_space.increase_scale (scales, smoother);
  }
  else
  {
    unsigned int neighbors = args.get_uint('n', "neighbors", 12);
    unsigned int fitting = args.get_uint ('f', "fitting", 2);
    unsigned int monge = args.get_uint ('m', "monge", 2);
    
    if (verbose)
      std::cerr << "Using jet smoother:" << std::endl
                << " * neighbors = " << neighbors << std::endl
                << " * fitting = " << fitting << std::endl
                << " * monge = " << monge << std::endl;
    
    Jet_smoother smoother (neighbors, fitting, monge);
    scale_space.increase_scale (scales, smoother);
  }

  if (args.get_bool('A', "alpha-shape"))
  {
    double radius = args.get_double('r', "radius", 0.1);
    bool force_manifold = args.get_bool('M', "manifold");
    double angle = args.get_double('a', "angle", 45.0);

    if (verbose)
      std::cerr << "Using alpha shape mesher:" << std::endl
                << " * radius = " << radius << std::endl
                << " * force_manifold = " << (force_manifold ? "yes" : "no") << std::endl
                << " * angle = " << angle << std::endl;
    
    Alpha_shape_mesher mesher (radius, false, force_manifold, angle);
    scale_space.reconstruct_surface (mesher);
  }
  else
  {
    double radius = args.get_double ('r', "radius", 5.0);
    double beta = args.get_double ('b', "beta", 0.52);
    double length = args.get_double ('l', "length", 0.0);

    if (verbose)
      std::cerr << "Using advancing front mesher:" << std::endl
                << " * radius = " << radius << std::endl
                << " * beta = " << beta << std::endl
                << " * length = " << length << std::endl;
    
    Advancing_front_mesher mesher (length, radius, beta);
    scale_space.reconstruct_surface (mesher);
  }
  
  if (verbose)
    std::cerr << scale_space.number_of_facets() << " facet(s) created." << std::endl;
  
  CGALApps::write_surface (args, points, CGAL::make_range (scale_space.facets_begin(),
                                                           scale_space.facets_end()));

  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

