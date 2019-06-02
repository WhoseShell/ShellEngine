#pragma once
#include "UserState.h"
#include "Pass\ShaderStruct.h"
#include "Common\DirectXHelper.h"

namespace Engine
{
	class MoveController
	{
	public:
		void MoveCamera(DirectX::XMFLOAT4X4 &view, UserState^ userState, DirectX::XMVECTORF32 &focus);
		void MoveCamera(DirectX::XMFLOAT4X4 &view, UserState^ userState);
	};
}