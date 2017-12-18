#ifndef CGALAPPS_IO_H
#define CGALAPPS_IO_H

#include <fstream>
#include <string>
#include <sstream>
#include <CGAL/Point_set_3/IO.h>

#include "Args.h"
#include "types.h"

namespace CGALApps
{

void read_point_set (const Args& args, Point_set& points, bool try_stdcin = true)
{
  // Input file can be given with -i or --input
  std::string filename = args.get_string ('i', "input");

  // Input file can also be given as first parameter without - or --
  if (filename == std::string())
    filename = args.get_string ('\0', "");

  // Otherwise, read from std::cin
  if (filename == std::string())
  {
    if (!try_stdcin)
      return;
    
    CGAL::read_ply_point_set (std::cin, points);
  }
  else
  {
    std::ifstream f(filename.c_str());
    f >> points;
  }
}

void write_point_set (const Args& args, Point_set& points, bool binary = true)
{
  // Output file can be given with -o or --output
  std::string filename = args.get_string ('o', "output");

  // Otherwise, write to std::cout
  if (filename == std::string())
  {
    if (binary)
      CGAL::set_binary_mode(std::cout);
    else
    {
      CGAL::set_ascii_mode(std::cout);
      std::cout.precision (std::numeric_limits<double>::digits10 + 2);
    }
          
    std::cout << points;
  }
  else
  {
    std::ofstream f(filename.c_str());
    if (binary)
      CGAL::set_binary_mode(f);
    else
    {
      CGAL::set_ascii_mode(f);
      f.precision (std::numeric_limits<double>::digits10 + 2);
    }
    f << points;
  }
}

template <typename FacetRange>
void write_surface_to_stream (Point_set& points,
                              const FacetRange& facets,
                              std::ostream& stream)
{
  stream.precision (std::numeric_limits<double>::digits10 + 2);
  stream << "OFF\n" << points.size() << " " << facets.size() << " 0\n";
  for (typename Point_set::const_iterator it = points.begin(); it != points.end(); ++ it)
    stream << points.point(*it) << std::endl;
  for (typename FacetRange::const_iterator it = facets.begin(); it != facets.end(); ++ it)
    stream << "3 " << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << std::endl;

}

template <typename FacetRange>
void write_surface (const Args& args, Point_set& points,
                    const FacetRange& facets)
{
  // Output file can be given with -o or --output
  std::string filename = args.get_string ('o', "output");

  // Otherwise, write to std::cout
  if (filename == std::string())
    write_surface_to_stream (points, facets, std::cout);
  else
  {
    std::ofstream f(filename.c_str());
    write_surface_to_stream (points, facets, f);
  }
}

void write_surface (const Args& args, const Polyhedron& mesh)
{
  // Output file can be given with -o or --output
  std::string filename = args.get_string ('o', "output");

  // Otherwise, write to std::cout
  if (filename == std::string())
  {
    std::cout.precision (std::numeric_limits<double>::digits10 + 2);
    std::cout << mesh;
  }
  else
  {
    std::ofstream f(filename.c_str());
    f.precision (std::numeric_limits<double>::digits10 + 2);
    f << mesh;
  }
}

bool extension_of_file_is (const std::string& filename, const char* ext)
{
  if(filename.size() < 5) // Filename is at least a.ext
    return false;

  return ((filename[filename.size() - 3] == ext[0] || filename[filename.size() - 3] == std::toupper(ext[0]))
          && (filename[filename.size() - 2] == ext[1] || filename[filename.size() - 2] == std::toupper(ext[1]))
          && (filename[filename.size() - 1] == ext[2] || filename[filename.size() - 1] == std::toupper(ext[2])));
}


}

#endif // CGALAPPS_IO_H
