#pragma once
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

		bool getForward();
		bool getBack();
		bool getLeft();
		bool getRight();

		UserState();

	private:
		bool                        m_forward;
		bool                        m_back;
		bool                        m_left;
		bool                        m_right;
	};
}
