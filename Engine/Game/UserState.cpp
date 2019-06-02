#include "pch.h"
#include "UserState.h"
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace std;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI;
using namespace Windows::Foundation;
using namespace Windows::Devices::Input;
using namespace Windows::Gaming::Input;
using namespace Windows::System;
using namespace Engine;
UserState::UserState() :
	m_forward(false), 
	m_back(false), 
	m_left(false), 
	m_right(false)
{}

void UserState::OnKeyDown(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if (Key == VirtualKey::W)
		m_forward = true;
	if (Key == VirtualKey::S)
		m_back = true;
	if (Key == VirtualKey::A)
		m_left = true;
	if (Key == VirtualKey::D)
		m_right = true;
}

void UserState::OnKeyUp(_In_ Windows::UI::Core::CoreWindow^ sender, _In_ Windows::UI::Core::KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// Figure out the command from the keyboard.
	if (Key == VirtualKey::W)
		m_forward = false;
	if (Key == VirtualKey::S)
		m_back = false;
	if (Key == VirtualKey::A)
		m_left = false;
	if (Key == VirtualKey::D)
		m_right = false;
}

bool UserState::getForward()
{
	return m_forward;
}

bool UserState::getBack()
{
	return m_back;
}

bool UserState::getLeft()
{
	return m_left;
}

bool UserState::getRight()
{
	return m_right;
}
