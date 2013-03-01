Helix
-----

- `helix.radius`:  The radius around which we draw the helix
- `helix.ribbon_thickness`: The thickness of the ribbons. The thinkness of the ribbon grows from the `radius` start, to `radius + ribbon_thickness` (in the outward direction).
- `helix.ribbon_height`: The height of the helix in the y-direction.
- `helix.angle_increment`:  Controls the number of rounds we make in the helix.
- `helix.grow_force`: How fast we grow in the y-direction
- `helix.tube_radius`: Radius of the tubes that connect the ribbons.
- `helix.tube_resolution`: Controls the number of triangles we create for the tubes.
- `helix.tube_interval`: Create a tube every X
- `helix.num_iterations`: How many 'steps' in the y-direction to we take. 
- `helix.draw_lines`: Draw lines so you can see the triangles.


How to change the smoothness
-----------------------------
Use `helix.angle_increment`, `helix.grow_forces` and `helix.num_iterations` to control the number of vertices and smoothness of the shape. 

For example use these settings to generate a lot of vertices:

- `helix.angle_increment`: 0.0075
- `helix.grow_force`: 1.4200
- `helix.num_iterations`: 500

Or use these settings to generate just a couple of vertices

- `helix.angle_increment`: 0.0637
- `helix.grow_force`: 10.00
- `helix.num_iterations`: 81

