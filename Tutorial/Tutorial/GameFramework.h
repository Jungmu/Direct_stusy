#pragma once
#include "Timer.h"
#include "Scene.h"

class CGameFramework
{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	//디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요하다.
	ID3D11Device *m_pd3dDevice;
	//스왑 체인 인터페이스에 대한 포인터이다. 주로 디스플레이를 제어하기 위하여 필요하다.
	IDXGISwapChain *m_pDXGISwapChain;
	//디바이스 컨텍스트에 대한 포인터이다. 주로 파이프라인 설정을 하기 위하여 필요하다.
	ID3D11DeviceContext *m_pd3dDeviceContext;
	//렌더 타겟 뷰 인터페이스에 대한 포인터이다. 
	ID3D11RenderTargetView *m_pd3dRenderTargetView;
	CGameTimer m_GameTimer;
	CScene *m_pScene;
	_TCHAR m_pszBuffer[50];
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	//디바이스, 스왑 체인, 디바이스 컨텍스트, 디바이스와 관련된 뷰를 생성하는 함수이다. 
	void SetViewport();
	bool CreateRenderTargetView();
	bool CreateDirect3DDisplay();

	//렌더링할 메쉬, 객체를 생성하고 소멸하는 함수이다. 
	void BuildObjects();
	void ReleaseObjects();

	//프레임워크의 핵심(사용자 입력, 애니메이션, 렌더링)을 구성하는 함수이다. 
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	//윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};
