#pragma once
#include "Module.h"
#include <MathGeoLib.h>

class ModuleCamera : public Module
{
	public:
        ModuleCamera();
        ~ModuleCamera();

        bool Init();
        update_status PreUpdate();
        update_status Update();
        update_status PostUpdate();
   
        void SetFOV();
        void SetAspectRatio();
        void SetPlaneDistances(float& nearDistance, float& farDistance);
        void InitFrustum();
        float4x4 GetModel();
        float4x4 GetViewMatrix();
        float4x4 GetProjectionMatrix();

        //float4x4 LookAt(x, y, z); 
        float4x4 LookAt(const float3& camera_position, const float3& target_position, const float3& up_vector);
        float4x4 GetViewMatrix2();
        float4x4 GetProjectionMatrix2();

        void MoveForwards(float speed);
        void MoveSideways(float speed);
        void MoveUpwards(float speed);
        void RotateYaw(float angle);
        void RotatePitch(float angle);

        void WindowResize();

        //void Rotate(float pitch, float yaw);
        void Rotate();

	private:        
        Frustum frustum;
        //float3x3 rotationDeltaMatrix; //Se usa en Rotate (apuntes)
};

