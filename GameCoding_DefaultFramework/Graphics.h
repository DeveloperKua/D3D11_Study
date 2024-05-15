#pragma once
class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

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

	void SetViewport();

private:
	HWND _hWnd;

private:
#pragma region Device & SwapChaun

	//https://learn.microsoft.com/ko-kr/windows/win32/direct3d11/overviews-direct3d-11-devices-intro
	//ID3D11Device / ID3D11DeviceContext
	//�������� �׷��� ��ġ �ϵ��� ���� ����Ʈ���� �����

	/// <summary>
	/// ��� ���� ����,  �ڿ� �Ҵ� / ����
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

	//RTV(Render Target View)
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	//Misc
	D3D11_VIEWPORT _viewPort = { 0 };
	float _clearColor[4] = { 0.5f,0.5f,0.5f,0.f };


};

