#pragma once
#include "Module.h"
#include "ModuleWindow.h"

class ModuleEditor : public Module
{
    public:
        ModuleEditor();
        ~ModuleEditor();

        bool Init();
        update_status PreUpdate();
        update_status Update();
        update_status PostUpdate();
        bool CleanUp();

        static void ShowMainMenuBar();
        static void ShowLogWindow();
    
    private:
        void MovementController();

    private:
        const char* glsl_version = "#version 460";
        static bool show_log_window;
        float speed;

        SDL_Window* window = nullptr;
        SDL_GLContext render_context = nullptr;
};