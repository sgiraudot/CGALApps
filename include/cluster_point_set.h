#ifndef CGALAPPS_CLUSTER_POINT_SET_H
#define CGALAPPS_CLUSTER_POINT_SET_H

#include <CGAL/Search_traits_3.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/Fuzzy_sphere.h>

#include "types.h"

namespace CGALApps
{

  void cluster_point_set (Point_set& points, double cluster_tolerance, std::size_t min_points, int method)
  {
    typedef CGAL::Search_traits_3<Kernel> Traits_base;
    typedef CGAL::Search_traits_adapter<typename Point_set::Index,
                                        typename Point_set::Point_map,
                                        Traits_base>  Tree_traits;

    typedef CGAL::Orthogonal_k_neighbor_search<Tree_traits> Neighbor_search;
    typedef Neighbor_search::Tree Tree;
    typedef Tree::Splitter Splitter;
    typedef CGAL::Fuzzy_sphere<Tree_traits> Sphere;

    typedef std::vector<typename Point_set::Index> Cluster;

    Tree_traits traits (points.point_map());
    Tree tree (points.begin(), points.end(), Splitter(), traits);
    
    std::vector<Cluster> clusters;
    std::set<typename Point_set::Index> done;
    
    std::size_t max_size = 0;
    std::size_t max_index = 0;
    for (typename Point_set::iterator it = points.begin(); it != points.end(); ++ it)
    {
      clusters.push_back (Cluster());

      std::deque<typename Point_set::Index> todo;
      todo.push_back (*it);
      
      while (!(todo.empty()))
      {
        typename Point_set::Index current = todo.front();
        todo.pop_front();
        if (!(done.insert (current).second))
          continue;
        clusters.back().push_back (current);
          
        Sphere s(points.point(current), cluster_tolerance, 0., traits);
        tree.search (std::back_inserter (todo), s);
      }
      if (clusters.back().empty())
      {
        clusters.pop_back();
        continue;
      }
      
      if (clusters.back().size() >= min_points)
      {
        if (clusters.back().size() > max_size)
        {
          max_size = clusters.back().size();
          max_index = clusters.size() - 1;
        }
      }
      else
        clusters.pop_back();
    }

    if (method == 2) // label
    {
      typename Point_set::Property_map<int> label = points.add_property_map<int>("label", -1).first;
      for (std::size_t i = 0; i < clusters.size(); ++ i)
        for (std::size_t j = 0; j < clusters[i].size(); ++ j)
          label[clusters[i][j]] = (int)i;
    }
    else
    {

      typename Point_set::iterator it = points.begin();
      
      if (method == 0) // filter
        for (std::size_t i = 0; i < clusters.size(); ++ i)
          for (std::size_t j = 0; j < clusters[i].size(); ++ j)
          {
            typename Point_set::iterator current = it ++;
            *current = clusters[i][j];
          }
      else // biggest
        for (std::size_t j = 0; j < clusters[max_index].size(); ++ j)
          {
            typename Point_set::iterator current = it ++;
            *current = clusters[max_index][j];
          }

      points.remove_from(it);
    }
  }
   
} // namespace CGALApps

#endif // CGALAPPS_ARGS_H_
