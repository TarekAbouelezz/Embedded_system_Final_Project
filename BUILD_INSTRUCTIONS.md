# Build Instructions

## Windows

### Using Visual Studio

1. Open PowerShell or Command Prompt in the project directory
2. Create build directory:
   ```powershell
   mkdir build
   cd build
   ```
3. Generate Visual Studio solution:
   ```powershell
   cmake .. -G "Visual Studio 18 2026" -A x64
   ```
   (Adjust the generator name based on your Visual Studio version)
4. Build the solution:
   ```powershell
   cmake --build . --config Release
   ```
5. Run the executable:
   ```powershell
   .\Release\fas_simulator.exe
   ```

### Using MinGW

1. Install MinGW-w64 and add to PATH
2. In PowerShell/CMD:
   ```powershell
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles"
   cmake --build .
   .\fas_simulator.exe
   ```

## Linux

```bash
mkdir build
cd build
cmake ..
make
./fas_simulator
```

## macOS

```bash
mkdir build
cd build
cmake ..
make
./fas_simulator
```

## Troubleshooting

### Missing Threads Library (Linux/macOS)
If you get errors about pthread, install build-essential (Linux) or Xcode Command Line Tools (macOS):
- Linux: `sudo apt-get install build-essential`
- macOS: `xcode-select --install`

### CMake Not Found
Install CMake:
- Windows: Download from cmake.org or use `choco install cmake`
- Linux: `sudo apt-get install cmake`
- macOS: `brew install cmake`

### C++17 Not Supported
Update your compiler:
- GCC: Version 7 or higher
- Clang: Version 5 or higher
- MSVC: Visual Studio 2017 or higher


