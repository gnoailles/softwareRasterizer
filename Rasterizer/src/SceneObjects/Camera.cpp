#include <SceneObjects\Camera.h>
#include "Rendering/Rasterizer.h"
using namespace SceneObjects;

Camera::Camera(const int& p_width, const int& p_height, const float& p_near, const float& p_far, const float& p_fov,
               const Mat4& p_modelView) : m_view(p_modelView), m_near(p_near), m_far(p_far)
{
	this->m_top = tanf(p_fov * static_cast<float>(M_PI) / 360.f) * this->m_near;
	this->m_right = this->m_top * (p_width / (float)p_height);
	this->m_projection = Rendering::Rasterizer::CreatePerspectiveProjectionMatrix(
		-this->m_right, this->m_right, -this->m_top,
		this->m_top, this->m_near, this->m_far);	
}

Camera::~Camera()
{
}

const Mat4& Camera::GetPerspectiveProjectionMatrix() const
{
	return this->m_projection;
}

void Camera::SetViewMatrix(const Mat4& p_modelView)
{
	this->m_view = p_modelView.Inverse();
}

void Camera::SetPosition(const Vec3& p_pos)
{
	this->m_view = Mat4::CreateTranslationMatrix(p_pos).Inverse();
}

const Mat4& Camera::GetViewMatrix() const
{
	return this->m_view;
}

Vec3 Camera::GetPosition() const
{
	return (this->m_view.Inverse() * Vec4(0, 0, 0, 1)).ToVec3();
}

void Camera::UseOrthographic()
{
	this->m_projection = Rendering::Rasterizer::CreateOrthographicProjectionMatrix(
		-this->m_right, this->m_right, -this->m_top,
		this->m_top, this->m_near, this->m_far);
}

void Camera::UsePerspective()
{
	this->m_projection = Rendering::Rasterizer::CreatePerspectiveProjectionMatrix(
		-this->m_right, this->m_right, -this->m_top,
		this->m_top, this->m_near, this->m_far);
}

const float& Camera::Top() const
{
	return m_top;
}

const float& Camera::Right() const
{
	return m_right;
}

const float& Camera::Near() const
{
	return m_near;
}

const float& Camera::Far() const
{
	return m_far;
}
