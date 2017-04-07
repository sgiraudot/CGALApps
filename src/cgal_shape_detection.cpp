#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Shape_detection_3.h>

typedef CGAL::Shape_detection_3::Efficient_RANSAC_traits
<Kernel, Point_set, Point_set::Point_map, Point_set::Vector_map> Traits;
typedef CGAL::Shape_detection_3::Efficient_RANSAC<Traits>        Efficient_ransac;
typedef CGAL::Shape_detection_3::Plane<Traits>                   Plane_shape;
typedef CGAL::Shape_detection_3::Cylinder<Traits>                Cylinder_shape;
typedef CGAL::Shape_detection_3::Cone<Traits>                    Cone_shape;
typedef CGAL::Shape_detection_3::Torus<Traits>                   Torus_shape;
typedef CGAL::Shape_detection_3::Sphere<Traits>                  Sphere_shape;


int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "--------------------------" << std::endl
              << "[CGALApps] Shape Detection" << std::endl
              << "--------------------------" << std::endl << std::endl
              << "Efficient RANSAC algorithm to detect shapes in a point set."
              << std::endl << std::endl
              << " -i  --input        Input file" << std::endl
              << " -o  --output       Output file in PLY format (default = standard output)" << std::endl
              << " -p  --probability  Probability for search endurance (default = 0.05)" << std::endl
              << " -m  --min-points   Minimum number of points in shape (default = 1% of total)" << std::endl
              << " -e  --epsilon      Maximum tolerance from point to shape (default = 1% of bounding box diagonal)" << std::endl
              << " -n  --normal       Maximum normal deviation in radiants (default = 0.45)" << std::endl
              << " -c  --cluster      Maximum distances between connected points (default = 1% of bounding box diagonal)" << std::endl
              << "Shapes:" << std::endl
              << " -P  --planes       Detect planes" << std::endl
              << " -C  --cylinders    Detect cylinders" << std::endl
              << " -N  --cones        Detect cones" << std::endl
              << " -T  --torus        Detect torus" << std::endl
              << " -S  --spheres      Detect spheres" << std::endl;
    return EXIT_SUCCESS;
  }
  
  Point_set points;

  CGALApps::read_point_set (args, points);
    
  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  if (!(points.has_normal_map()))
  {
    std::cerr << "Error: point set does not have normal vectors" << std::endl;
    return EXIT_FAILURE;
  }

  Efficient_ransac ransac;
  ransac.set_input(points,
                   points.point_map(),
                   points.normal_map());

  bool at_least_one_shape = false;
  if (args.get_bool ('P', "planes"))
  {
    ransac.add_shape_factory<Plane_shape>();
    at_least_one_shape = true;
  }
  if (args.get_bool ('C', "cylinder"))
  {
    ransac.add_shape_factory<Cylinder_shape>();
    at_least_one_shape = true;
  }
  if (args.get_bool ('N', "cone"))
  {
    ransac.add_shape_factory<Cone_shape>();
    at_least_one_shape = true;
  }
  if (args.get_bool ('T', "torus"))
  {
    ransac.add_shape_factory<Torus_shape>();
    at_least_one_shape = true;
  }
  if (args.get_bool ('S', "sphere"))
  {
    ransac.add_shape_factory<Sphere_shape>();
    at_least_one_shape = true;
  }

  if (!at_least_one_shape)
  {
    std::cerr << "Error: at least one shape type should be used (see --planes, --cylinders, etc)." << std::endl;
    return EXIT_FAILURE;
  }

  Efficient_ransac::Parameters parameters;
  parameters.probability = args.get_double('p', "probability", 0.05);
  parameters.min_points = args.get_size_t('m', "min-points", (std::numeric_limits<std::size_t>::max)());
  parameters.epsilon = args.get_double('e', "epsilon", -1);
  parameters.cluster_epsilon = args.get_double('c', "cluster", -1);
  parameters.normal_threshold = std::cos(args.get_double('n', "normal", 0.45));
  ransac.detect(parameters);

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
    
  CGALApps::write_point_set (args, points);

  return EXIT_SUCCESS;
}

