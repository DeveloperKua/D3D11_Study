#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd)
{
	_hWnd = hwnd;
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

Graphics::~Graphics()
{
}

void Graphics::RenderBegin()
{
	//�ĸ� ���� Draw
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewPort);
}

void Graphics::RenderEnd()
{
	//���� ���۷� Present
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = GWinSizeX;
		desc.BufferDesc.Height = GWinSizeY;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//�ȼ� ��� ������ �� ��Ƽ ���ø� ���� ������
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		//���� ��� �뵵
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		//�ĸ� ���� ��
		desc.BufferCount = 1;
		desc.OutputWindow = _hWnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	//https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
#pragma region Create Device Parameter
		nullptr, //����� �׷��� ����� ����, nullptr->�⺻ ����� ����

		//https://learn.microsoft.com/ko-kr/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
		D3D_DRIVER_TYPE_HARDWARE, //�۾��� ������ ����̹� Ÿ�� ����

		nullptr, //DriverType�� SOFTWARE�� ��� Software�� NULL X

		//https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag
		0,//��ġ ���� �� ����� �÷��׸� �����մϴ�. ���� ���, ����� ���� �����ϰų�, ���� ������ ���� ������ �� �ֽ��ϴ�.

		nullptr,//�����ϰ��� �ϴ� ��� ������ �迭�� �����մϴ�. ��ġ�� ������ �� �� �迭�� ������ ������� ��� ������ �õ��մϴ�.
		0, //(���� ����)pFeatureLevels �迭�� ��� ��

		D3D11_SDK_VERSION, //SDK ������ �����մϴ�.
#pragma endregion

#pragma region SwapChain Parameter
		& desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
#pragma endregion
	);

	//D3D11CreateDeviceAndSwapChain�� ���(HRESULT)�� �����ߴ��� Ȯ��
	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	//Swap Chaind���� backBuffer ȹ��
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	//RTV ����
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Graphics::SetViewport()
{
	_viewPort.TopLeftX = 0.f;
	_viewPort.TopLeftY = 0.f;
	_viewPort.Width = static_cast<float>(GWinSizeX);
	_viewPort.Height = static_cast<float>(GWinSizeY);
	_viewPort.MinDepth = 0.f;
	_viewPort.MaxDepth = 1.f;
}