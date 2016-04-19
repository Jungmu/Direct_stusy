//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
	m_hInstance = NULL;
	m_hWnd = NULL;     

    m_hDCFrameBuffer = NULL;   
    m_hBitmapFrameBuffer = NULL;   

	m_bActive = true;    

	m_nObjects = 0;
}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
    srand(timeGetTime());

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	RECT rc;
	GetClientRect(m_hWnd, &rc);

	m_pCamera = new CCamera();
	m_pCamera->SetViewport(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);

	BuildFrameBuffer(); 

	BuildObjects(); 

	SetupGameState();

	return(true);
}

void CGameFramework::BuildFrameBuffer()
{
    HDC hDC = ::GetDC(m_hWnd);

    m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, m_pCamera->m_Viewport.m_nWidth, m_pCamera->m_Viewport.m_nHeight);    
    ::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
    ::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
    HBRUSH hBrush = ::CreateSolidBrush(dwColor);
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_pCamera->m_Viewport.m_xTopLeft, m_pCamera->m_Viewport.m_yTopLeft, m_pCamera->m_Viewport.m_nWidth, m_pCamera->m_Viewport.m_nHeight);
    ::SelectObject(m_hDCFrameBuffer, hOldBrush);
    ::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{    
    HDC hDC = ::GetDC(m_hWnd);
    ::BitBlt(hDC, m_pCamera->m_Viewport.m_xTopLeft, m_pCamera->m_Viewport.m_yTopLeft, m_pCamera->m_Viewport.m_nWidth, m_pCamera->m_Viewport.m_nHeight, m_hDCFrameBuffer, m_pCamera->m_Viewport.m_xTopLeft, m_pCamera->m_Viewport.m_yTopLeft, SRCCOPY);
    ::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::SetupGameState()
{
}

void CGameFramework::SetupRenderStates()
{
}

void CGameFramework::BuildObjects()
{

	CCubeMesh *pCubeMesh = new CCubeMesh();
	m_pCubespce = new CCubeSpace();

	m_nObjects = 2;
	m_pObjects = new CGameObject[m_nObjects];
	m_pObjects[0].SetMesh(pCubeMesh);
	m_pObjects[0].SetColor(RGB(255, 0, 0));
	m_pObjects[0].SetPosition(-3.5f, +2.0f, +14.0f);
	m_pObjects[0].m_OvjectMoveVector = D3DXVECTOR3(0.0f, 0.0f, 0.01f);
    m_pObjects[1].SetMesh(pCubeMesh);
	m_pObjects[1].SetColor(RGB(0, 0, 255));
	m_pObjects[1].SetPosition(+3.5f, -2.0f, +14.0f);
	m_pObjects[1].m_OvjectMoveVector = D3DXVECTOR3(0.01f, 0.02f, 0.0f);
}

void CGameFramework::ReleaseObjects()
{
	if (m_pObjects) delete [] m_pObjects;
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
    if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);

    if (m_hWnd) DestroyWindow(m_hWnd);
}

void CGameFramework::ProcessInput()
{
	if (GetKeyState(VK_LEFT) & 0xFF00)m_pCamera->m_Matrix._41 += 25.0f * 0.0005f;
	if (GetKeyState(VK_RIGHT) & 0xFF00) m_pCamera->m_Matrix._41 -= 25.0f * 0.0005f;
	if (GetKeyState(VK_UP) & 0xFF00) m_pCamera->m_Matrix._42 -= 25.0f * 0.0005f;
	if (GetKeyState(VK_DOWN) & 0xFF00) m_pCamera->m_Matrix._42 += 25.0f * 0.0005f;
	if (GetKeyState(VK_DELETE) & 0xFF00) m_pCamera->m_Matrix._43 += 25.0f * 0.0005f;
	if (GetKeyState(VK_END) & 0xFF00) m_pCamera->m_Matrix._43 -= 25.0f * 0.0005f;
}

void CGameFramework::AnimateObjects()
{
    float fYaw = 0.0f, fPitch = 0.0f, fRoll = 0.0f;

	for (int i = 0; i < m_nObjects; i++)
	{
        fYaw   = 0.02f * (i+1);
        fPitch = 0.02f * (i+1);
        fRoll  = 0.01f * (i+1);
        m_pObjects[i].Rotate(fPitch, fYaw, fRoll);
	}
}

void CGameFramework::MoveObjects()
{
	
	for (int i = 0; i < m_nObjects; i++)
	{
		m_pObjects[i].Translate(m_pObjects[i].m_OvjectMoveVector);
	}
}

void CGameFramework::ReflectObject()
{
	
	D3DXVECTOR3 object;
	D3DXVECTOR3 reflectionVector;
	float projection;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			object = D3DXVECTOR3(m_pObjects[i].m_WorldMatrix._41, m_pObjects[i].m_WorldMatrix._42, m_pObjects[i].m_WorldMatrix._43);
			if (D3DXPlaneDotCoord(&m_pCubespce->m_Plane[j], &object) < 0)
			{
				projection = D3DXVec3Dot(&m_pObjects[i].m_OvjectMoveVector, &m_pCubespce->m_Normal[j]);
				reflectionVector = m_pObjects[i].m_OvjectMoveVector - (2 * projection * m_pCubespce->m_Normal[j]);
				m_pObjects[i].m_OvjectMoveVector = reflectionVector;
			}
		}
		
	}
}

void CGameFramework::FrameAdvance()
{    
    if (!m_bActive) return;

	

	ProcessInput();

	AnimateObjects();

	ReflectObject();

	MoveObjects();

    ClearFrameBuffer(RGB(255, 255, 255));

	for (int i = 0; i < m_nObjects; i++) m_pObjects[i].Render(m_hDCFrameBuffer, m_pCamera);

	PresentFrameBuffer();
}


