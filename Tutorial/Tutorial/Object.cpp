#include "stdafx.h"
#include "Object.h"


CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_pMesh = NULL;
	m_pShader = NULL;

	m_nReferences = 0;
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

void CGameObject::Animate(float fTimeElapsed)
{
}
void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	//연결된 쉐이더를 렌더링(준비)한다.
	if (m_pShader) m_pShader->Render(pd3dDeviceContext);
	//연결된 메쉬를 렌더링한다.
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}
CTriangleObject::CTriangleObject()
{
}

CTriangleObject::~CTriangleObject()
{
}

void CTriangleObject::Animate(float fTimeElapsed)
{
	CGameObject::Animate(fTimeElapsed);
}

void CTriangleObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CGameObject::Render(pd3dDeviceContext);
}

