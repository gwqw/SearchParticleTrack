# Search Particle Tracks
My attempts to search linear particle tracks using `opencv` library

## Parameter string

```sh
search_tracks <image_name> <method> [<rho> <theta> [<threshold> [<minLineLength> <maxLineGap>]]]
image_name	path to image
method	0 - standard Hough transform, 1 - probabilistic Hough transform
rho		rho threshold in pixels
theta	theta threshold in degrees
threshold	threshold in pixels in line
```



## Dependency

`search_tracks` depends from OpenCV3.