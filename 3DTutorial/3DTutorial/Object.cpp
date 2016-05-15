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
	D3DXMATRIX mtxRotate;
	//y-축 회전 행렬을 생성하고 월드 변환 행렬에 곱한다. 
	D3DXMatrixRotationY(&mtxRotate, (float)D3DXToRadian(45.0f*fTimeElapsed));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CRotatingObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CGameObject::Render(pd3dDeviceContext);
}




