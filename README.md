# CGALApps

Collection of small self-contained one-task programs designed to
easily apply CGAL algorithms to data sets and combine these algorithms
using standard Unix pipes.

## Documentation

All applications embed their own documentation accessible with the
argument `--help` (or `-h`). For example:

```sh
$ cgal_shape_detection --help
--------------------------
[CGALApps] Shape Detection
--------------------------

Efficient RANSAC algorithm to detect shapes in a point set.

 -i  --input        Input file
 -o  --output       Output file in PLY format (default = standard output)
 -p  --probability  Probability for search endurance (default = 0.05)
 -m  --min-points   Minimum number of points in shape (default = 1% of total)
 -e  --epsilon      Maximum tolerance from point to shape (default = 1% of bounding box diagonal)
 -n  --normal       Maximum normal deviation in radiants (default = 0.45)
 -c  --cluster      Maximum distances between connected points (default = 1% of bounding box diagonal)
Shapes:
 -P  --planes       Detect planes
 -C  --cylinders    Detect cylinders
 -N  --cones        Detect cones
 -T  --torus        Detect torus
 -S  --spheres      Detect spheres
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
* `cgal_compute_average_spacing`
* `cgal_grid_simplify_point_set`
* `cgal_jet_estimate_normals`
* `cgal_jet_smooth_point_set`
* `cgal_mst_orient_normals`
* `cgal_poisson_surface_reconstruction`
* `cgal_shape_detection`

## Todo

### Point Set Processing

* `estimate_k_neighbor_scale()`
* `estimate_range_scale()`
* `remove_outliers()`
* `random_simplify_point_set()`
* `hierarchy_simplify_point_set()`
* `wlop_simplify_and_regularize_point_set()`
* `bilateral_point_set()`
* `jet_estimate_normals()`
* `pca_estimate_normals()`
* `mst_orient_normals()`
* `edge_aware_upsample_point_set()`
* `vcm_estimate_normals()`
* `structure_point_set()`

### Surface Reconstruction

* `scale_space_surface_reconstruction()`
