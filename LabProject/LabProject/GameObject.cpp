#include "StdAfx.h"
#include "GameObject.h"


CCubeSpace::CCubeSpace()
{
	D3DXVECTOR3 Normal[6]{D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 0, 1),
		D3DXVECTOR3(0, -1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(1, 0, 0)};

	D3DXVECTOR3 point[6]{D3DXVECTOR3(0, 0, 50), D3DXVECTOR3(0, 0, -50),
		D3DXVECTOR3(0, 50, 0), D3DXVECTOR3(0, -50, 0), D3DXVECTOR3(50, 0, 0), D3DXVECTOR3(-50, 0, 0)};

	
	for (int i = 0; i < 6; ++i)
	{
		m_point[i] = point[i];
		m_Normal[i] = Normal[i];
		D3DXPlaneFromPointNormal(&m_Plane[i], &m_point[i], &m_Normal[i]);
	}	
}

CPolygon::CPolygon(int nVertices) 
{ 
	if (nVertices > 0)
	{
		m_nVertices = nVertices; 
		m_pVertices = new CVertex[nVertices]; 
	}
}

CPolygon::~CPolygon() 
{ 
	if (m_pVertices) delete [] m_pVertices; 
}

void CPolygon::SetVertex(int nIndex, CVertex vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	{
		m_pVertices[nIndex] = vertex;
	}
}

void CPolygon::Draw(HDC hDCFrameBuffer, CGameObject *pObject, CCamera *pCamera)
{

	CVertex vertex;
	D3DXVECTOR3 vPrevious, vCurrent;

	D3DXMATRIX m_WorldMatrix = pObject->m_WorldMatrix;
	D3DXMATRIX m_CameraMetrix = pCamera->m_Matrix;
	D3DXMATRIX m_PerspectiveMatrix;
	D3DXMATRIX m_ScreenMatrix;
	
	for (int i = 0; i <= m_nVertices; i++)
	{
		vertex = m_pVertices[i % m_nVertices];
		vCurrent = vertex.m_vPosition;	

		// 원근투영 행렬 초기화
		D3DXMatrixPerspectiveFovLH(&m_PerspectiveMatrix, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);

		// 화면 좌표변환 행렬 초기화
		m_ScreenMatrix._11 = pCamera->m_Viewport.m_nWidth * 0.5;
		m_ScreenMatrix._12 = m_ScreenMatrix._13 = m_ScreenMatrix._14 = 0;
		m_ScreenMatrix._22 = -1 * pCamera->m_Viewport.m_nHeight * 0.5;
		m_ScreenMatrix._21 = m_ScreenMatrix._23 = m_ScreenMatrix._24 = 0;
		m_ScreenMatrix._33 = 1;
		m_ScreenMatrix._31 = m_ScreenMatrix._32 = m_ScreenMatrix._34 = 0;
		m_ScreenMatrix._41 = pCamera->m_Viewport.m_xTopLeft + (pCamera->m_Viewport.m_nWidth * 0.5);
		m_ScreenMatrix._42 = pCamera->m_Viewport.m_yTopLeft + (pCamera->m_Viewport.m_nHeight * 0.5);
		m_ScreenMatrix._43 = 0;
		m_ScreenMatrix._44 = 1;

		// 월드변환
		D3DXVec3TransformCoord(&vCurrent, &vCurrent, &m_WorldMatrix);
		// 카메라 변환
		D3DXVec3TransformCoord(&vCurrent, &vCurrent, &m_CameraMetrix);
		// 원근 투영 변환
		D3DXVec3TransformCoord(&vCurrent, &vCurrent, &m_PerspectiveMatrix);
		// 화면 좌표 변환
		D3DXVec3TransformCoord(&vCurrent, &vCurrent, &m_ScreenMatrix);

		if ((i != 0) && (vCurrent.z > 0.0f))
		{
			::MoveToEx(hDCFrameBuffer, (long)vPrevious.x, (long)vPrevious.y, NULL);
			::LineTo(hDCFrameBuffer, (long)vCurrent.x, (long)vCurrent.y);
		}
		vPrevious = vCurrent; 


	}

	




//	CVertex vertex;
//    D3DXVECTOR3 vPrevious, vCurrent, vRotated;
//    for (int i = 0; i <= m_nVertices; i++) 
//    {
//		vertex = m_pVertices[i % m_nVertices];
//        vRotated = vCurrent = vertex.m_vPosition;	
//
////Rotation Transformation
//		if (pObject->m_fxRotation != 0.0f) 
//		{
//			 vRotated.y = float(vCurrent.y * cos(DegreeToRadian(pObject->m_fxRotation)) - vCurrent.z * sin(DegreeToRadian(pObject->m_fxRotation)));
//			 vRotated.z = float(vCurrent.y * sin(DegreeToRadian(pObject->m_fxRotation)) + vCurrent.z * cos(DegreeToRadian(pObject->m_fxRotation)));
//			 vCurrent = vRotated;
//		}
//		if (pObject->m_fyRotation != 0.0f) 
//		{
//			 vRotated.x = float(vCurrent.x * cos(DegreeToRadian(pObject->m_fyRotation)) + vCurrent.z * sin(DegreeToRadian(pObject->m_fyRotation)));
//			 vRotated.z = float(-vCurrent.x * sin(DegreeToRadian(pObject->m_fyRotation)) + vCurrent.z * cos(DegreeToRadian(pObject->m_fyRotation)));
//			 vCurrent = vRotated;
//		}
//		if (pObject->m_fzRotation != 0.0f) 
//		{
//			 vRotated.x = float(vCurrent.x * cos(DegreeToRadian(pObject->m_fzRotation)) - vCurrent.y * sin(DegreeToRadian(pObject->m_fzRotation)));
//			 vRotated.y = float(vCurrent.x * sin(DegreeToRadian(pObject->m_fzRotation)) + vCurrent.y * cos(DegreeToRadian(pObject->m_fzRotation)));
//			 vCurrent = vRotated;
//		}
////Translation Transformation
//		vCurrent.x += pObject->m_fxPosition;
//		vCurrent.y += pObject->m_fyPosition;
//		vCurrent.z += pObject->m_fzPosition;
////View Transformation
//		vCurrent.x -= pCamera->m_fxPosition;
//		vCurrent.y -= pCamera->m_fyPosition;
//		vCurrent.z -= pCamera->m_fzPosition;
//		if (pCamera->m_fxRotation != 0.0f) 
//		{
//			 vRotated.y = float(vCurrent.y * cos(DegreeToRadian(-pCamera->m_fxRotation)) - vCurrent.z * sin(DegreeToRadian(-pCamera->m_fxRotation)));
//			 vRotated.z = float(vCurrent.y * sin(DegreeToRadian(-pCamera->m_fxRotation)) + vCurrent.z * cos(DegreeToRadian(-pCamera->m_fxRotation)));
//			 vCurrent.x = vRotated.x;
//			 vCurrent.y = vRotated.y;
//		}
//		if (pCamera->m_fyRotation != 0.0f) 
//		{
//			 vRotated.x = float(vCurrent.x * cos(DegreeToRadian(-pCamera->m_fyRotation)) + vCurrent.z * sin(DegreeToRadian(-pCamera->m_fyRotation)));
//			 vRotated.z = float(-vCurrent.x * sin(DegreeToRadian(-pCamera->m_fyRotation)) + vCurrent.z * cos(DegreeToRadian(-pCamera->m_fyRotation)));
//			 vCurrent.x = vRotated.x;
//			 vCurrent.y = vRotated.y;
//		}
//		if (pCamera->m_fzRotation != 0.0f) 
//		{
//			 vRotated.x = float(vCurrent.x * cos(DegreeToRadian(-pCamera->m_fzRotation)) - vCurrent.y * sin(DegreeToRadian(-pCamera->m_fzRotation)));
//			 vRotated.y = float(vCurrent.x * sin(DegreeToRadian(-pCamera->m_fzRotation)) + vCurrent.y * cos(DegreeToRadian(-pCamera->m_fzRotation)));
//			 vCurrent.x = vRotated.x;
//			 vCurrent.y = vRotated.y;
//		}
////Perspective Transformation
//	    float fAspect = (float)pCamera->m_Viewport.m_nWidth / (float)pCamera->m_Viewport.m_nHeight;
//		if (vCurrent.z != 0.0f) vCurrent.x /= (fAspect * vCurrent.z);
//		if (vCurrent.z != 0.0f) vCurrent.y /= vCurrent.z;
////Screen Transformation
//        vCurrent.x = +vCurrent.x * (pCamera->m_Viewport.m_nWidth * 0.5f) + pCamera->m_Viewport.m_xTopLeft + (pCamera->m_Viewport.m_nWidth * 0.5f);
//        vCurrent.y = -vCurrent.y * (pCamera->m_Viewport.m_nHeight * 0.5f) + pCamera->m_Viewport.m_yTopLeft + (pCamera->m_Viewport.m_nHeight * 0.5f);
//
//        if ((i != 0) && (vCurrent.z > 0.0f)) 
//		{
//			
//			::MoveToEx(hDCFrameBuffer, (long)vPrevious.x, (long)vPrevious.y, NULL);
//			::LineTo(hDCFrameBuffer, (long)vCurrent.x, (long)vCurrent.y);
//		}
//        vPrevious = vCurrent; 
//    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMesh::CMesh(int nPolygons)
{
	if (nPolygons > 0)
	{
		m_nPolygons = nPolygons;
		m_ppPolygons = new CPolygon * [nPolygons];
		m_nReferences = 0;
	}
}

CMesh::~CMesh(void)
{
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; i++) if (m_ppPolygons[i]) delete m_ppPolygons[i];
		delete [] m_ppPolygons;
	}
}

void CMesh::SetPolygon(int nIndex, CPolygon *pPolygon)
{
	if ((0 <= nIndex) && (nIndex < m_nPolygons) && m_ppPolygons)
	{
		m_ppPolygons[nIndex] = pPolygon;
	}
}

void CMesh::Render(HDC hDCFrameBuffer, CGameObject *pObject, CCamera *pCamera)
{
    for (int j = 0; j < m_nPolygons; j++)
    {
        m_ppPolygons[j]->Draw(hDCFrameBuffer, pObject, pCamera);
    } 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCubeMesh::CCubeMesh() : CMesh(6)
{
	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-2, +2, -2));
	pFrontFace->SetVertex(1, CVertex(+2, +2, -2));
	pFrontFace->SetVertex(2, CVertex(+2, -2, -2));
	pFrontFace->SetVertex(3, CVertex(-2, -2, -2));
	SetPolygon(0, pFrontFace);

	CPolygon *pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-2, +2, +2));
	pTopFace->SetVertex(1, CVertex(+2, +2, +2));
	pTopFace->SetVertex(2, CVertex(+2, +2, -2));
	pTopFace->SetVertex(3, CVertex(-2, +2, -2));
	SetPolygon(1, pTopFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-2, -2, +2));
	pBackFace->SetVertex(1, CVertex(+2, -2, +2));
	pBackFace->SetVertex(2, CVertex(+2, +2, +2));
	pBackFace->SetVertex(3, CVertex(-2, +2, +2));
	SetPolygon(2, pBackFace);

	CPolygon *pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-2, -2, -2));
	pBottomFace->SetVertex(1, CVertex(+2, -2, -2));
	pBottomFace->SetVertex(2, CVertex(+2, -2, +2));
	pBottomFace->SetVertex(3, CVertex(-2, -2, +2));
	SetPolygon(3, pBottomFace);

	CPolygon *pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-2, +2, +2));
	pLeftFace->SetVertex(1, CVertex(-2, +2, -2));
	pLeftFace->SetVertex(2, CVertex(-2, -2, -2));
	pLeftFace->SetVertex(3, CVertex(-2, -2, +2));
	SetPolygon(4, pLeftFace);

	CPolygon *pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(+2, +2, -2));
	pRightFace->SetVertex(1, CVertex(+2, +2, +2));
	pRightFace->SetVertex(2, CVertex(+2, -2, +2));
	pRightFace->SetVertex(3, CVertex(+2, -2, -2));
	SetPolygon(5, pRightFace);
}

CCubeMesh::~CCubeMesh(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject() 
{ 
	m_pMesh = NULL; 
	m_dwColor = 0x00000000;

	m_WorldMatrix._11 = 1.0f;
	m_WorldMatrix._12 = 0.0f;
	m_WorldMatrix._13 = 0.0f;
	m_WorldMatrix._14 = 0.0f;

	m_WorldMatrix._21 = 0.0f;
	m_WorldMatrix._22 = 1.0f;
	m_WorldMatrix._23 = 0.0f;
	m_WorldMatrix._24 = 0.0f;

	m_WorldMatrix._31 = 0.0f;
	m_WorldMatrix._32 = 0.0f;
	m_WorldMatrix._33 = 1.0f;
	m_WorldMatrix._34 = 0.0f;

	m_WorldMatrix._41 = 0.0f;
	m_WorldMatrix._42 = 0.0f;
	m_WorldMatrix._43 = 0.0f;
	m_WorldMatrix._44 = 1.0f;

}

CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::Rotate(float pitch, float yaw, float roll)
{
	D3DXMATRIX mRotate;
	D3DXMatrixRotationX(&mRotate, D3DXToRadian(pitch));
	D3DXMatrixRotationY(&mRotate, D3DXToRadian(yaw));
	D3DXMatrixRotationZ(&mRotate, D3DXToRadian(roll));
	D3DXMatrixMultiply(&m_WorldMatrix, &mRotate, &m_WorldMatrix);
}

void CGameObject::Translate(D3DXVECTOR3 move)
{
	m_WorldMatrix._41 += move.x;
	m_WorldMatrix._42 += move.y;
	m_WorldMatrix._43 += move.z;
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_WorldMatrix._41 = x;
	m_WorldMatrix._42 = y;
	m_WorldMatrix._43 = z;
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);

	if (m_pMesh) m_pMesh->Render(hDCFrameBuffer, this, pCamera);

	::SelectObject(hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCamera::CCamera() 
{ 

	D3DXVECTOR3 vEyePt(0.0f, 30.0f, -10.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 2.0f, 14.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_Matrix, &vEyePt, &vLookatPt, &vUpVec);

    m_Viewport.m_nWidth = 640;
    m_Viewport.m_nHeight = 480;
}

CCamera::~CCamera() 
{
}

void CCamera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight)
{
	m_Viewport.m_xTopLeft = xTopLeft;
    m_Viewport.m_yTopLeft = yTopLeft;
    m_Viewport.m_nWidth = nWidth;
    m_Viewport.m_nHeight = nHeight;
}
