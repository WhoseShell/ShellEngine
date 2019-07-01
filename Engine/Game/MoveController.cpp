#include "pch.h"
#include "MoveController.h"

using namespace Engine;
using namespace std;
using namespace Windows::Foundation;
using namespace DirectX;

Engine::MoveController::MoveController():moveRate(0.1f), rotateRate(0.02f)
{
}

void Engine::MoveController::MoveCamera(XMFLOAT4X4 &view, UserState ^ userState, XMFLOAT3 &eyePosition, XMFLOAT3 &lookPosition, XMFLOAT3 &up)
{
	XMVECTOR forwardDir = (XMVector3Normalize(XMLoadFloat3(&lookPosition) - XMLoadFloat3(&eyePosition))); //计算正前方单位向量F
	XMStoreFloat3(&lookPosition, XMLoadFloat3(&eyePosition) + forwardDir);//重新设置目标点坐标

	float xChange = userState->current_touch.x * rotateRate; //获得鼠标的X轴相对变化值
	float yChange = userState->current_touch.y * rotateRate; //或者鼠标的Y轴相对变化值
	
	if (xChange != 0 || yChange != 0) //如果存在鼠标移动
	{
		forwardDir = XMVector3Transform(forwardDir, (DirectX::XMMatrixRotationY(-xChange))); //鼠标左右将向量F沿着Y坐标轴旋转
		forwardDir = XMVector3Transform(forwardDir, (DirectX::XMMatrixRotationX(yChange))); //鼠标上下移动将向量F沿着X坐标轴旋转 

		forwardDir = XMVector3Normalize(forwardDir); //归一化旋转后的向量F
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&eyePosition) + forwardDir);//重新设置目标点坐标

		userState->current_touch.x = userState->current_touch.y = 0; //将相对改变量归零
	}
	if (userState->getForward())
	{
		/*XMFLOAT3 forwardDir;
		XMStoreFloat3(&forwardDir, XMVector3Normalize(XMLoadFloat3(&lookPosition) - XMLoadFloat3(&eyePosition)));*/
		
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) + forwardDir * moveRate);//视点沿着F前进
		XMStoreFloat3(&eyePosition , XMLoadFloat3(&eyePosition) + forwardDir * moveRate);//目标点沿着F前进

	};
	if (userState->getBack())
	{
		
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) - forwardDir * moveRate);//视点沿着F后退
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) - forwardDir * moveRate);//目标点沿着F后退

		
	};
	if (userState->getLeft())
	{
		XMVECTOR yAxis = { 0.0f, 1.0f, 0.0f };
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) - XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);//计算垂直于F和Y轴的向量，视点沿着正方向移动
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) - XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);//计算垂直于F和Y轴的向量，目标点沿着正方向移动

		
	};
	if (userState->getRight())
	{
		XMVECTOR yAxis = { 0.0f, 1.0f, 0.0f };
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) + XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);//计算垂直于F和Y轴的向量，视点沿着负方向移动
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) + XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);//计算垂直于F和Y轴的向量，目标点沿着负方向移动
	};
	if (userState->getUp())
	{
		eyePosition.y += moveRate;
		lookPosition.y += moveRate;
	};
	if (userState->getDown())
	{
		eyePosition.y -= moveRate;
		lookPosition.y -= moveRate;
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
