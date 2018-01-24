#pragma once
#include "Maths/Mat4.h"

using namespace Maths;

namespace SceneObjects
{
	class Camera
	{
	private:
		Mat4 m_modelView;
	public:
		Camera(const Mat4& p_modelView = Mat4::CreateTranslationMatrix(Vec3(0,0,0)));
		~Camera();

		void SetModelView(const Mat4& p_modelView);
		const Mat4& GetModelView() const;
	};
}
