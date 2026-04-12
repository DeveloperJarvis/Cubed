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

## Vulkan Docs

```bash
https://docs.vulkan.org/spec/latest/
```

## glslangValidator

```bash
# in cmd
cd Cubed\Cubed-Client\Assets\Shaders
glslangValidator
glslangValidator -V

# for vert
>glslangValidator -V -o bin/basic.vert.spirv basic.vert.glsl
# for frag
>glslangValidator -V -o bin/basic.frag.spirv basic.frag.glsl
```

Created `Compile.bat` for same in the Shaders folder

## Graphics Debugging

You could use [Render Doc](https://renderdoc.org)

1. Setup `Executable Path`
2. Setup `Working directory` from solution
3. Click `Launch`
4. Project will load, take screenshot using `F12` then close it
5. Under `Event Browser` goto `vkCmdDrawIndexed`
6. Click `Mesh Viewer` you could see that actually
