#ifndef CGALAPPS_ARGS_H_
#define CGALAPPS_ARGS_H_

#include <map>
#include <vector>

namespace CGALApps
{

class Args
{
  typedef std::map<char, std::vector<std::string> > Short_map;
  typedef std::map<std::string, std::vector<std::string> > Long_map;
  Short_map m_short;
  Long_map m_long;
  
public:

  Args (int argc, char** argv)
  {
    if (argc == 1)
      return;

    typename Short_map::iterator default_short = m_short.end();
    
    typename Short_map::iterator last_short = m_short.end();
    typename Long_map::iterator last_long = m_long.end();
    for (int i = 1; i < argc; ++ i)
    {
      std::string a(argv[std::size_t(i)]);
      if (a.size() == 2 && a[0] == '-')
      {
        last_short = m_short.insert (std::make_pair (a[1], std::vector<std::string>())).first;
        last_long = m_long.end();
      }
      else if (a.size() > 2 && a[0] == '-' && a[1] == '-')
      {
        last_long = m_long.insert (std::make_pair (argv[std::size_t(i)]+2, std::vector<std::string>())).first;
        last_short = m_short.end();
      }
      else
      {
        if (last_long != m_long.end())
          last_long->second.push_back(a);
        else if (last_short != m_short.end())
          last_short->second.push_back(a);
        else
        {
          if (default_short == m_short.end())
            default_short = m_short.insert
              (std::make_pair ('\0', std::vector<std::string>())).first;
          default_short->second.push_back(a);
        }
      }
    }
  }

  std::string get_string (const char& s, const char* l, const std::string& def = std::string(), const std::size_t iter = 0) const
  {
    typename Short_map::const_iterator sit = m_short.find(s);
    if (sit != m_short.end())
      if (sit->second.size() > iter)
        return sit->second[iter];
    typename Long_map::const_iterator lit = m_long.find(std::string(l));
    if (lit != m_long.end())
      if (lit->second.size() > iter)
        return lit->second[iter];
    return def;
  }

  bool get_bool (const char& s, const char* l) const
  {
    typename Short_map::const_iterator sit = m_short.find(s);
    if (sit != m_short.end())
      return true;
    typename Long_map::const_iterator lit = m_long.find(std::string(l));
    if (lit != m_long.end())
      return true;
    return false;
  }
  
  int get_int (const char& s, const char* l, const int& def = 0) const
  {
    typename Short_map::const_iterator sit = m_short.find(s);
    if (sit != m_short.end())
      return std::atoi(sit->second.front().c_str());
    typename Long_map::const_iterator lit = m_long.find(std::string(l));
    if (lit != m_long.end())
      return std::atoi(lit->second.front().c_str());
    return def;
  }
  unsigned int get_uint (const char& s, const char* l, const unsigned int& def = 0) const
  {
    return static_cast<unsigned int>(get_int (s, l, static_cast<int>(def)));
  }
  std::size_t get_size_t (const char& s, const char* l, const std::size_t& def = 0) const
  {
    return static_cast<std::size_t>(get_int (s, l, static_cast<int>(def)));
  }

  double get_double (const char& s, const char* l, const double& def = 0.) const
  {
    typename Short_map::const_iterator sit = m_short.find(s);
    if (sit != m_short.end())
      return std::atof(sit->second.front().c_str());
    typename Long_map::const_iterator lit = m_long.find(std::string(l));
    if (lit != m_long.end())
      return std::atof(lit->second.front().c_str());
    return def;
  }
  float get_float (const char& s, const char* l, const float& def = 0) const
  {
    return static_cast<float>(get_double (s, l, static_cast<double>(def)));
  }


};

} // namespace CGALApps

#endif // CGALAPPS_ARGS_H_
