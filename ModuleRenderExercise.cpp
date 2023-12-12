#include "ModuleRenderExercise.h"
#include <GL/glew.h> //Init the GLEW library
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "DirectXTex.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ModuleRenderExercise::ModuleRenderExercise() : VAO(0), VBO(0), EBO(0), texture(0)
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
    LOG("Creating Triangle");

    //Load the Hello World shaders and create the program
    helloWorldProgram.Init("assets/shaders/vertex_shader.glsl", "assets/shaders/fragment_shader.glsl");

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    //TEXT:+++++++
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // Positions          // Colors           // Texture Coords
       -1.0f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Bottom Left
        1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top Right
       -1.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };

    unsigned int indices[] = { // note that we start from 0!
        0, 1, 2, // Triangle 1: V0-V1-V2
        2, 3, 0  // Triangle 2: V2-V3-V0
    };
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);//************************************************************************************************************
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // Must be called each frame for drawing the triangle

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //************************************************************************************************************
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // TEXT:+++++++
   
    // position attribute
    // size = 3 float per vertex
    // stride = sizeof(float)*3 is equivalent to stride = 0. Ej: glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture
    // -------------------------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    //Loads the texture
    App->GetTexture()->Init(L"assets/Baboon.dds", texture); //DDS file route
   
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //************************************************************************************************************
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //************************************************************************************************************

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    return true;
}

update_status ModuleRenderExercise::PreUpdate()
{

    return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    //TEXT:++++++
    
    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    //Activate the Shader program  
    model = App->GetCamera()->GetModel();
    view = App->GetCamera()->GetViewMatrix();
    projection = App->GetCamera()->GetProjectionMatrix();

    // draw our first triangle
    helloWorldProgram.Use(model, view, projection); //Render the triangle using the Hello World program

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 3); // 1 triangle to draw = 3 vertices
    
    //**********************************************************************************************************************
    
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //**********************************************************************************************************************

    // glBindVertexArray(0); // no need to unbind it every time 

    return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{

    return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp() {
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO); // Must be called one time at destruction of vertex buffer
    glDeleteBuffers(1, &EBO); //**************************************************************************************************


    //glDeleteTextures(GLsizei n, const GLuint * textures);


    //Delete program
    helloWorldProgram.CleanUp();

    return true;
}