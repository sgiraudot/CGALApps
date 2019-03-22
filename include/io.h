#ifndef CGALAPPS_IO_H
#define CGALAPPS_IO_H

#include <fstream>
#include <string>
#include <sstream>
#include <CGAL/Point_set_3/IO.h>

#include "types.h"

namespace CGALApps
{

void read_point_set (const std::string& filename, Point_set& points, bool try_stdcin = true)
{
  // Otherwise, read from std::cin
  if (filename == "")
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

void write_point_set (const std::string& filename, const Point_set& points, bool binary = true)
{
  // Otherwise, write to std::cout
  if (filename == "")
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
void write_surface_to_stream (const Point_set& points,
                              const FacetRange& facets,
                              std::ostream& stream)
{
  stream.precision (std::numeric_limits<double>::digits10 + 2);
  stream << "OFF\n" << points.size() << " " << facets.size() << " 0\n";
  for (const typename Point_set::Index& idx : points)
    stream << points.point(idx) << std::endl;
  for (const typename FacetRange::const_iterator::value_type& f : facets)
    stream << "3 " << f[0] << " " << f[1] << " " << f[2] << std::endl;

}

template <typename FacetRange>
void write_surface (const std::string& filename, const Point_set& points,
                    const FacetRange& facets)
{
  // Otherwise, write to std::cout
  if (filename == "")
    write_surface_to_stream (points, facets, std::cout);
  else
  {
    std::ofstream f(filename.c_str());
    write_surface_to_stream (points, facets, f);
  }
}

void write_surface (const std::string& filename, const Polyhedron& mesh)
{
  // Otherwise, write to std::cout
  if (filename == "")
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
