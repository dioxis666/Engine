#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleOpenGL.h"
//#include "imgui.h" //Used for UI interface
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <shellapi.h> //Used to open Web browser
#include "ModuleCamera.h"
#include <ModuleDebugDraw.h> //Used for axisTriad and xzSquareGrid
#include "debugdraw.h"
#include "ModuleInput.h"

bool ModuleEditor::show_log_window = false;

ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    LOG("Creating ImGui Context");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();  

    window = App->GetWindow()->window;
    render_context = App->GetOpenGL()->GetContext();

    SDL_GL_MakeCurrent(window, render_context); // ESTO ES NECESARIO? SI LO QUITAS FUNCIONA!

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, render_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

update_status ModuleEditor::PreUpdate()
{
    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
    //******************************************************
        ImGuiIO& io = ImGui::GetIO(); (void)io;
    //******************************************************
    // 
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    //Create Menu bar for the editor
    ShowMainMenuBar();

    //Windows:
    if (show_log_window) ShowLogWindow();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //Call axis function
    dd::axisTriad(float4x4::identity, 0.1f, 1.0f);

    //Call the grid function
    dd::xzSquareGrid(-10, 10, 0.0f, 1.0f, dd::colors::LimeGreen);

    // Call Draw method from ModuleDebugDraw
    float4x4 view = App->GetCamera()->GetViewMatrix();
    float4x4 projection = App->GetCamera()->GetProjectionMatrix();
    
    int width = App->GetWindow()->GetWidth();
    int height = App->GetWindow()->GetHeight();

    // Call Draw method passing view, projection, screen width and height as arguments
    App->GetDebugDraw()->Draw(view, projection, width, height);

    //Camera control with keyboard and mouse
    MovementController();

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{

    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::ShowMainMenuBar() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Separator();
            if (ImGui::MenuItem("Quit"))
            { 
                exit(0); 
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            if (ImGui::MenuItem("Console", "CTRL+C")) {
                if (show_log_window == false) show_log_window = true;
                else show_log_window = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("GitHub page")) 
            {   
                // Open Url
                const char* url = "https://github.com/dioxis666";
                ShellExecute(0, 0, url, 0, 0, SW_SHOW);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ModuleEditor::ShowLogWindow() {
    string logBuffer = GetLogBuffer(); //Get the log content
    const char* logChar = logBuffer.c_str(); //Convert log string to char

    ImGui::TextUnformatted(logChar); //Gets log.cpp char result of all LOG messages. 
}

void ModuleEditor::MovementController() {
    //Pressing mouse right button...
    if (App->GetInput()->GetMouseButton(SDL_BUTTON_RIGHT)) {
        //Holding SHIFT duplicates movement speed.
        if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT)) {
            speed = 1.0f * 2;
        }
        else {
            //Keyboard speed:
            speed = 1.0f;
        }

        //WASD Movement:
        if (App->GetInput()->GetKey(SDL_SCANCODE_W)) {
            App->GetCamera()->MoveForwards(speed);
        }

        if (App->GetInput()->GetKey(SDL_SCANCODE_S)) {
            App->GetCamera()->MoveForwards(-speed);
        }

        if (App->GetInput()->GetKey(SDL_SCANCODE_A)) {
            App->GetCamera()->MoveSideways(-speed); // Move left with Deta Time
        }
        if (App->GetInput()->GetKey(SDL_SCANCODE_D)) {
            App->GetCamera()->MoveSideways(speed); // Move right with Deta Time
        }

        //QE Movement:
        if (App->GetInput()->GetKey(SDL_SCANCODE_Q)) {
            App->GetCamera()->MoveUpwards(-speed); // Move down with Deta Time
        }
        if (App->GetInput()->GetKey(SDL_SCANCODE_E)) {
            App->GetCamera()->MoveUpwards(speed); // Move up with Deta Time
        }

        //Mouse rotations:
        if (App->GetInput()->GetMouseDeltaX() != 0) {
            float deltaMouseX = App->GetInput()->GetMouseDeltaX(); //Mouse motion X

            App->GetCamera()->RotateYaw(-deltaMouseX); // Rotate horizontally with mouse
        }

        if (App->GetInput()->GetMouseDeltaY() != 0) {
            float deltaMouseY = App->GetInput()->GetMouseDeltaY(); //Mouse motion Y

            App->GetCamera()->RotatePitch(-deltaMouseY); // Rotate vertically with mouse
        }
    }

    //Keyboard rotations:
    float rotationSpeed = 0.1f;

    if (App->GetInput()->GetKey(SDL_SCANCODE_LEFT)) {
        App->GetCamera()->RotateYaw(rotationSpeed); // Rotate right with Delta Time
    }

    if (App->GetInput()->GetKey(SDL_SCANCODE_RIGHT)) {
        App->GetCamera()->RotateYaw(-rotationSpeed); // Rotate left with Delta Time
    }

    if (App->GetInput()->GetKey(SDL_SCANCODE_UP)) {
        App->GetCamera()->RotatePitch(rotationSpeed); // Rotate down with Delta Time
    }

    if (App->GetInput()->GetKey(SDL_SCANCODE_DOWN)) {
        App->GetCamera()->RotatePitch(-rotationSpeed); // Rotate up with Delta Time
    }


    // HACER ESTO MODIFICANDO EL FOV???


    //Mouse scrolling:
    int mouse_scroll = App->GetInput()->GetMouseScroll();
    float scroll_speed = 500.0f;

    if (mouse_scroll > 0) {
        App->GetCamera()->MoveForwards(scroll_speed);
    }
    else if (mouse_scroll < 0) {
        App->GetCamera()->MoveForwards(-scroll_speed);
    }

    /*
    if (App->GetInput()->GetMouseButton(SDL_BUTTON_LEFT)) {
        LOG("MOUSE LEFT");
    }
    */
}

