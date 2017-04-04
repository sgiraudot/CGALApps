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


}

#endif // CGAL_TOOLS_IO_H
