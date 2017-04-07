#ifndef CGAL_TOOLS_IO_H
#define CGAL_TOOLS_IO_H

#include <fstream>
#include <string>
#include <sstream>
#include <CGAL/Point_set_3/IO.h>

#include "Args.h"
#include "types.h"

namespace CGALApps
{

void read_point_set (const Args& args, Point_set& points)
{
  // Input file can be given with -i or --input
  std::string filename = args.get_string ('i', "input");

  // Input file can also be given as first parameter without - or --
  if (filename == std::string())
    filename = args.get_string ('\0', "");

  // Otherwise, read from std::cin
  if (filename == std::string())
  {
    std::string input, line;
    while (getline(std::cin, line))
      input = input + line + "\n";
    std::istringstream iss(input);
    iss >> points;
  }
  else
  {
    std::ifstream f(filename.c_str());
    f >> points;
  }
}

void write_point_set (const Args& args, Point_set& points)
{
  // Output file can be given with -o or --output
  std::string filename = args.get_string ('o', "output");

  // Otherwise, write to std::cout
  if (filename == std::string())
    std::cout << points;
  else
  {
    std::ofstream f(filename.c_str());
    f << points;
  }
}

template <typename FacetRange>
void write_surface_to_stream (Point_set& points,
                              const FacetRange& facets,
                              std::ostream& stream)
{
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


}

#endif // CGAL_TOOLS_IO_H
