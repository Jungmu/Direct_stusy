#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

private:
	int m_nReferences;
	bool m_bActive;
public:
	void AddRef();
	void Release();

	D3DXVECTOR3 m_ObjectMoveVector;
	D3DXMATRIX m_d3dxmtxWorld;
	CMesh *m_pMesh;
	CShader *m_pShader;
	void SetActive(bool bActive = false) { m_bActive = bActive; }
	bool GetActive() { return m_bActive; }
	void SetPosition(float x, float y, float z);
	void Rotate(float pitch, float yaw, float roll);
	void Translate(D3DXVECTOR3 move);
	virtual void SetMesh(CMesh *pMesh);
	virtual void SetShader(CShader *pShader);
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	//월드 좌표계의 픽킹 광선을 생성한다.
	void GenerateRayForPicking(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxWorld, D3DXMATRIX *pd3dxmtxView, D3DXVECTOR3 *pd3dxvPickRayPosition, D3DXVECTOR3 *pd3dxvPickRayDirection);
	//월드 좌표계의 픽킹 광선을 생성한다.
	int PickObjectByRayIntersection(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);

};
class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	
};
class CBoxObject : public CGameObject
{
public:
	CBoxObject();
	virtual ~CBoxObject();

	D3DXPLANE m_Plane[6];
	D3DXVECTOR3 m_Normal[6];
	D3DXVECTOR3 m_point[6];

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};
class CPlayerObject : public CGameObject
{
public:
	CPlayerObject();
	virtual ~CPlayerObject();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};
class CLineObject : public CGameObject
{
public:
	CMesh *m_pMesh;
	CLineObject();
	virtual ~CLineObject();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};
