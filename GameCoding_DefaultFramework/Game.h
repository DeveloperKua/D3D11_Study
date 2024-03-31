#pragma once
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	void RenderBegin();
	void RenderEnd();

private:
	/// <summary>
	/// Device�� SwapChain�� ���ÿ� �����մϴ�.
	/// </summary>
	void CreateDeviceAndSwapChain();
	
	/// <summary>
	/// Swap Chain�� ����۳� �ؽ�ó�� ���� 2D ǥ�鿡 ���� �並 �����ϴµ� ���Ǹ�,
	/// �׷��� ������������ ��� ���� �ܰ�(Output Manager stage)���� ������ ����� �ۼ��ϴµ� ����
	/// </summary>
	void CreateRenderTargetView();


	void SetViewPort();
private:
	HWND _hWnd;
	uint32 _width = 0;
	uint32 _height = 0;

private:
	//DX

#pragma region Device & SwapChaun

	//https://learn.microsoft.com/ko-kr/windows/win32/direct3d11/overviews-direct3d-11-devices-intro
	//ID3D11Device / ID3D11DeviceContext 
	//�������� �׷��� ��ġ �ϵ��� ���� ����Ʈ���� �����

	/// <summary>
	/// ��� ���� ���˰� �ڿ� �Ҵ� / ����
	/// </summary>
	ComPtr<ID3D11Device> _device = nullptr;

	/// <summary>
	/// ���� ��� ����, �ڿ��� �׷��� ������ ���ο� ����, GPU�� ������ ������ ��� ����
	/// </summary>
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;

	//https://learn.microsoft.com/ko-kr/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi
	/// <summary>
	/// ���� ���۸��� ������ ���� 
	/// </summary>
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
#pragma endregion

#pragma region RTV(Render Target View)
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
#pragma endregion
	
#pragma region Misc
	D3D11_VIEWPORT _viewPort = { 0 };
#pragma endregion

};

