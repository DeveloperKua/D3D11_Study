#include "pch.h"
#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init(HWND hwnd)
{
	_hWnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	//TODO
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewPort();
}

void Game::Update()
{

}

void Game::Render()
{

}

void Game::RenderBegin()
{
}

void Game::RenderEnd()
{
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//픽셀 계단 현상일 떄 멀티 샘플링 관련 설정값
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		//버퍼 사용 용도
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		
		//후면 버퍼 수 
		desc.BufferCount =1;
		desc.OutputWindow = _hWnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

#pragma region COMPtr 참조 방식
	//_device.Get();
	//*device라면 Get의 경우 device

	//*device라면 GetAddressOf의 경우 &device
	//_device.GetAddressOf();

#pragma endregion

	//https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
#pragma region Create Device Parameter
		nullptr, //사용할 그래픽 어댑터 지정, nullptr->기본 어댑터 지정

		//https://learn.microsoft.com/ko-kr/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
		D3D_DRIVER_TYPE_HARDWARE, //작업을 수행할 드라이버 타입 지정


		nullptr, //DriverType이 SOFTWARE인 경우 Software는 NULL X

		//https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag
		0,//장치 생성 시 사용할 플래그를 지정합니다. 예를 들어, 디버그 모드로 생성하거나, 단일 스레드 모드로 설정할 수 있습니다.

		nullptr,//지원하고자 하는 기능 수준의 배열을 지정합니다. 장치가 생성될 때 이 배열에 지정된 순서대로 기능 수준을 시도합니다.
		0, //(이전 인자)pFeatureLevels 배열의 요소 수

		D3D11_SDK_VERSION, //SDK 버전을 지정합니다.
#pragma endregion

#pragma region SwapChain Parameter
		& desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
#pragma endregion
	);
	
	//D3D11CreateDeviceAndSwapChain의 결과(HRESULT)가 성공했는지 확인
	assert(SUCCEEDED(hr));

}

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	//Swap Chaind에서 backBuffer 획득
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	//RTV 생성
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	assert(SUCCEEDED(hr));

}

void Game::SetViewPort()
{
	_viewPort.TopLeftX = 0.f;
	_viewPort.TopLeftY = 0.f;
	_viewPort.Width =  static_cast<float>(_width);
	_viewPort.Height = static_cast<float>(_height);
	_viewPort.MinDepth = 0.f;
	_viewPort.MaxDepth = 1.f;

}
