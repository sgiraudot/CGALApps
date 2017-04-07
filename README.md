# CGALApps

Collection of small self-contained one-task programs designed to
easily apply CGAL algorithms to data sets and combine these algorithms
using standard Unix pipes.

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

* `poisson_surface_reconstruction()`
* `scale_space_surface_reconstruction()`
