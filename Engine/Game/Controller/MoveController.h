#pragma once
#include "Game\State\UserState.h"
#include "RenderPipeline\Pass\ShaderStruct.h"
#include "Common\DirectXHelper.h"

using namespace DirectX;
namespace Engine
{
	class MoveController
	{
	public:
		MoveController();

		float moveRate;
		float rotateRate;
		void MoveCamera(
			XMFLOAT4X4 &view, 
			UserState^ userState, 
			XMFLOAT3 &eyePosition,
			XMFLOAT3 &lookPosition,
			XMFLOAT3 &up
		);
		void MoveCamera(XMFLOAT4X4 &view, UserState^ userState);
		
	};
}