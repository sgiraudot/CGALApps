#ifndef CGALAPPS_TYPES_H
#define CGALAPPS_TYPES_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Polyhedron_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;

typedef CGAL::Point_set_3<Point_3, Vector_3> Point_set;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

typedef std::array<std::size_t, 3> Facet;
typedef std::array<unsigned char, 3> Color;

// Concurrency
#ifdef CGAL_LINKED_WITH_TBB
typedef CGAL::Parallel_tag Concurrency_tag;
#else
typedef CGAL::Sequential_tag Concurrency_tag;
#endif

#endif // CGALAPPS_TYPES_H
