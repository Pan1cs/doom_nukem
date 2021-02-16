# Doom-Nukem
Final Project in the graphics branch of Hive Helsinki code school. Creating a very low level raycaster and a game with it.

# Design Reasonings
- We decided to pursue a track of creating Duke Nukem engine.
- Calculate angles as radians, player field of vision is calculated as radians, and are then translated to pixels using how much they use screen space in ratio to the whole field of vision in radians (approx. 1.57)s

Fabien's Sanglard's tutorial for Building Duke Nukem Graphics Engine [Link](https://fabiensanglard.net/duke3d/build_engine_internals.php)
