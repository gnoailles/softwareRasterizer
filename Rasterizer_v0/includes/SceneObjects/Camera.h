#pragma once
#include "Maths/Mat4.h"

using namespace Maths;

namespace SceneObjects
{
	class Camera
	{
	private:
		Mat4 m_view;
		Mat4 m_projection;
		float m_top;
		float m_right;
		float m_near;
		float m_far;
	public:
		Camera(const int& p_width, const int& p_height, const float& p_near,
			const float& p_far, const float& p_fov,
			const Mat4& p_modelView = Mat4::CreateTranslationMatrix(Vec3(0, 0, 0)));
		~Camera();

		void SetViewMatrix(const Mat4& p_modelView);
		void SetPosition(const Vec3& p_pos);

		const Mat4& GetPerspectiveProjectionMatrix() const;
		const Mat4& GetViewMatrix() const;
		Vec3 GetPosition() const;

		void UseOrthographic();
		void UsePerspective();

		const float& Top() const;
		const float& Right() const;
		const float& Near() const;
		const float& Far() const;
	};
}
