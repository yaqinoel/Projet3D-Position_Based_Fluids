# Position Based Fluids

A course project to reproduce the Position Based Fluids (PBF) method.

## Requirements

- CMake 3.22 or later, Git, and a C/C++ toolchain with C++17 support.
- Internet access on the first CMake configure. CMake downloads GLM 1.0.1 and, when the viewer is enabled, GLFW 3.4. The GLAD sources are included in `third_party/glad`.
- A desktop graphics environment with OpenGL 3.3 Core support to run the viewer.

## Build and run from a terminal

Run these commands from the project root

```bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build --parallel
    ./build/pbf_glfw
```

Run `./build/pbf_glfw` to open the viewer or `./build/Projet3D_PBF` to run the command-line demo.

## Run in VS Code

1. Open the project root in VS Code and install Microsoft's **C/C++** and **CMake Tools** extensions.
2. Open the command palette (`⌘⇧P` on macOS or `Ctrl+Shift+P` on Windows and Linux). Run `CMake: Select a Kit` and choose your installed toolchain. Run `CMake: Configure`, then `CMake: Build`.
3. Run `CMake: Set Launch/Debug Target` and select `pbf_glfw` for the viewer or `Projet3D_PBF` for the command-line demo. Run `CMake: Run Without Debugging` or use the play button next to **Launch** in the CMake Tools sidebar.

## Viewer controls

| Input | Action |
| --- | --- |
| Hold the right mouse button and drag | Look around |
| `W` / `S` | Move forward / backward |
| `A` / `D` | Move left / right |
| `Q` / `E` | Move down / up |
