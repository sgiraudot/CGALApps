# CGALApps

Collection of small self-contained one-task programs designed to
easily apply CGAL algorithms to data sets and combine these algorithms
using standard Unix pipes.

## Documentation

All applications embed their own documentation accessible with the
argument `--help` (or `-h`). For example:

```sh
$ cgal_efficient_ransac --help
---------------------------
[CGALApps] Efficient RANSAC
---------------------------

Efficient RANSAC algorithm to detect shapes in a point set.

Options:
  -h [ --help ]                         Display this help message
  -v [ --verbose ]                      Display info to stderr
  -i [ --input ] arg (=stdin)           Input file
  -o [ --output ] arg (=stdout)         Output file in PLY format
  -p [ --probability ] arg (=0.050000)  Probability for search endurance
  -m [ --min-points ] arg (=1% of total diagonal)
                                        Minimum number of points in shape
  -e [ --epsilon ] arg (=1% of bounding box diagonal)
                                        Maximum tolerance from point to shape
  -n [ --normal ] arg (=0.450000)       Maximum normal deviation in radiants
  -c [ --cluster ] arg (=1% of bounding box diagonal)
                                        Maximum distances between connected 
                                        points

Shapes:
  -P [ --planes ]                       Detect planes
  -C [ --cylinders ]                    Detect cylinders
  -N [ --cones ]                        Detect cones
  -T [ --torus ]                        Detect torues
  -S [ --spheres ]                      Detect spheres
```
For more information on the parameters, please refer to the
corresponding CGAL reference manual pages.

## Examples

To compute the average spacing using 6 nearest neighbors (default):

```sh
$ cgal_compute_average_spacing data/example.xyz
0.048374
```

To compute the average spacing using 12 nearest neighbors:

```sh
$ cgal_compute_average_spacing data/example.xyz -n 12
0.072842
```

To simplify a point set using a regular grid of size 0.5:

```sh
$ cgal_grid_simplify_point_set data/example.xyz -e 0.5 -o out.ply
```

To reconstruct a simplified version of a point set:

```sh
$ cgal_grid_simplify_point_set data/example.xyz -e 0.5 | cgal_advancing_front_surface_reconstruction -o reco.off
```

## Available Apps

* `cgal_advancing_front_surface_reconstruction`
* `cgal_cluster_point_set`
* `cgal_compute_average_spacing`
* `cgal_convert`
* `cgal_efficient_ransac`
* `cgal_grid_simplify_point_set`
* `cgal_jet_estimate_normals`
* `cgal_jet_smooth_point_set`
* `cgal_mst_orient_normals`
* `cgal_poisson_surface_reconstruction`
* `cgal_random_simplify_point_set`
* `cgal_remove_outliers`
* `cgal_scale_space_surface_reconstruction`

## Todo

### Point Set Processing

* `estimate_k_neighbor_scale()`
* `estimate_range_scale()`
* `hierarchy_simplify_point_set()`
* `wlop_simplify_and_regularize_point_set()`
* `bilateral_point_set()`
* `pca_estimate_normals()`
* `edge_aware_upsample_point_set()`
* `vcm_estimate_normals()`
* `structure_point_set()`
