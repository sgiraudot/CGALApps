#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Shape_detection_3.h>
#include <CGAL/Real_timer.h>

typedef CGAL::Shape_detection_3::Shape_detection_traits
<Kernel, Point_set, Point_set::Point_map, Point_set::Vector_map> Traits;
typedef CGAL::Shape_detection_3::Efficient_RANSAC<Traits>        Efficient_ransac;
typedef CGAL::Shape_detection_3::Plane<Traits>                   Plane_shape;
typedef CGAL::Shape_detection_3::Cylinder<Traits>                Cylinder_shape;
typedef CGAL::Shape_detection_3::Cone<Traits>                    Cone_shape;
typedef CGAL::Shape_detection_3::Torus<Traits>                   Torus_shape;
typedef CGAL::Shape_detection_3::Sphere<Traits>                  Sphere_shape;


int main (int argc, char** argv)
{
  bool verbose;
  std::string ifilename;
  std::string ofilename;
  Efficient_ransac::Parameters parameters;
  bool planes, cylinders, cones, torus, spheres;
  
  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file in PLY format", ofilename, "", "stdout");
  args.add_option ("probability,p", "Probability for search endurance", parameters.probability, 0.05);
  args.add_option ("min-points,m", "Minimum number of points in shape", parameters.min_points,
                   std::numeric_limits<std::size_t>::max(), "1% of total diagonal");
  args.add_option ("epsilon,e", "Maximum tolerance from point to shape", parameters.epsilon,
                   -1, "1% of bounding box diagonal");
  args.add_option ("normal,n", "Maximum normal deviation in radiants", parameters.normal_threshold, 0.45);
  args.add_option ("cluster,c", "Maximum distances between connected points", parameters.cluster_epsilon,
                   -1, "1% of bounding box diagonal");

  args.add_section ("Shapes");
  args.add_option ("planes,P", "Detect planes", planes, false);
  args.add_option ("cylinders,C", "Detect cylinders", cylinders, false);
  args.add_option ("cones,N", "Detect cones", cones, false);
  args.add_option ("torus,T", "Detect torues", torus, false);
  args.add_option ("spheres,S", "Detect spheres", spheres, false);

  if(!args.parse(argc, argv))
  {
    std::cout << "---------------------------" << std::endl
              << "[CGALApps] Efficient RANSAC" << std::endl
              << "---------------------------" << std::endl << std::endl
              << "Efficient RANSAC algorithm to detect shapes in a point set."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }

  CGAL::Real_timer t;
  if (verbose)
  {
    std::cerr << "[CGALApps] Remove Outliers" << std::endl
              << " * probability = " << parameters.probability << std::endl
              << " * min-points = " << parameters.min_points << std::endl
              << " * epsilon = " << parameters.epsilon << std::endl
              << " * cluster = " << parameters.cluster_epsilon << std::endl
              << " * normal = " << parameters.normal_threshold << std::endl
              << " * shapes =";
    if (planes)
      std::cerr << " planes";
    if (cylinders)
      std::cerr << " cylinders";
    if (cones)
      std::cerr << " cones";
    if (torus)
      std::cerr << " torus";
    if (spheres)
      std::cerr << " spheres";
    std::cerr << std::endl;
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

  Efficient_ransac ransac;
  ransac.set_input(points,
                   points.point_map(),
                   points.normal_map());

  bool at_least_one_shape = false;
  if (planes)
  {
    ransac.add_shape_factory<Plane_shape>();
    at_least_one_shape = true;
  }
  if (cylinders)
  {
    ransac.add_shape_factory<Cylinder_shape>();
    at_least_one_shape = true;
  }
  if (cones)
  {
    ransac.add_shape_factory<Cone_shape>();
    at_least_one_shape = true;
  }
  if (torus)
  {
    ransac.add_shape_factory<Torus_shape>();
    at_least_one_shape = true;
  }
  if (spheres)
  {
    ransac.add_shape_factory<Sphere_shape>();
    at_least_one_shape = true;
  }

  if (!at_least_one_shape)
  {
    std::cerr << "Error: at least one shape type should be used (see --planes, --cylinders, etc)." << std::endl;
    return EXIT_FAILURE;
  }

  ransac.detect(parameters);

  if (verbose)
    std::cerr << ransac.shapes().size() << " shape(s) detected." << std::endl;

  Point_set::Property_map<int> label_map = points.add_property_map<int>("shape_index", -1).first;
  Point_set::Property_map<unsigned char> red_map = points.add_property_map<unsigned char>("red", 0).first;
  Point_set::Property_map<unsigned char> green_map = points.add_property_map<unsigned char>("green", 0).first;
  Point_set::Property_map<unsigned char> blue_map = points.add_property_map<unsigned char>("blue", 0).first;

  CGAL::Random rand(time(0));
  int idx = 0;
  BOOST_FOREACH(boost::shared_ptr<Efficient_ransac::Shape> shape, ransac.shapes())
  {
    unsigned char r, g, b;

    r = static_cast<unsigned char>(64 + rand.get_int(0, 192));
    g = static_cast<unsigned char>(64 + rand.get_int(0, 192));
    b = static_cast<unsigned char>(64 + rand.get_int(0, 192));

    BOOST_FOREACH(std::size_t i, shape->indices_of_assigned_points())
    {
      Point_set::Index pos = *(points.begin() + i);
      label_map[pos] = idx;
      red_map[pos] = r;
      green_map[pos] = g;
      blue_map[pos] = b;
    }
    ++ idx;
  }
    
  CGALApps::write_point_set (ofilename, points);

  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }

  return EXIT_SUCCESS;
}

