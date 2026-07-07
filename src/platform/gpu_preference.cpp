#include <windows.h>

// Standard exports used by the Windows graphics driver to determine if the application 
// should prefer the high-performance discrete GPU on Optimus/PowerXpress systems.
// This is done transparently before the OpenGL context is even created.

extern "C" 
{
    // NVIDIA Optimus
    // http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

    // AMD PowerXpress
    // https://community.amd.com/t5/opengl-vulkan/how-to-choose-different-gpus-by-c-code/m-p/132549
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
