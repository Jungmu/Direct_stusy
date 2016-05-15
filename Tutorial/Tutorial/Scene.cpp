#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
	m_ppObjects = NULL;
	m_nObjects = 0;
}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	//쉐이더 객체를 생성한다.
	CShader *pShader = new CShader();
	pShader->CreateShader(pd3dDevice);

	//게임 객체에 대한 포인터들의 배열을 정의한다.
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	//삼각형 메쉬(CTriangleMesh)를 생성한다.
	CTriangleMesh *pMesh = new CTriangleMesh(pd3dDevice);

	//삼각형 객체(CTriangleObject)를 생성하고 삼각형 메쉬와 쉐이더를 연결한다.
	CTriangleObject *pObject = new CTriangleObject();
	pObject->SetMesh(pMesh);
	pObject->SetShader(pShader);

	//객체를 저장한다.
	m_ppObjects[0] = pObject;
}

void CScene::ReleaseObjects()
{
	//게임 객체 리스트의 각 객체를 반환(Release)하고 리스트를 소멸시킨다.
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}


bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::ProcessInput()
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext)
{
	//모든 게임 객체들을 렌더링한다.
	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Render(pd3dDeviceContext);
	}
}

