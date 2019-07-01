#include"pch.h"
#include"Object.h"

XMFLOAT4X4 Engine::Object::GetTransform4x4()
{
	XMFLOAT4X4	out;
	auto tansform4x4 =
		XMMatrixScaling(transform->scale.x, transform->scale.y, transform->scale.z) *
		XMMatrixRotationRollPitchYaw(transform->rotation.x, transform->rotation.y, transform->rotation.z) *
		XMMatrixTranslation(transform->location.x, transform->location.y, transform->location.z);
	XMStoreFloat4x4(&out, tansform4x4);
	return out;
}

void Engine::Object::SetLocation(FLOAT x, FLOAT y, FLOAT z)
{
	if (transform == nullptr)
	{
		return;
	}

	transform->location.x = x;
	transform->location.y = y;
	transform->location.z = z;
}

void Engine::Object::SetRotation(FLOAT x, FLOAT y, FLOAT z)
{
	if (transform == nullptr)
	{
		return;
	}

	transform->rotation.x = x;
	transform->rotation.y = y;
	transform->rotation.z = z;
}

void Engine::Object::SetScale(FLOAT x, FLOAT y, FLOAT z)
{
	if (transform == nullptr)
	{
		return;
	}

	transform->scale.x = x;
	transform->scale.y = y;
	transform->scale.z = z;
}
