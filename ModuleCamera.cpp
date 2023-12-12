#include "ModuleCamera.h"
#include <GL/glew.h> //Init the GLEW library
#include "Application.h"
#include "ModuleWindow.h"

ModuleCamera::ModuleCamera() { }
ModuleCamera::~ModuleCamera() {}

bool ModuleCamera::Init()
{
    LOG("Init Camera");

	//Call the frustum function
	InitFrustum();

    return true;
}

update_status ModuleCamera::PreUpdate()
{
    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
    return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
    return UPDATE_CONTINUE;
}

//SetFOV() … should set the horizontal FOV keeping the aspect ratio
void ModuleCamera::SetFOV() {

}

//SetAspectRatio() … should change the vertical FOV to meet the new aspect ratio.
void ModuleCamera::SetAspectRatio() {

}

void ModuleCamera::SetPlaneDistances(float& nearDistance, float& farDistance) {
	frustum.nearPlaneDistance = nearDistance;
	frustum.farPlaneDistance = farDistance;
}

void ModuleCamera::InitFrustum()
{
	frustum.type = FrustumType::PerspectiveFrustum;
		
	frustum.pos = { 0.0f, 2.0f, 9.0f }; //X,Y,Z

	//frustum.pos = float3::zero;

	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;

	//Get the window's aspect ratio  
	float aspect_ratio = App->GetWindow()->GetAspectRatio();
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

float4x4 ModuleCamera::GetModel()
{
	//************************
	// Original code with 45 degree Z Rotation
	//float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 1.0f));
	//************************

	//This code have no 45 degree rotation and float3(1.0f, 1.0f, 1.0f)) to avoid rotation and deformation problems with the EBO Square
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(0.0f), float3(1.0f, 1.0f, 1.0f));

	return model;
}

float4x4 ModuleCamera::GetViewMatrix()
{
	float4x4 view = frustum.ViewMatrix();
	
	//float4x4 view = LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);

	return view;
}

float4x4 ModuleCamera::GetProjectionMatrix()
{
	float4x4 proj = frustum.ProjectionMatrix();

	return proj;
}

float4x4 ModuleCamera::LookAt(const float3& camera_position, const float3& target_position, const float3& up_vector)
{
	float3 forward = (target_position - camera_position).Normalized();
	float3 right = Cross(forward, up_vector).Normalized();
	float3 up = Cross(right, forward).Normalized();

	float4x4 viewMatrix = {
		{right.x, up.x, -forward.x, 0.0f},
		{right.y, up.y, -forward.y, 0.0f},
		{right.z, up.z, -forward.z, 0.0f},
		{-right.Dot(camera_position), -up.Dot(camera_position), forward.Dot(camera_position), 1.0f}
	};

	return viewMatrix;
}

void ModuleCamera::MoveForwards(float speed) {
	float deltaTime = 0.005f; //Variable for testing

	// Obtener la dirección hacia la que se moverá el frustum (en este caso, el vector front del frustum)
	float3 direction = frustum.front.Normalized();

	// Calcular el desplazamiento basado en la dirección, la velocidad proporcionada y el tiempo transcurrido (delta time)
	float3 displacement = direction * speed * deltaTime;

	// Actualizar la posición del frustum sumando el desplazamiento calculado
	frustum.pos += displacement;
}

void ModuleCamera::MoveSideways(float speed) {
	float deltaTime = 0.005f; //Variable for testing

	// Obtener la dirección lateral (usando el producto cruz entre el frente y arriba para obtener la derecha)
	float3 right = Cross(frustum.front, frustum.up).Normalized();

	// Calcular el desplazamiento lateral basado en la dirección, la velocidad y el tiempo transcurrido (delta time)
	float3 displacement = right * speed * deltaTime;

	// Actualizar la posición del frustum sumando el desplazamiento calculado
	frustum.pos += displacement;
}

void ModuleCamera::MoveUpwards(float speed) {
	float deltaTime = 0.005f; //Variable for testing

	// Obtener la dirección hacia arriba (usando el vector de arriba del frustum)
	float3 up = frustum.up.Normalized();

	// Calcular el desplazamiento vertical basado en la dirección, la velocidad y el tiempo transcurrido (delta time)
	float3 displacement = up * speed * deltaTime;

	// Actualizar la posición del frustum sumando el desplazamiento calculado
	frustum.pos += displacement;
}

void ModuleCamera::RotateYaw(float angle) {
	float deltaTime = 0.005f;
	angle *= deltaTime;

	// Obtener la matriz de rotación en el eje vertical (Y)
	float4x4 rotationMatrix = float4x4::RotateY(angle);

	// Aplicar la rotación al frente y a la derecha del frustum
	frustum.front = rotationMatrix.MulDir(frustum.front).Normalized();
	frustum.up = rotationMatrix.MulDir(frustum.up).Normalized();
}

void ModuleCamera::RotatePitch(float angle) {
	float deltaTime = 0.005f;
	angle *= deltaTime;

	// Obtener la eje horizontal (derecha) del frustum
	float3 right = Cross(frustum.front, frustum.up).Normalized();

	// Obtener la matriz de rotación en el eje horizontal (Pitch)
	float4x4 rotationMatrix = float4x4::RotateAxisAngle(right, angle);

	// Aplicar la rotación al frente y al vector de arriba del frustum
	frustum.front = rotationMatrix.MulDir(frustum.front).Normalized();
	frustum.up = rotationMatrix.MulDir(frustum.up).Normalized();
}

//Detect window resize and trigger FOV recalculation accordingly
void ModuleCamera::WindowResize() {
	/*En ModuleIOpenGL existe esta función : void ModuleOpenGL::WindowResized(unsigned width, unsigned height) y en el ModuleInput 
	se hace referencia a ella. */
}

void ModuleCamera::Rotate()
{
	// Debemos extraer una matriz de rotación de 3x3 de nuestra matriz modelo 4x4 y aplicar la transformación.
	float3x3 rotationDeltaMatrix; // = some rotation delta value

	float3 oldFront = frustum.front.Normalized(); // Utilizar float3 en lugar de vec
	frustum.front = rotationDeltaMatrix * oldFront; // Realizar la multiplicación directa en lugar de usar MultDir

	float3 oldUp = frustum.up.Normalized(); // Utilizar float3 en lugar de vec
	frustum.up = rotationDeltaMatrix * oldUp; // Realizar la multiplicación directa en lugar de usar MultDir
}