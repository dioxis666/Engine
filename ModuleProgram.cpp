#include "ModuleProgram.h"
#include "Globals.h"
#include <GL/glew.h> //Init the GLEW library

ModuleProgram::ModuleProgram() : programId(0)
{
}

ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    LOG("Creating Shader Program");

    char* vertexShaderSource = LoadShaderSource(vertexShaderPath);
    char* fragmentShaderSource = LoadShaderSource(fragmentShaderPath);

    unsigned vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    programId = CreateProgram(vertexShader, fragmentShader);

    free(vertexShaderSource);
    free(fragmentShaderSource);

    return true;
}

void ModuleProgram::Use(float4x4 model, float4x4 view, float4x4 projection) const
{
    glUseProgram(programId);

    /*
    glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_TRUE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programId, "proj"), 1, GL_TRUE, &projection[0][0]);
    */
    
    glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
    glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(2, 1, GL_TRUE, &projection[0][0]);
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name)
{
    char* data = nullptr;
    FILE* file = nullptr;
    fopen_s(&file, shader_file_name, "rb");

    if (file)
    {
        fseek(file, 0, SEEK_END);
        size_t size = static_cast<size_t>(ftell(file));
        data = (char*)malloc(static_cast<size_t>(size) + 1);

        if (data == nullptr) {
            // Shows error message
            LOG("Error: Failed to allocate memory for shader source data");
        }
        else {
            fseek(file, 0, SEEK_SET);
            fread(data, 1, size, file);
            data[size] = 0;
        }

        fclose(file);
    }

    return data;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
    unsigned shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, 0);
    glCompileShader(shader_id);

    int res = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetShaderInfoLog(shader_id, len, &written, info);
            LOG("Log Info: %s", info);
            free(info);
        }
    }
    return shader_id;
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
    unsigned program_id = glCreateProgram();
    glAttachShader(program_id, vtx_shader);
    glAttachShader(program_id, frg_shader);
    glLinkProgram(program_id);
    int res;
    glGetProgramiv(program_id, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        int len = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            int written = 0;
            char* info = (char*)malloc(len);
            glGetProgramInfoLog(program_id, len, &written, info);
            LOG("Program Log Info: %s", info);
            free(info);
        }
    }
    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    return program_id;
}

bool ModuleProgram::CleanUp() 
{    
    glDeleteProgram(programId);
    return true;
}