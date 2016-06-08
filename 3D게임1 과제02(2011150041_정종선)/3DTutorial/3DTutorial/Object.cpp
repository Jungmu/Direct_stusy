#include "stdafx.h"
#include "Object.h"

CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_pMesh = NULL;
	m_pShader = NULL;
	m_nReferences = 0;
	m_bActive = true;
}
CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pShader) m_pShader->Release();
}
void CGameObject::AddRef()
{
	m_nReferences++;
}

void CGameObject::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences <= 0) delete this;
}
void CGameObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}
void CGameObject::SetShader(CShader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();

}
void CGameObject::Rotate(float pitch, float yaw, float roll)
{
	D3DXMATRIX mRotate;
	D3DXMatrixRotationX(&mRotate, D3DXToRadian(pitch));
	D3DXMatrixMultiply(&m_d3dxmtxWorld, &mRotate, &m_d3dxmtxWorld);
	D3DXMatrixRotationY(&mRotate, D3DXToRadian(yaw));
	D3DXMatrixMultiply(&m_d3dxmtxWorld, &mRotate, &m_d3dxmtxWorld);
	D3DXMatrixRotationZ(&mRotate, D3DXToRadian(roll));
	D3DXMatrixMultiply(&m_d3dxmtxWorld, &mRotate, &m_d3dxmtxWorld);
}
void CGameObject::Translate(D3DXVECTOR3 move)
{
	m_d3dxmtxWorld._41 += move.x;
	m_d3dxmtxWorld._42 += move.y;
	m_d3dxmtxWorld._43 += move.z;
}
void CGameObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;
}
void CGameObject::Animate(float fTimeElapsed)
{
}


void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pShader)
	{
		m_pShader->UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
		m_pShader->Render(pd3dDeviceContext);
	}
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}

CRotatingObject::CRotatingObject()
{
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	Rotate(rand() % 180 * fTimeElapsed, rand() % 270 * fTimeElapsed, rand() % 180 * fTimeElapsed);
	Translate(m_ObjectMoveVector*fTimeElapsed);
}

void CRotatingObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (GetActive()) {
		CGameObject::Render(pd3dDeviceContext);
	}
}

CBoxObject::CBoxObject()
{
	D3DXVECTOR3 Normal[6]{ D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 0, 1),
		D3DXVECTOR3(0, -1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(1, 0, 0) };

	D3DXVECTOR3 point[6]{ D3DXVECTOR3(0, 0, PLANE_WIDTH / 2), D3DXVECTOR3(0, 0, -PLANE_WIDTH / 2),
		D3DXVECTOR3(0, PLANE_WIDTH/2, 0), D3DXVECTOR3(0, -PLANE_WIDTH / 2, 0), D3DXVECTOR3(PLANE_WIDTH / 2, 0, 0), D3DXVECTOR3(-PLANE_WIDTH / 2, 0, 0) };

	for (int i = 0; i < 6; ++i)
	{
		m_point[i] = point[i];
		m_Normal[i] = Normal[i];
		D3DXPlaneFromPointNormal(&m_Plane[i], &m_point[i], &m_Normal[i]);
	}
}

CBoxObject::~CBoxObject()
{
}
void CBoxObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CGameObject::Render(pd3dDeviceContext);
}

CPlayerObject::CPlayerObject()
{
}
CPlayerObject::~CPlayerObject()
{
}
void CPlayerObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
		CGameObject::Render(pd3dDeviceContext);		
}
void CGameObject::GenerateRayForPicking(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxWorld, D3DXMATRIX *pd3dxmtxView, D3DXVECTOR3 *pd3dxvPickRayPosition, D3DXVECTOR3 *pd3dxvPickRayDirection)
{
	//pd3dxvPickPosition: 카메라 좌표계의 점(화면 좌표계에서 마우스를 클릭한 점을 역변환한 점)
	//pd3dxmtxWorld: 월드 변환 행렬, pd3dxmtxView: 카메라 변환 행렬
	//pd3dxvPickRayPosition: 픽킹 광선의 시작점, pd3dxvPickRayDirection: 픽킹 광선 벡터
	/*객체의 월드 변환 행렬이 주어지면 객체의 월드 변환 행렬과 카메라 변환 행렬을 곱하고 역행렬을 구한다. 이것은 카메라 변환 행렬의 역행렬과 객체의 월드 변환 행렬의 역행렬의 곱과 같다. 객체의 월드 변환 행렬이 주어지지 않으면 카메라 변환 행렬의 역행렬을 구한다. 객체의 월드 변환 행렬이 주어지면 모델 좌표계의 픽킹 광선을 구하고 그렇지 않으면 월드 좌표계의 픽킹 광선을 구한다.*/
	D3DXMATRIX d3dxmtxInverse;
	D3DXMATRIX d3dxmtxWorldView = *pd3dxmtxView;
	if (pd3dxmtxWorld) D3DXMatrixMultiply(&d3dxmtxWorldView, pd3dxmtxWorld, pd3dxmtxView);
	D3DXMatrixInverse(&d3dxmtxInverse, NULL, &d3dxmtxWorldView);
	D3DXVECTOR3 d3dxvCameraOrigin(0.0f, 0.0f, 0.0f);
	/*카메라 좌표계의 원점 (0, 0, 0)을 위에서 구한 역행렬로 변환한다. 변환의 결과는 카메라 좌표계의 원점에 대응되는 모델 좌표계의 점 또는 월드 좌표계의 점이다.*/
	D3DXVec3TransformCoord(pd3dxvPickRayPosition, &d3dxvCameraOrigin, &d3dxmtxInverse);
	/*카메라 좌표계의 점을 위에서 구한 역행렬로 변환한다. 변환의 결과는 마우스를 클릭한 점에 대응되는 모델 좌표계의 점 또는 월드 좌표계의 점이다.*/
	D3DXVec3TransformCoord(pd3dxvPickRayDirection, pd3dxvPickPosition, &d3dxmtxInverse);
	//픽킹 광선의 방향 벡터를 구한다.
	*pd3dxvPickRayDirection = *pd3dxvPickRayDirection - *pd3dxvPickRayPosition;
}
int CGameObject::PickObjectByRayIntersection(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo)
{
	//pd3dxvPickPosition: 카메라 좌표계의 점(화면 좌표계에서 마우스를 클릭한 점을 역변환한 점)
	//pd3dxmtxView: 카메라 변환 행렬
	D3DXVECTOR3 d3dxvPickRayPosition, d3dxvPickRayDirection;
	int nIntersected = 0;
	//활성화된 객체에 대하여 메쉬가 있으면 픽킹 광선을 구하고 객체의 메쉬와 충돌 검사를 한다.
	if (m_bActive && m_pMesh)
	{
		//객체의 모델 좌표계의 픽킹 광선을 구한다.
		GenerateRayForPicking(pd3dxvPickPosition, &m_d3dxmtxWorld, pd3dxmtxView, &d3dxvPickRayPosition, &d3dxvPickRayDirection);
		/*모델 좌표계의 픽킹 광선과 메쉬의 충돌을 검사한다. 픽킹 광선과 메쉬의 삼각형들은 여러 번 충돌할 수 있다. 검사의 결과는 충돌된 횟수이다.*/
		nIntersected = m_pMesh->CheckRayIntersection(&d3dxvPickRayPosition, &d3dxvPickRayDirection, pd3dxIntersectInfo);
	}
	return(nIntersected);
}

CLineObject::CLineObject()
{
}
CLineObject::~CLineObject()
{
}
void CLineObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CGameObject::Render(pd3dDeviceContext);
}
