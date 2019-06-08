#include "pch.h"
#include "MoveController.h"

using namespace Engine;
using namespace std;
using namespace Windows::Foundation;
using namespace DirectX;

Engine::MoveController::MoveController():moveRate(0.1f)
{
}

void Engine::MoveController::MoveCamera(XMFLOAT4X4 &view, UserState ^ userState, XMFLOAT3 &eyePosition, XMFLOAT3 &lookPosition, XMFLOAT3 &up)
{
	XMVECTOR forwardDir = (XMVector3Normalize(XMLoadFloat3(&lookPosition) - XMLoadFloat3(&eyePosition)));

	if (userState->getForward())
	{
		/*XMFLOAT3 forwardDir;
		XMStoreFloat3(&forwardDir, XMVector3Normalize(XMLoadFloat3(&lookPosition) - XMLoadFloat3(&eyePosition)));*/
		
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) + forwardDir * moveRate);
		XMStoreFloat3(&eyePosition , XMLoadFloat3(&eyePosition) + forwardDir * moveRate);

	};
	if (userState->getBack())
	{
		
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) - forwardDir * moveRate);
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) - forwardDir * moveRate);

		
	};
	if (userState->getLeft())
	{
		XMVECTOR yAxis = { 0.0f, 1.0f, 0.0f };
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) - XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) - XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);

		
	};
	if (userState->getRight())
	{
		XMVECTOR yAxis = { 0.0f, 1.0f, 0.0f };
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) + XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) + XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);
	};

	XMStoreFloat4x4(&view, XMMatrixLookAtRH(XMLoadFloat3(&eyePosition), XMLoadFloat3(&lookPosition), XMLoadFloat3(&up)));
}

void Engine::MoveController::MoveCamera(XMFLOAT4X4 &view, UserState ^ userState)
{
	// question：这里的matrix是vs阶段的转置
	if (userState->getForward())
	{
		view._43 += 0.1f;
	};
	if (userState->getBack())
	{
		view._43 -= 0.1f;
	};
	if (userState->getLeft())
	{
		view._41 += 0.1f;
	};
	if (userState->getRight())
	{
		view._41 -= 0.1f;
	};
}
