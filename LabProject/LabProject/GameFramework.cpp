//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"
#include <random>

using namespace std;

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
	CMasterCubeMesh *masterCubeMesh = new CMasterCubeMesh();
	CCubeMesh *pCubeMesh = new CCubeMesh();
	m_pCubespce = new CCubeSpace();
	
	
	default_random_engine dre;
	uniform_int_distribution<> ui(0,255);
	uniform_real_distribution<> speedur(-0.05,0.05);
	uniform_real_distribution<> ur(-50, 50);

	m_nObjects = 100;
	m_pObjects = new CGameObject[m_nObjects];
	m_pMasterObject = new CGameObject();
	m_pMasterObject->SetMesh(masterCubeMesh);
	m_pMasterObject->SetColor(RGB(255, 0, 0));
	m_pMasterObject->SetPosition(0, 0, 0);
	m_pMasterObject->m_OvjectMoveVector = D3DXVECTOR3((float)speedur(dre), (float)speedur(dre), (float)speedur(dre));
	for (int i = 0; i < m_nObjects; ++i)
	{
		m_pObjects[i].SetMesh(pCubeMesh);
		m_pObjects[i].SetColor(RGB((BYTE)ui(dre), (BYTE)ui(dre), (BYTE)ui(dre)));
		m_pObjects[i].SetPosition((float)ur(dre), (float)ur(dre), (float)ur(dre));
		m_pObjects[i].m_OvjectMoveVector = D3DXVECTOR3((float)speedur(dre), (float)speedur(dre), (float)speedur(dre));
	}
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
        fYaw   = 0.02f * (i+1*2);
        fPitch = 0.02f * (i+1*3);
        fRoll  = 0.01f * (i+1*4);
        m_pObjects[i].Rotate(fPitch, fYaw, fRoll);
	}
	m_pMasterObject->Rotate(fPitch, fYaw, fRoll);
}

void CGameFramework::MoveObjects()
{
	m_pMasterObject->Translate(m_pMasterObject->m_OvjectMoveVector);
	for (int i = 0; i < m_nObjects; i++)
	{
		m_pObjects[i].Translate(m_pObjects[i].m_OvjectMoveVector);
	}
}

void CGameFramework::ReflectObject()
{
	D3DXVECTOR3 masterObject;
	D3DXVECTOR3 object;
	D3DXVECTOR3 reflectionVector;
	float projection;
	masterObject = D3DXVECTOR3(m_pMasterObject->m_WorldMatrix._41, m_pMasterObject->m_WorldMatrix._42, m_pMasterObject->m_WorldMatrix._43);
	for (int i = 0; i < m_nObjects; i++)
	{		
		object = D3DXVECTOR3(m_pObjects[i].m_WorldMatrix._41, m_pObjects[i].m_WorldMatrix._42, m_pObjects[i].m_WorldMatrix._43);
		for (int j = 0; j < 6; j++)
		{
						if (D3DXPlaneDotCoord(&m_pCubespce->m_Plane[j], &object) < 0)
			{
				projection = D3DXVec3Dot(&m_pObjects[i].m_OvjectMoveVector, &m_pCubespce->m_Normal[j]);
				reflectionVector = m_pObjects[i].m_OvjectMoveVector - (2 * projection * m_pCubespce->m_Normal[j]);
				m_pObjects[i].m_OvjectMoveVector = reflectionVector;
			}			
		}		
	}
	for (int j = 0; j < 6; j++)
	{
		if (D3DXPlaneDotCoord(&m_pCubespce->m_Plane[j], &masterObject) < 0)
		{
			projection = D3DXVec3Dot(&m_pMasterObject->m_OvjectMoveVector, &m_pCubespce->m_Normal[j]);
			reflectionVector = m_pMasterObject->m_OvjectMoveVector - (2 * projection * m_pCubespce->m_Normal[j]);
			m_pMasterObject->m_OvjectMoveVector = reflectionVector;
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
	m_pMasterObject->Render(m_hDCFrameBuffer, m_pCamera);
	PresentFrameBuffer();
}


