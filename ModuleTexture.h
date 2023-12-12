#pragma once
#include "Module.h"

class ModuleTexture : public Module
{
    public:
        struct TexMetadata {
            size_t width;
            size_t height;
            size_t depth;
            size_t arraySize;
            size_t mipLevels;
            uint32_t miscFlags;
            uint32_t miscFlags2;
            //DXGI_FORMAT format;
            //TEX_DIMENSION dimension;
        };

        ModuleTexture();
        ~ModuleTexture();
        bool Init(const wchar_t* file, unsigned int texture);
        bool LoadDDSTexture(const wchar_t* filename);

    private:
        unsigned int texture;
};

