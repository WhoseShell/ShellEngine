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
	XMVECTOR forwardDir = (XMVector3Normalize(XMLoadFloat3(&lookPosition) - XMLoadFloat3(&eyePosition))); //������ǰ����λ����F
	XMStoreFloat3(&lookPosition, XMLoadFloat3(&eyePosition) + forwardDir);//��������Ŀ�������

	float xChange = userState->current_touch.x * rotateRate; //�������X����Ա仯ֵ
	float yChange = userState->current_touch.y * rotateRate; //��������Y����Ա仯ֵ
	
	if (xChange != 0 || yChange != 0) //�����������ƶ�
	{
		forwardDir = XMVector3Transform(forwardDir, (DirectX::XMMatrixRotationY(-xChange))); //������ҽ�����F����Y��������ת
		forwardDir = XMVector3Transform(forwardDir, (DirectX::XMMatrixRotationX(yChange))); //��������ƶ�������F����X��������ת 

		forwardDir = XMVector3Normalize(forwardDir); //��һ����ת�������F
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&eyePosition) + forwardDir);//��������Ŀ�������

		userState->current_touch.x = userState->current_touch.y = 0; //����Ըı�������
	}
	if (userState->getForward())
	{
		/*XMFLOAT3 forwardDir;
		XMStoreFloat3(&forwardDir, XMVector3Normalize(XMLoadFloat3(&lookPosition) - XMLoadFloat3(&eyePosition)));*/
		
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) + forwardDir * moveRate);//�ӵ�����Fǰ��
		XMStoreFloat3(&eyePosition , XMLoadFloat3(&eyePosition) + forwardDir * moveRate);//Ŀ�������Fǰ��

	};
	if (userState->getBack())
	{
		
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) - forwardDir * moveRate);//�ӵ�����F����
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) - forwardDir * moveRate);//Ŀ�������F����

		
	};
	if (userState->getLeft())
	{
		XMVECTOR yAxis = { 0.0f, 1.0f, 0.0f };
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) - XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);//���㴹ֱ��F��Y����������ӵ������������ƶ�
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) - XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);//���㴹ֱ��F��Y���������Ŀ��������������ƶ�

		
	};
	if (userState->getRight())
	{
		XMVECTOR yAxis = { 0.0f, 1.0f, 0.0f };
		XMStoreFloat3(&lookPosition, XMLoadFloat3(&lookPosition) + XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);//���㴹ֱ��F��Y����������ӵ����Ÿ������ƶ�
		XMStoreFloat3(&eyePosition, XMLoadFloat3(&eyePosition) + XMVector3Normalize(XMVector3Cross(forwardDir, yAxis)) * moveRate);//���㴹ֱ��F��Y���������Ŀ������Ÿ������ƶ�
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
	// question�������matrix��vs�׶ε�ת��
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
