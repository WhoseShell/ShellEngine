#include "pch.h"
#include "MoveController.h"

using namespace Engine;
using namespace std;
using namespace Windows::Foundation;
using namespace DirectX;

void Engine::MoveController::MoveCamera(XMFLOAT4X4 &view, UserState ^ userState, XMVECTORF32 &focus)
{
	if (userState->getForward())
	{
		
	};
	if (userState->getBack())
	{

	};
	if (userState->getLeft())
	{

	};
	if (userState->getRight())
	{

	};
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
