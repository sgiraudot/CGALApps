#ifndef CGALAPPS_ARGS_H_
#define CGALAPPS_ARGS_H_

#include <boost/program_options.hpp>

#include <iostream>
#include <map>
#include <vector>

namespace CGALApps
{

class Args
{
  typedef boost::program_options::options_description Desc;
  Desc all;
  std::vector<Desc> desc;

  template <typename T>
  std::string get_default_str (const T& value, const std::string& default_str)
  {
    if (default_str != std::string())
      return default_str;
    return std::to_string(value);
  }

  std::string get_default_str (const char* value, const std::string& default_str)
  {
    if (default_str != std::string())
      return default_str;
    return std::string(value);
  }
  
public:

  Args (bool& verbose, std::string& ifilename) : desc(1, Desc("Options"))
  {
    desc[0].add_options() ("help,h", "Display this help message");
    this->add_option ("verbose,v", "Display info to stderr", verbose, false);
    this->add_option ("input,i", "Input file", ifilename, "", "stdin");
    std::cout.precision(2);
  }

  void add_section(const std::string& name)
  {
    all.add(desc.back());
    desc.push_back(Desc(name));
  }
  
  template <typename RefType, typename DefType>
  void add_option (const char* tags, const char* description, RefType& var, const DefType& default_value,
                   const std::string& str_default_value = std::string())
  {
    desc.back().add_options()(tags,
                              boost::program_options::value<RefType>(&var)->default_value
                              (default_value, get_default_str(default_value, str_default_value)), description);
  }

  void add_option (const char* tags, const char* description, bool& var, const bool& default_value)
  {
    desc.back().add_options()(tags, boost::program_options::bool_switch(&var)->default_value(default_value), description);
  }

  bool parse(int argc, char** argv)
  {
    all.add(desc.back());
    boost::program_options::positional_options_description p;
    p.add("input", -1);
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv)
                                  .options(all).positional(p).run(), vm);
    boost::program_options::notify(vm);

    return !vm.count("help");
  }

  const boost::program_options::options_description& help() const { return all; }
};

} // namespace CGALApps

#endif // CGALAPPS_ARGS_H_
