# Building Galactic2

## Windows (recommended)

1. Install Visual Studio 2022 or Build Tools with **Desktop development with C++**.
2. Install CMake 3.20 or newer.
3. From the repository root, run:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
.\build\Release\galactic2.exe .\README.md
```

The executable accepts one regular-file path. Directory traversal and destructive operations are intentionally outside the current prototype.

## Other platforms

The prototype uses C++17 `std::filesystem` and can be built with a recent GCC or Clang, but Windows is the primary target. On non-Windows systems, the Windows attribute field is omitted.
