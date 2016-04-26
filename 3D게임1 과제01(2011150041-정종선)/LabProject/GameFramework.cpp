//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"
#include <random>

using namespace std;
int k = 0;
bool onLine = false;
//다음은 게임 프레임워크에서 사용할 타이머이다.
CGameTimer m_GameTimer;

//다음은 프레임 레이트를 주 윈도우의 캡션에 출력하기 위한 문자열이다.
_TCHAR m_pszBuffer[50];

CGameFramework::CGameFramework()
{
	m_hInstance = NULL;
	m_hWnd = NULL;     

    m_hDCFrameBuffer = NULL;   
    m_hBitmapFrameBuffer = NULL;   

	m_bActive = true;    

	m_nObjects = 0;
	_tcscpy_s(m_pszBuffer, _T("LapProject ("));
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
	CLineMesh *pLineMesh = new CLineMesh();
	m_pCubespce = new CCubeSpace();
	
	
	default_random_engine dre;
	uniform_int_distribution<> ui(0,255);
	uniform_real_distribution<> speedur(-20,20);
	uniform_real_distribution<> ur(-50, 50);

	m_nObjects = 100;
	m_pObjects = new CGameObject[m_nObjects];
	m_pMasterObject = new CGameObject();
	m_pMasterObject->SetMesh(masterCubeMesh);
	m_pMasterObject->SetColor(RGB(255, 0, 0));
	m_pMasterObject->SetPosition(0, 0, 0);
	m_pMasterObject->m_OvjectMoveVector = D3DXVECTOR3((float)speedur(dre), (float)speedur(dre), (float)speedur(dre));
	m_pLineObject = new CGameObject();
	m_pLineObject->SetMesh(pLineMesh);
	m_pLineObject->SetColor(RGB(255, 0, 0));
	m_pLineObject->SetPosition(m_pMasterObject->m_WorldMatrix._41, m_pMasterObject->m_WorldMatrix._42, m_pMasterObject->m_WorldMatrix._43);
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
	if (GetKeyState(VK_SPACE) & 0xff00)
	{		
		onLine = true;
	}
}

void CGameFramework::CrashCube()
{
	bool pointInRect = true;
	D3DXMATRIX m_ObjectWorldMatrix;
	D3DXMATRIX m_LineWorldMatrix = m_pLineObject->m_WorldMatrix;
	D3DXMATRIX m_MasterWorldMatrix = m_pMasterObject->m_WorldMatrix;
	D3DXVECTOR3 MasterVertice[4], Vertice[6][4];
	D3DXVECTOR3 Line[2]{D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,1,0)};
	D3DXVECTOR3 crossDot, edge, dir, normal, edgeNormal;
	CCubeMesh *pCubeMesh = new CCubeMesh();
	CPolygon **polygon = pCubeMesh->GetPolygon();
	D3DXPLANE CrashPlane,RayPlane;
	
	D3DXVec3TransformCoord(&Line[0], &Line[0], &m_LineWorldMatrix);
	D3DXVec3TransformCoord(&Line[1], &Line[1], &m_LineWorldMatrix);

	if (onLine == true)
	{
		for (int k = 0; k < m_nObjects; k++)
		{
			m_ObjectWorldMatrix = m_pObjects[k].m_WorldMatrix;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					Vertice[i][j] = polygon[i]->m_pVertices[j].m_vPosition;
					D3DXVec3TransformCoord(&Vertice[i][j], &Vertice[i][j], &m_ObjectWorldMatrix);
				}
				D3DXPlaneFromPoints(&CrashPlane, &Vertice[i][0], &Vertice[i][1], &Vertice[i][2]);
				D3DXPlaneIntersectLine(&crossDot, &CrashPlane, &Line[0], &Line[1]);

				D3DXVec3Cross(&normal, &(Vertice[i][1] - Vertice[i][0]), &(Vertice[i][3] - Vertice[i][0]));
				D3DXVec3Normalize(&normal, &normal);

				edge = Vertice[i][1] - Vertice[i][0];
				D3DXVec3Cross(&edgeNormal, &edge, &normal);
				dir = Vertice[i][0] - crossDot;
				if (D3DXVec3Dot(&dir, &edgeNormal) < 0.0f)
				{
					pointInRect = false;
				}

				edge = Vertice[i][2] - Vertice[i][1];
				D3DXVec3Cross(&edgeNormal, &edge, &normal);
				dir = Vertice[i][1] - crossDot;
				if (D3DXVec3Dot(&dir, &edgeNormal) < 0.0f)
				{
					pointInRect = false;
				}

				edge = Vertice[i][3] - Vertice[i][2];
				D3DXVec3Cross(&edgeNormal, &edge, &normal);
				dir = Vertice[i][2] - crossDot;
				if (D3DXVec3Dot(&dir, &edgeNormal) < 0.0f)
				{
					pointInRect = false;
				}

				edge = Vertice[i][0] - Vertice[i][3];
				D3DXVec3Cross(&edgeNormal, &edge, &normal);
				dir = Vertice[i][3] - crossDot;
				if (D3DXVec3Dot(&dir, &edgeNormal) < 0.0f)
				{
					pointInRect = false;
				}
				for (int i = 0; i < 4; ++i)
				{
					MasterVertice[i] = polygon[1]->m_pVertices[i].m_vPosition;
					D3DXVec3TransformCoord(&MasterVertice[i], &MasterVertice[i], &m_MasterWorldMatrix);
				}
				D3DXPlaneFromPoints(&RayPlane, &MasterVertice[0], &MasterVertice[1], &MasterVertice[2]);
				
				if (pointInRect && D3DXPlaneDotCoord(&RayPlane, &crossDot) > 0)
				{
					m_pObjects[k].SetMesh(NULL);
				}
				else
				{
					pointInRect = true;
				}

			}
		}
	}
}

void CGameFramework::AnimateObjects()
{
    float fYaw = 0.0f, fPitch = 0.0f, fRoll = 0.0f;
	
	for (int i = 0; i < m_nObjects; i++)
	{
        fYaw   = 2.0f * (i+1*3);
        fPitch = 2.0f * (i+2*2);
        fRoll  = 1.0f * (i+2*3);
		m_pObjects[i].Rotate(fPitch*m_GameTimer.GetTimeElapsed(), fYaw*m_GameTimer.GetTimeElapsed(), fRoll*m_GameTimer.GetTimeElapsed());
	}
	m_pMasterObject->Rotate(fPitch*m_GameTimer.GetTimeElapsed(), fYaw*m_GameTimer.GetTimeElapsed(), fRoll*m_GameTimer.GetTimeElapsed());
	m_pLineObject->Rotate(fPitch*m_GameTimer.GetTimeElapsed(), fYaw*m_GameTimer.GetTimeElapsed(), fRoll*m_GameTimer.GetTimeElapsed());
}


void CGameFramework::MoveObjects()
{
	m_pMasterObject->Translate(m_pMasterObject->m_OvjectMoveVector*m_GameTimer.GetTimeElapsed());
	m_pLineObject->SetPosition(m_pMasterObject->m_WorldMatrix._41, m_pMasterObject->m_WorldMatrix._42, m_pMasterObject->m_WorldMatrix._43);
	for (int i = 0; i < m_nObjects; i++)
	{
		m_pObjects[i].Translate(m_pObjects[i].m_OvjectMoveVector*m_GameTimer.GetTimeElapsed());
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
	m_GameTimer.Tick();
    if (!m_bActive) return;

	

	ProcessInput();

	AnimateObjects();

	ReflectObject();

	MoveObjects();

    ClearFrameBuffer(RGB(255, 255, 255));

	for (int i = 0; i < m_nObjects; i++) m_pObjects[i].Render(m_hDCFrameBuffer, m_pCamera);
	
	

	if (onLine)
	{
		CrashCube();
		m_pLineObject->Render(m_hDCFrameBuffer, m_pCamera);
		onLine = false;
	}
	m_pMasterObject->Render(m_hDCFrameBuffer, m_pCamera);
	
	
	PresentFrameBuffer();

	/*현재의 프레임 레이트를 문자열로 가져와서 주 윈도우의 타이틀로 출력한다. m_pszBuffer 문자열이 "LapProject ("으로 초기화되었으므로 (m_pszBuffer+12)에서부터 프레임 레이트를 문자열로 출력하여 “ FPS)” 문자열과 합친다.
	_itow_s(m_nCurrentFrameRate, (m_pszBuffer+12), 37, 10);
	wcscat_s((m_pszBuffer+12), 37, _T(" FPS)"));
	*/

	m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);

}


