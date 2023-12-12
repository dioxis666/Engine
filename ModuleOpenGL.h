#pragma once
#include "Module.h"
#include "ModuleRenderExercise.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleOpenGL : public Module
{
public:
	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void* GetContext() const; // Function to get the OpenGL context.

	void WindowResized(unsigned width, unsigned height);

private:
	/* En C++, una variable de tipo void* se utiliza para almacenar direcciones de memoria sin especificar el tipo de dato al que apunta. 
	Esto puede ser útil en situaciones en las que se necesita manejar datos de manera genérica o cuando se trabaja con punteros a datos cuyo tipo es desconocido o puede variar dinámicamente. */
	void* context;





	//ModuleRenderExercise VBO_triangle;
};
