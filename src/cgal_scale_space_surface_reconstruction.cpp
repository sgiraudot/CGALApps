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
  bool verbose;
  std::string ifilename;
  std::string ofilename;
  unsigned int scales;
  unsigned int neighbors;
  std::string smoother;
  std::string mesher;
  unsigned int samples;
  unsigned int fitting;
  unsigned int monge;
  double alpha;
  bool force_manifold;
  double angle;
  double radius;
  double beta;
  double length;
  
  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in OFF format", ofilename, "", "stdout");
  args.add_option ("nb-scales,n", "Number of scales used", scales, 4);
  args.add_option ("neighbors,n", "Number of nearest neighbors used for smoothing", neighbors, 12);
  args.add_option ("fitting,f", "Degree of fitting used for smoothing", fitting, 2);
  args.add_option ("smoother,s", "Smoother used [jet/pca]", smoother, "jet");
  args.add_option ("mesher,m", "Mesher used [advancing/alpha]", mesher, "advancing");

  args.add_section ("Jet Smoother Parameter");
  args.add_option ("monge,m", "Monge degree", monge, 2);

  args.add_section ("Weighted PCA Smoother Parameter");
  args.add_option ("samples,S", "Number of samples used to estimate neighborhood radius", samples, 300);

  args.add_section ("Advancing Front Mesher Parameters");
  args.add_option ("radius,r", "Radius ratio bound", radius, 5.0);
  args.add_option ("beta,b", "Beta angle in radiants", beta, 0.52);
  args.add_option ("length,l", "Maximum length of a facet", length, 0., "no limit");

  args.add_section ("Alpha Shape Mesher Parameters");
  args.add_option ("alpha,a", "Radius of alpha sphere", alpha, 0.1);
  args.add_option ("manifold,M", "Force manifold output", force_manifold, false);
  args.add_option ("angle,A", "Maximum angle between two facets for manifold output", angle, 45);

  if(!args.parse(argc, argv))
  {
    std::cout << "-------------------------------------------------" << std::endl
              << "[CGALApps] Scale Space Surface Reconstruction" << std::endl
              << "-------------------------------------------------" << std::endl << std::endl
              << "Reconstructs an interpolating surface based on the a smoothed version on the point set."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }


  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Scale Space Surface Reconstruction" << std::endl
              << " * scales = " << scales << std::endl;
    t.start();
  }
  
  Point_set points;

  CGALApps::read_point_set (ifilename, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  Scale_space scale_space (points.points().begin(), points.points().end());

  if (smoother == "pca")
  {

    if (verbose)
      std::cerr << "Using weighted PCA smoother:" << std::endl
                << " * neighbors = " << neighbors << std::endl
                << " * samples = " << samples << std::endl;

    Weighted_PCA_smoother smoother (neighbors, samples);
    scale_space.increase_scale (scales, smoother);
  }
  else if (smoother == "jet")
  {
    if (verbose)
      std::cerr << "Using jet smoother:" << std::endl
                << " * neighbors = " << neighbors << std::endl
                << " * fitting = " << fitting << std::endl
                << " * monge = " << monge << std::endl;
    
    Jet_smoother smoother (neighbors, fitting, monge);
    scale_space.increase_scale (scales, smoother);
  }
  else
  {
    std::cerr << "Error: unknown smoother \"" << smoother << "\"" << std::endl;
    return EXIT_FAILURE;
  }

  if (mesher == "alpha")
  {
    if (verbose)
      std::cerr << "Using alpha shape mesher:" << std::endl
                << " * radius = " << radius << std::endl
                << " * force_manifold = " << (force_manifold ? "yes" : "no") << std::endl
                << " * angle = " << angle << std::endl;
    
    Alpha_shape_mesher mesher (radius, false, force_manifold, angle);
    scale_space.reconstruct_surface (mesher);
  }
  else if (mesher == "advancing")
  {
    if (verbose)
      std::cerr << "Using advancing front mesher:" << std::endl
                << " * radius = " << radius << std::endl
                << " * beta = " << beta << std::endl
                << " * length = " << length << std::endl;
    
    Advancing_front_mesher mesher (length, radius, beta);
    scale_space.reconstruct_surface (mesher);
  }
  else
  {
    std::cerr << "Error: unknown mesher \"" << mesher << "\"" << std::endl;
    return EXIT_FAILURE;
  }
  
  if (verbose)
    std::cerr << scale_space.number_of_facets() << " facet(s) created." << std::endl;
  
  CGALApps::write_surface (ofilename, points, CGAL::make_range (scale_space.facets_begin(),
                                                                scale_space.facets_end()));

  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

