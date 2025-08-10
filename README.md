# Rasterizer in C++ using OpenGL

If you want to use the project yourself, go to the latest release and download the binary from there

---

I just wanted to make a rasterizer inspired by this video from [Sebastian Lague's Youtube Channel](https://www.youtube.com/@SebastianLague)

<a href="https://www.youtube.com/watch?v=yyJ-hdISgnw" target="_blank">
<img src="https://img.youtube.com/vi/yyJ-hdISgnw/maxresdefault.jpg" width="500">
</a>

i first decided to do it in C# using RayLib like he did and it worked, but i wanted to make it faster so i decided to switch to C++ and OpenGL.

There are other easier ways to do rasterization in OpenGL (it basically does the rasterization for you) but i wanted the challenge of making it from scratch.

---

## TODO

 - [x] Create a window
 - [x] Draw a quad that covers the screen
 - [x] Draw a texture onto that quad
 - [x] Figure out some math
 - [x] Render a triangle
 - [x] Render multiple triangles with correct depth calculations
 - [ ] Be able to Read .obj files
 - [ ] Render 3D triangles
 - [ ] Clip triangles correctly
 - [ ] Render textures onto those 3D triangles
 - [ ] Create Shading with normals
 - [ ] (Maybe) Add some ui elements and polish the result