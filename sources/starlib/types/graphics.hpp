#pragma once
namespace starlib
{
    typedef void* (*gl_loader_func) (const char *name);

    enum class graphics_api
    {
        GL45, VK13, DX11, DX12, METAL
    };
}