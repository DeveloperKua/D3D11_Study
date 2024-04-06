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
	//�ĸ� ���� Draw
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewPort);
}

void Game::RenderEnd()
{
	//���� ���۷� Present
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

#pragma region COMPtr ���� ���
	//_device.Get();
	//*device��� Get�� ��� device

	//*device��� GetAddressOf�� ��� &device
	//_device.GetAddressOf();

#pragma endregion

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

void Game::CreateRenderTargetView()
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
		//D3D11_USAGE_DEFAULT	:: GPU���� �б� �� ���� �׼����� �ʿ��� ���ҽ��Դϴ�. �̴� ���� �Ϲ����� ��� ������ �� �ֽ��ϴ�.

		//D3D11_USAGE_IMMUTABLE	:: GPU������ ���� �� �ִ� ���ҽ��Դϴ�. GPU���� �ۼ��� �� ������ CPU���� ���� �׼����� �� �����ϴ�. 
		//							�� ������ ���ҽ��� ���� �Ŀ� ������ �� �����Ƿ� ���� �� �ʱ�ȭ�ؾ� �մϴ�.

		//D3D11_USAGE_DYNAMIC	:: GPU(�б� ����)�� CPU(���� ����)���� �׼����� �� �ִ� ���ҽ��Դϴ�. 
		//							���� ���ҽ��� CPU���� �����Ӵ� �� �� �̻� ������Ʈ�Ǵ� ���ҽ��� �����մϴ�. 
		//							���� ���ҽ��� ������Ʈ�Ϸ��� Map �޼��带 ����մϴ�.

		//D3D11_USAGE_STAGING	:: GPU���� CPU���� ������ ����(����)�� �����ϴ� ���ҽ��Դϴ�.
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
