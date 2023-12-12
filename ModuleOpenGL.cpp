#include "ModuleOpenGL.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include <GL/glew.h> //Init the GLEW library

ModuleOpenGL::ModuleOpenGL() : context(nullptr)
{
}

// Destructor
ModuleOpenGL::~ModuleOpenGL()
{
}

//***********************************************************************************************************************
// OpenGL Error Function
/*
static void APIENTRY openglCallbackFunction(GLenum source, GLenum type,	GLuint id, GLenum severity,	GLsizei length,	const GLchar* message, const void* userParam) 
{
	(void)source; (void)type; (void)id;
	(void)severity; (void)length; (void)userParam;
	fprintf(stderr, "%s\n", message);
	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		fprintf(stderr, "Aborting...\n");
		abort();
	}
}
*/

void __stdcall openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
	switch (source) {
	case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
	};
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
	};
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};
	LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
}
//***********************************************************************************************************************

// Called before render is available
bool ModuleOpenGL::Init()
{
	LOG("Creating Renderer context");

	//OpenGL Initialization: 

	//Setup attributes with SDL_GL_SetAttribute():
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	//Init once with : SDL_GL_CreateContext():
	
	/*typedef void* SDL_GL_Context; se utiliza para definir un alias de tipo que hace que el código sea más claro al indicar que context es un puntero genérico relacionado 
	con el contexto de OpenGL, en lugar de simplemente un puntero genérico como void*. Usar typedef de esta manera no cambia el tipo de dato subyacente (todavía es un puntero void*), 
	pero le da un nombre más descriptivo (SDL_GL_Context) que facilita la comprensión del código. */
	typedef void* SDL_GL_Context;
	context = SDL_GL_CreateContext(App->GetWindow()->window); //SDL_GLContext SDL_GL_CreateContext(SDL_Window * window);

	//After context creation, init the GLEW library:
	GLenum err = glewInit(); // … check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION)); // Should be 2.0

	//To detect our current hardware and driver capabilities we use glGetString():
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Initialize some OpenGL global states(check documentation):
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	// Enable the Open GL debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);

	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	//For clearing the screen:
	int window_width, window_height;
	SDL_GetWindowSize(App->GetWindow()->window, &window_width, &window_height);

	//Setup glViewport to 0, 0, window_width, window_height if window is resized:
	glViewport(0, 0, window_width, window_height);

	//void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	//Swap frame buffer on ModuleOpenGL:
	SDL_GL_SwapWindow(App->GetWindow()->window); //Change window from back buffer to front buffer

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(context); //Remove context:






	//***********************************************************************************************************************
	//CleanUp Triangle on ModuleRenderExercise
	//VBO_triangle.CleanUp();
	//***********************************************************************************************************************

	return true;
}


void* ModuleOpenGL::GetContext() const
{
	return context; // Return the OpenGL Context
}

void ModuleOpenGL::WindowResized(unsigned width, unsigned height)
{
	//LOG("RESIZED WINDOW");
	App->GetWindow()->SetAspectRatio(width, height);
}




