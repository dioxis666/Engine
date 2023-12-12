#include "ModuleTexture.h"
#include <GL/glew.h> //Init the GLEW library
#include "DirectXTex.h"

ModuleTexture::ModuleTexture() { }
ModuleTexture::~ModuleTexture() {}

bool ModuleTexture::Init(const wchar_t* file, unsigned int import_texture)
{
    LOG("Loading textures");
    texture = import_texture;

    if (!LoadDDSTexture(file)) {
       LOG("Error Loading image texture file");
       return false;
    }
    else {      
        return true;
    }
}

bool ModuleTexture::LoadDDSTexture(const wchar_t* filename) {
    DirectX::ScratchImage image;
    DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, nullptr, image);

    //Get metadata from image
    DirectX::TexMetadata metadata;
    DirectX::GetMetadataFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, metadata);

    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;

    switch (metadata.format) {
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
        type = GL_UNSIGNED_BYTE;
        break;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
        internalFormat = GL_RGBA8;
        format = GL_BGRA;
        type = GL_UNSIGNED_BYTE;
        break;
    case DXGI_FORMAT_B5G6R5_UNORM:
        internalFormat = GL_RGB8;
        format = GL_BGR;
        type = GL_UNSIGNED_BYTE;
        break;
    default:
        assert(false && "Unsupported format");
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, texture);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Rotar la imagen (flip verticalmente)
    DirectX::ScratchImage rotatedImage;
    HRESULT hrRotate = DirectX::FlipRotate(
        image.GetImages(), image.GetImageCount(), image.GetMetadata(),
        DirectX::TEX_FR_FLIP_VERTICAL,
        rotatedImage
    );

    // Verificar si la rotación fue exitosa
    if (SUCCEEDED(hrRotate)) {
        // Utilizar la imagen rotada para cargarla en OpenGL
        for (size_t i = 0; i < rotatedImage.GetMetadata().mipLevels; ++i) {
            const DirectX::Image* mip = rotatedImage.GetImage(i, 0, 0);
            glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);
        }

        // Establecer parámetros de textura para mapeo de mipmaps
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, rotatedImage.GetMetadata().mipLevels - 1);

        return true;
    }
    else {
        LOG("Image rotation failed");
        return false;
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    /*
    // Iterar a través de los niveles de mipmap e inicializar cada uno
    for (size_t i = 0; i < metadata.mipLevels; ++i) {
        const DirectX::Image* mip = image.GetImage(i, 0, 0);
        glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);
    }

    // Establecer parámetros de textura para mapeo de mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, metadata.mipLevels - 1);
    */

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    return true;
}