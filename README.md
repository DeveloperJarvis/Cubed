# Cubed

### Build in Visual Studio

```bash
Ctrl + Shift + B
```

### Run in Visual Studio

```bash
F5
```

## Errors

1. MVSC Error
   1. Visual Studio Installer -> select Desktop development with C++
   2. make sure these are checked
      - MVSC vXXX build tools
      - Windows 10/ 11 SDK
   3. Retarget project
      - Right click solution -> Retarget solution
      - Choose available toolset

2. Vulkan SDK error
   1. Make sure Vulkan SDK is installed on device
   2. Add to System Variables: VULKAN_SDK
   3. Rerun Setup.bat > Reload All
