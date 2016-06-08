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
	std::default_random_engine dre;
	std::uniform_real_distribution<> speedur(-20, 20);
	std::uniform_real_distribution<> ur(-50, 50);
	m_pd3dDevice = pd3dDevice;
	CShader *pShader = new CShader();
	pShader->CreateShader(pd3dDevice);

	m_nObjects = 100;
	m_ppObjects = new CGameObject*[m_nObjects];
	//가로x세로x높이가 15x15x15인 정육면체 메쉬를 생성한다.
	CCubeMesh *pCubeMesh = new CCubeMesh(pd3dDevice, 15.0f, 15.0f, 15.0f);
	CCubeMesh *pBoxMesh = new CCubeMesh(pd3dDevice, PLANE_WIDTH, PLANE_WIDTH , PLANE_WIDTH , D3DXCOLOR(0.0f,0.0f,0.0f,0.0f),1);
	CCubeMesh *pPlayerMesh = new CCubeMesh(pd3dDevice, 15.0f, 15.0f, 15.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f),2);
	
	m_LineObject = new CLineObject();
	m_LineObject->SetShader(pShader);
	m_LineObject->SetPosition(0, 0, 0);

	m_BoxObject = new CBoxObject();
	m_BoxObject->SetMesh(pBoxMesh);
	m_BoxObject->SetShader(pShader);
	m_BoxObject->SetPosition(0, 0, 0);

	m_PlayerObject = new CPlayerObject();
	m_PlayerObject->SetMesh(pPlayerMesh);
	m_PlayerObject->SetShader(pShader);
	m_PlayerObject->SetPosition(0, 0, 0);
	
	for (int i = 0; i < m_nObjects; ++i)
	{
		CRotatingObject *pObject = new CRotatingObject();
		pObject->SetMesh(pCubeMesh);
		pObject->SetShader(pShader);
		pObject->SetPosition((float)ur(dre), (float)ur(dre), (float)ur(dre));
		pObject->m_ObjectMoveVector = D3DXVECTOR3((float)speedur(dre), (float)speedur(dre), (float)speedur(dre));
		m_ppObjects[i] = pObject;
	}
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
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		/*윈도우의 클라이언트 영역에서 왼쪽 마우스 버튼이 눌려지면 마우스의 위치를 사용하여 픽킹을 처리한다. 마우스 픽킹으로 선택된 객체가 있으면 그 객체를 비활성화한다.*/
		if (m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam)))
		{
						
			m_pSelectedObject->SetActive(false);
		}
			
		break;
	}
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
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fTimeElapsed);
}


void CScene::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	m_BoxObject->Render(pd3dDeviceContext);
	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Render(pd3dDeviceContext);
	}
	m_PlayerObject->Render(pd3dDeviceContext);
	if (onClick)
	{
		m_LineObject->Render(pd3dDeviceContext);
		lineRanderCount++;
		if (lineRanderCount > 50)
		{
			onClick = false;
			lineRanderCount = 0;
		}			
	}	
}


void CScene::ReflectObject()
{
	D3DXVECTOR3 object;
	D3DXVECTOR3 reflectionVector;
	float projection;

	for (int i = 0; i < m_nObjects; i++)
	{
  		object = D3DXVECTOR3(m_ppObjects[i]->m_d3dxmtxWorld._41, m_ppObjects[i]->m_d3dxmtxWorld._42, m_ppObjects[i]->m_d3dxmtxWorld._43);
		for (int j = 0; j < 6; j++)
		{
			if (D3DXPlaneDotCoord(&m_BoxObject->m_Plane[j], &object) < 0)
			{
				projection = D3DXVec3Dot(&m_ppObjects[i]->m_ObjectMoveVector, &m_BoxObject->m_Normal[j]);
				reflectionVector = m_ppObjects[i]->m_ObjectMoveVector - (2 * projection * m_BoxObject->m_Normal[j]);
				m_ppObjects[i]->m_ObjectMoveVector = reflectionVector;
			}
		}
	}
}

CGameObject *CScene::PickObjectPointedByCursor(int xClient, int yClient)
{
	if (!m_pCamera) return(NULL);

	D3DXMATRIX d3dxmtxView = m_pCamera->GetViewMatrix();
	D3DXMATRIX d3dxmtxProjection = m_pCamera->GetProjectionMatrix();
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	D3DXVECTOR3 d3dxvPickPosition;
	/*화면 좌표계의 점 (xClient, yClient)를 화면 좌표 변환의 역변환과 투영 변환의 역변환을 한다. 그 결과는 카메라 좌표계의 점이다. 투영 평면이 카메라에서 z-축으로 거리가 1이므로 z-좌표는 1로 설정한다.*/
	d3dxvPickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / d3dxmtxProjection._11;
	d3dxvPickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / d3dxmtxProjection._22;
	d3dxvPickPosition.z = 1.0f;

	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	MESHINTERSECTINFO d3dxIntersectInfo;
	CGameObject *pIntersectedObject = NULL, *pNearestObject = NULL;
	//씬의 모든 쉐이더 객체에 대하여 픽킹을 처리하여 카메라와 가장 가까운 픽킹된 객체를 찾는다.
	
	pIntersectedObject = PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
	if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
	{
		fNearHitDistance = d3dxIntersectInfo.m_fDistance;
		pNearestObject = pIntersectedObject;
	}
	if (pNearestObject)
	{
		m_pLineMesh = new CLineMesh(m_pd3dDevice, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(pNearestObject->m_d3dxmtxWorld._41, pNearestObject->m_d3dxmtxWorld._42, pNearestObject->m_d3dxmtxWorld._43));
		m_LineObject->SetMesh(m_pLineMesh);
		onClick = true;
	}
	
	return(pNearestObject);
}

CGameObject *CScene::PickObjectByRayIntersection(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo)
{
	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	CGameObject *pSelectedObject = NULL;
	MESHINTERSECTINFO d3dxIntersectInfo;
	/*모든 객체들에 대하여 픽킹 광선을 생성하고 객체의 바운딩 박스와의 교차를 검사한다. 교차하는 객체들 중에 카메라와 가장 가까운 객체의 정보와 객체를 반환한다.*/
	for (int i = 0; i < m_nObjects; i++)
	{
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(pd3dxvPickPosition, pd3dxmtxView, &d3dxIntersectInfo);
		if ((nIntersected > 0) && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pSelectedObject = m_ppObjects[i];
			if (pd3dxIntersectInfo) *pd3dxIntersectInfo = d3dxIntersectInfo;
		}
	}
	return(pSelectedObject);
}