#pragma once
#include "MCP/Maths/mat4.h"
#include "MCP/Event/Event.h"
namespace MC
{
	//@TODO: Switch mode para alternar entre a cria��o de uma c�mera perspectiva ou ortogr�fica.
	//@TODO: Mudar para quarternions
	//@TODO: Um modo para LookAt!
	
	/*    Very Primitive Camera */

	class Camera
	{
	public:
		Camera(const float AR, const vec3& position);
		Camera() = default;

		const mat4 getViewMatrix() const;


		void UpdateCameraVectors();

		void AddCameraTargetPosition(vec3 pos);

		inline void SetCameraTarget(vec3& target) { m_CameraTarget = target; } 
		void SetCameraPosition(vec3& position);
		inline void SetCameraYaw(float Yaw) { UpdateCameraVectors(); m_Yaw = Yaw; }
		inline void SetCameraPitch(float Pitch) { UpdateCameraVectors(); m_Pitch = Pitch; }
	
		inline vec3 GetCameraTarget() const { return m_CameraTarget; }
		inline vec3 GetCameraUp() const { return m_CameraUp; }
		inline vec3 GetCameraRight() const { return m_CameraRight; }
			 
		inline float GetCameraYaw() const { return m_Yaw; }
		inline float GetCameraPitch() const { return m_Pitch; }

		inline mat4 GetProjection() const { return m_Projection; }
		inline void SetProjection(float AR);

		inline void SetViewMatrix(mat4& view) { m_ViewMatrix = view; }

		void OnEvent(Event& e);

	private:
		vec3 m_CameraTarget;
		vec3 m_CameraUp;
		vec3 m_CameraRight;

	private:
		float m_Yaw, m_Pitch;

	private:
		mat4 m_CameraPosition, m_CameraRotation;
		mat4 m_ViewMatrix, m_Projection;
	};
}