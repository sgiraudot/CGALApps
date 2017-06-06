#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Point_set_3/Point_set_processing_3.h>
#include <CGAL/Real_timer.h>

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
              << " -v  --verbose  Display info to stderr" << std::endl
              << " -i  --input    Input file" << std::endl
              << " -o  --output   Output file (default = out.ply)" << std::endl
              << " -a  --ascii    Force ASCII format for PLY" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = args.get_bool('v', "verbose");
  
  CGAL::Real_timer t;
  if (verbose)
    std::cerr << "[CGALApps] Convert" << std::endl;
      
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
    {
      if (verbose)
        std::cerr << " * Convertion to binary PLY" << std::endl;
      CGAL::set_binary_mode(std::cout);
    }
    else if (verbose)
      std::cerr << " * Convertion to ASCII PLY" << std::endl;
    
    write_ply_point_set (output, points);
  }
  else if(CGALApps::extension_of_file_is(output_filename, "off"))
  {
    if (verbose)
      std::cerr << " * Convertion to OFF" << std::endl;
    write_off_point_set (output, points);
  }
  else if(CGALApps::extension_of_file_is(output_filename, "xyz"))
  {
    if (verbose)
      std::cerr << " * Convertion to XYZ" << std::endl;
    write_xyz_point_set (output, points);
  }
  else
  {
    if (verbose)
      std::cerr << "Error: unknown output format." << std::endl;
    return EXIT_FAILURE;
  }
  
  if (verbose)
  {
    t.stop();
    std::cerr << "Done in " << t.time() << " second(s)." << std::endl;
  }
  
  return EXIT_SUCCESS;
}

