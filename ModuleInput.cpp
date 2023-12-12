#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "SDL/include/SDL.h"
#include "imgui_impl_sdl2.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;

	mouse_motion_x = 0;
	mouse_motion_y = 0;
	mouse_scroll = 0;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        //Send input events to ImGui calling:
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->GetOpenGL()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
						
			case SDL_MOUSEMOTION:
				mouse_motion_x = sdlEvent.motion.xrel;
				mouse_motion_y = sdlEvent.motion.yrel;
				break;

			case SDL_MOUSEBUTTONDOWN:
					mouse_buttons[sdlEvent.button.button - 1] = true;
				break;

			case SDL_MOUSEBUTTONUP:
					mouse_buttons[sdlEvent.button.button - 1] = false;
				break;

			case SDL_MOUSEWHEEL:
				mouse_scroll = sdlEvent.wheel.y;
				break;
        }
    }

    keyboard = SDL_GetKeyboardState(nullptr);

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
