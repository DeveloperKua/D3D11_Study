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
	/// Device와 SwapChain을 동시에 생성합니다.
	/// </summary>
	void CreateDeviceAndSwapChain();
	
	/// <summary>
	/// Swap Chain의 백버퍼나 텍스처와 같은 2D 표면에 대한 뷰를 생성하는데 사용되며,
	/// 그래픽 파이프라인의 출력 병합 단계(Output Manager stage)에서 렌더링 결과를 작성하는데 사용됨
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
	//물리적인 그래픽 장치 하드웨어에 대한 소프트웨어 제어기

	/// <summary>
	/// 기능 지원 점검고 자원 할당 / 생성
	/// </summary>
	ComPtr<ID3D11Device> _device = nullptr;

	/// <summary>
	/// 렌더 대상 설정, 자원을 그래픽 파이프 라인에 묶음, GPU가 수행할 렌더링 명령 지시
	/// </summary>
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;

	//https://learn.microsoft.com/ko-kr/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi
	/// <summary>
	/// 더블 버퍼링의 역할을 수행 
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

