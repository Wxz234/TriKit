# TriKit Engine

TriKit is a self-developed 3D-focused game engine based on DirectX 12, providing efficient graphics rendering, input handling, resource management, and scene management capabilities.

## Directory Structure

```
TriKit/
├── Engine/
│   ├── Core/          # Core engine code
│   ├── Rendering/     # Rendering system
│   ├── Input/         # Input system
│   ├── Resources/     # Resource management
│   └── Scene/         # Scene management
├── Examples/          # Example applications
├── CMakeLists.txt     # CMake build configuration
├── README.md          # English documentation
└── README_zh.md       # Chinese documentation
```

## Core Features

- **DirectX 12 Rendering**: Utilizes the latest DirectX 12 features to provide efficient 3D rendering capabilities
- **Window Management**: Creates and manages game windows, handles Windows messages
- **Input System**: Supports keyboard and mouse input
- **Resource Management**: Loads and manages textures, models, shaders, and other resources
- **Scene Management**: Manages game scenes and objects

## Build Instructions

1. Ensure CMake 3.16 or higher is installed
2. Ensure Windows SDK 10.0.19041.0 or higher is installed
3. Ensure a DirectX 12 compatible graphics card and drivers are installed

### Build Steps

```bash
# Create build directory
mkdir build
cd build

# Configure CMake
cmake ..

# Build project
cmake --build . --config Release
```

## Run Examples

After building, find `BasicExample.exe` in the `build/bin` directory. Running it will display a window with a blue background, demonstrating the basic functionality of the TriKit engine.

## Future Plans

- Improve rendering system to support more 3D features
- Add physics engine integration
- Implement advanced lighting and shadow effects
- Add material system
- Support more platforms

## License

MIT License

---

[Read this document in Chinese](README_zh.md)