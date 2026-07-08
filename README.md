A lightweight **OpenGL** application for loading `.obj` models into a scene, lighting them, and tweaking their transforms in real time through an **ImGui**-based editor.

Built from scratch in **C++** with a custom OBJ parser and a small OpenGL renderer - no Assimp or other model-import library involved.

---

### Features

* **Custom OBJ loader** - parses vertex positions, texture coordinates, and normals directly from `.obj` files (supports both triangulated and quad faces, splitting quads into triangles automatically).
* **Multiple models in one scene** - add as many models as you like, each with its own transform and material.
* **Live transform editing** - position, scale, and rotation sliders per model.
* **Dynamic lighting** - add, edit, and delete any number of:
  * Point lights
  * Directional lights
  * Spot lights (with adjustable inner/outer cutoff angles)
* **ImGui editor panels** - a "Model Inspector" scene tree and a contextual "Editor" panel

---
