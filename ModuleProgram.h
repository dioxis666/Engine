#pragma once
#include <MathGeoLib.h>

class ModuleProgram
{
public:
    ModuleProgram();
    ~ModuleProgram();

    bool Init(const char* vertexShaderPath, const char* fragmentShaderPath);
    void Use(float4x4 model, float4x4 view, float4x4 proj) const;
    bool CleanUp();
    char* LoadShaderSource(const char* shader_file_name);
    unsigned CompileShader(unsigned type, const char* source);
    unsigned CreateProgram(unsigned vtxShader, unsigned frgShader);
    
private:
    unsigned programId;
};
