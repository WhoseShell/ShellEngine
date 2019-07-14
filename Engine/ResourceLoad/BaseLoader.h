#pragma once

namespace DX 
{
	class BaseLoader
	{
	public:
		BaseLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);

	protected:
		Microsoft::WRL::ComPtr<ID3D11Device3> m_d3dDevice;

		Platform::Array<byte>^ ReadData(
			_In_ Platform::String^ filename
		);
	};


}