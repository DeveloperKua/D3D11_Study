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
	SetViewport();

	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();

}

void Game::Update()
{

}

void Game::Render()
{
	RenderBegin();

	{

		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		//IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

		//RS

		//PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

		//OM
		_deviceContext->Draw(_vertices.size(), 0);
	}

	RenderEnd();
}

void Game::RenderBegin()
{
	//후면 버퍼 Draw
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewPort);
}

void Game::RenderEnd()
{
	//전면 버퍼로 Present
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
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
		desc.BufferCount = 1;
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
	CHECK(hr);

}

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	//Swap Chaind에서 backBuffer 획득
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	//RTV 생성
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);

}

void Game::SetViewport()
{
	_viewPort.TopLeftX = 0.f;
	_viewPort.TopLeftY = 0.f;
	_viewPort.Width =  static_cast<float>(_width);
	_viewPort.Height = static_cast<float>(_height);
	_viewPort.MinDepth = 0.f;
	_viewPort.MaxDepth = 1.f;

}

void Game::CreateGeometry()
{
	//vertexData
	{
	_vertices.resize(3);
	 
	_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
	_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);

	_vertices[1].position = Vec3(0.f, 1.f, 0.f);
	_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);

	_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
	_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);

	}

	//vertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		//https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ne-d3d11-d3d11_usage
		//D3D11_USAGE_DEFAULT	:: GPU에서 읽기 및 쓰기 액세스가 필요한 리소스입니다. 이는 가장 일반적인 사용 선택일 수 있습니다.

		//D3D11_USAGE_IMMUTABLE	:: GPU에서만 읽을 수 있는 리소스입니다. GPU에서 작성할 수 없으며 CPU에서 전혀 액세스할 수 없습니다. 
		//							이 유형의 리소스는 만든 후에 변경할 수 없으므로 만들 때 초기화해야 합니다.

		//D3D11_USAGE_DYNAMIC	:: GPU(읽기 전용)와 CPU(쓰기 전용)에서 액세스할 수 있는 리소스입니다. 
		//							동적 리소스는 CPU에서 프레임당 한 번 이상 업데이트되는 리소스에 적합합니다. 
		//							동적 리소스를 업데이트하려면 Map 메서드를 사용합니다.

		//D3D11_USAGE_STAGING	:: GPU에서 CPU로의 데이터 전송(복사)을 지원하는 리소스입니다.
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();

		_device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
	}
}

void Game::CreateInputLayout()
{

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());

}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr);

	CHECK(hr);

}
