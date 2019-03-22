#include "Args.h"
#include "types.h"
#include "io.h"

#include <CGAL/Real_timer.h>

int main (int argc, char** argv)
{
  bool verbose;
  std::string ifilename;
  std::string ofilename;
  bool ascii;
  
  CGALApps::Args args (verbose, ifilename);
  args.add_option ("output,o", "Output file", ofilename, "out.ply");
  args.add_option ("ascii,a", "Force ASCII format for PLY", ascii, false);

  if(!args.parse(argc, argv))
  {
    std::cout << "----------------------------------" << std::endl
              << "[CGALApps] Convert" << std::endl
              << "----------------------------------" << std::endl << std::endl
              << "Converts a point set to another format (deduced from the user-given extension)."
              << std::endl << args.help();
    return EXIT_SUCCESS;
  }

  CGAL::Real_timer t;
  if (verbose)
    std::cerr << "[CGALApps] Convert" << std::endl;
      
  Point_set points;

  CGALApps::read_point_set (ifilename, points);

  if (points.empty())
  {
    std::cerr << "Error: zero points read." << std::endl;
    return EXIT_FAILURE;
  }

  std::ofstream output(ofilename);
  
  if(CGALApps::extension_of_file_is(ofilename, "ply"))
  {
    if (!ascii)
    {
      if (verbose)
        std::cerr << " * Convertion to binary PLY" << std::endl;
      CGAL::set_binary_mode(output);
    }
    else if (verbose)
      std::cerr << " * Convertion to ASCII PLY" << std::endl;
    
    write_ply_point_set (output, points);
  }
  else if(CGALApps::extension_of_file_is(ofilename, "off"))
  {
    if (verbose)
      std::cerr << " * Convertion to OFF" << std::endl;
    write_off_point_set (output, points);
  }
  else if(CGALApps::extension_of_file_is(ofilename, "xyz"))
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

