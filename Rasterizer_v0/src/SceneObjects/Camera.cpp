#include <SceneObjects\Camera.h>
using namespace SceneObjects;


Camera::Camera(const Mat4& p_modelView) : m_modelView(p_modelView.Inverse())
{
}

Camera::~Camera()
{
}

void Camera::SetModelView(const Mat4& p_modelView)
{
	this->m_modelView = p_modelView.Inverse();
}

const Mat4& Camera::GetModelView() const
{

	return this->m_modelView;
}
