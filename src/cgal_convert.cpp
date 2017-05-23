#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Point_set_3/Point_set_processing_3.h>

int main (int argc, char** argv)
{
  CGALApps::Args args (argc, argv);

  if (args.get_bool ('h', "help"))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Convert" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Converts a point set to another format (deduced from the user-given extension)."
              << std::endl << std::endl
              << " -i  --input      Input file" << std::endl
              << " -o  --output     Output file (default = out.ply)" << std::endl
              << " -a  --ascii      Force ASCII format for PLY" << std::endl;
    return EXIT_SUCCESS;
  }

  Point_set points;

  CGALApps::read_point_set (args, points);

  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  std::string output_filename = args.get_string('o', "output", "", 1);
  if(output_filename == "")
    output_filename = args.get_string('\0', "", "out.ply", 1);

  std::ofstream output(output_filename);
  
  if(CGALApps::extension_of_file_is(output_filename, "ply"))
  {
    if (!(args.get_bool ('a', "--ascii")))
      CGAL::set_binary_mode(std::cout);
    write_ply_point_set (output, points);
  }
  else if(CGALApps::extension_of_file_is(output_filename, "off"))
    write_off_point_set (output, points);
  else if(CGALApps::extension_of_file_is(output_filename, "xyz"))
    write_xyz_point_set (output, points);
  else
  {
    std::cerr << "Error: unknown output format." << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

