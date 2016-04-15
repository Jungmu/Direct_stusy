#pragma once

#define DegreeToRadian(x)	((x)*3.1415926535/180.0)

struct VIEWPORT
{
	int						m_xTopLeft;
	int						m_yTopLeft;
	int						m_nWidth;
	int						m_nHeight;
};

class CCamera
{
public:
	CCamera();
	~CCamera();

	D3DXMATRIX m_Matrix;

	VIEWPORT				m_Viewport;

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight);
};

class CGameObject;

class CVertex
{
public:
	CVertex() { }
    CVertex(float x, float y, float z) { m_vPosition = D3DXVECTOR3(x, y, z); }

    D3DXVECTOR3				m_vPosition;  
};

class CPolygon
{
public:
	CPolygon(int nVertices);
	virtual ~CPolygon();

    int						m_nVertices;     
    CVertex					*m_pVertices;   

	void SetVertex(int nIndex, CVertex vertex);
	void Draw(HDC hDCFrameBuffer, CGameObject *pObject, CCamera *pCamera);
};

class CMesh
{
public:
	CMesh(int nPolygons);
	virtual ~CMesh();

private:
	int						m_nReferences;

    int						m_nPolygons;     
    CPolygon				**m_ppPolygons;   

public:
	void AddRef() { m_nReferences++; }
	void Release() { m_nReferences--; if (m_nReferences <= 0) delete this; }

public:
	void SetPolygon(int nIndex, CPolygon *pPolygon);
	virtual void Render(HDC hDCFrameBuffer, CGameObject *pObject, CCamera *pCamera);
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh();
	virtual ~CCubeMesh();
};

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

public:
    CMesh					*m_pMesh; 

	DWORD					m_dwColor;

	D3DXMATRIX m_WorldMatrix;



public:
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	
	void SetPosition(float x, float y, float z);
	void Rotate(float pitch, float yaw, float roll);

	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};

