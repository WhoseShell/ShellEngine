#pragma once
#include "Common\DirectXHelper.h"

using namespace DirectX;
namespace Engine
{
	ref class UserState
	{
	internal:
		void OnKeyDown(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::KeyEventArgs^ args
		);

		void OnKeyUp(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::KeyEventArgs^ args
		);

		void PointerMoved(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::PointerEventArgs^ args
		);

		void OnMouseMoved(
			_In_ Windows::Devices::Input::MouseDevice^ mouseDevice,
			_In_ Windows::Devices::Input::MouseEventArgs^ args
		);

		bool getForward();
		bool getBack();
		bool getLeft();
		bool getRight();
		bool getUp();
		bool getDown();

		XMFLOAT2					last_touch;
		XMFLOAT2					current_touch;


		UserState();

	private:
		bool                        m_forward;
		bool                        m_back;
		bool                        m_left;
		bool                        m_right;
		bool                        m_up;
		bool                        m_down;
	};
}
