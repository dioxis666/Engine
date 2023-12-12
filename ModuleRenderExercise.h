#pragma once
#include "Module.h"
#include "ModuleProgram.h"

class ModuleRenderExercise : public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();

    bool Init();
    update_status PreUpdate();
    update_status Update();
    update_status PostUpdate();
    bool CleanUp();

private:
    //unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;

    ModuleProgram helloWorldProgram;

    float4x4 model, view, projection;

    //++++++++++++++++++++++++++++++++++++++++++++++
    
    //TEXT:
    unsigned int texture; 

    //++++++++++++++++++++++++++++++++++++++++++++++
};