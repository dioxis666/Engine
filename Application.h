#pragma once
#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleWindow;
class ModuleOpenGL;
class ModuleTexture;
class ModuleInput;
class ModuleRenderExercise;
class ModuleCamera;
class ModuleEditor;
class ModuleDebugDraw;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleWindow* GetWindow() { return window; }
    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleTexture* GetTexture() { return texture; }
    ModuleInput*  GetInput() { return input; }
    ModuleRenderExercise* GetRenderExercise() { return render_exercise; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleEditor* GetEditor() { return editor; }
    ModuleDebugDraw* GetDebugDraw() { return debug_draw; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleTexture* texture = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleRenderExercise* render_exercise = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleDebugDraw* debug_draw = nullptr;

    std::list<Module*> modules;
};

extern Application* App;
