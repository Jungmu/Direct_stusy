#pragma once
#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)
struct MESHINTERSECTINFO {
	DWORD m_dwFaceIndex;
	float m_fU;
	float m_fV;
	float m_fDistance;
};
struct VS_VB_INSTANCE
{
	D3DXMATRIX m_d3dxTransform;
	D3DXCOLOR m_d3dxColor;
};
class AABB
{
public:
	//바운딩 박스의 최소점과 최대점을 나타내는 벡터이다.
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	//두 개의 바운딩 박스를 합한다.
	void Merge(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Merge(AABB *pAABB);
	//바운딩 박스의 8개의 꼭지점을 행렬로 변환하고 최소점과 최대점을 다시 계산한다.
	void Update(D3DXMATRIX *pd3dxmtxTransform);
};
class CVertex
{
	//정점의 위치 정보(3차원 벡터)를 저장하기 위한 멤버 변수를 선언한다. 
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//CVertex 클래스의 생성자와 소멸자를 다음과 같이 선언한다. 
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
};
class CDiffusedVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
	//정점의 색상을 나타내는 멤버 변수(D3DXCOLOR 구조체)를 선언한다. 
	D3DXCOLOR m_d3dxcDiffuse;
public:
	D3DXVECTOR3 GetPosition() { return m_d3dxvPosition; }
	//생성자와 소멸자를 선언한다.
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = d3dxvPosition; m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	~CDiffusedVertex() { }
};

class CMesh
{
public:
	//CMesh 클래스의 생성자와 소멸자를 선언한다. 
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();
	int CheckRayIntersection(D3DXVECTOR3 *pd3dxvRayPosition, D3DXVECTOR3 *pd3dxvRayDirection, MESHINTERSECTINFO *pd3dxIntersectInfo);
	//CMesh 클래스 객체의 참조(Reference)와 관련된 멤버 변수와 함수를 선언한다.
private:
	int m_nReferences;

public:
	void AddRef();
	void Release();
public:
	/*각 정점의 위치 벡터를 픽킹을 위하여 저장한다(정점 버퍼를 DYNAMIC으로 생성하고 Map()을 하지 않아도 되도록).*/
	CDiffusedVertex *m_pd3dxvPositions;
	/*메쉬의 인덱스를 저장한다(인덱스 버퍼를 DYNAMIC으로 생성하고 Map()을 하지 않아도 되도록).*/
	UINT *m_pnIndices;
	
	/*인덱스 버퍼(인덱스의 배열)에 대한 인터페이스 포인터이다. 인덱스 버퍼는 정점 버퍼(배열)에 대한 인덱스를 가진다.*/
	ID3D11Buffer *m_pd3dIndexBuffer;
	//인덱스 버퍼가 포함하는 인덱스의 개수이다. 
	UINT m_nIndices;
	//인덱스 버퍼에서 메쉬를 표현하기 위해 사용되는 시작 인덱스이다. 
	UINT m_nStartIndex;
	//각 인덱스에 더해질 인덱스이다. 
	int m_nBaseVertex;

protected:
	//정점 데이터를 저장하기 위한 정점 버퍼 인터페이스 포인터를 선언한다.
	ID3D11Buffer *m_pd3dVertexBuffer;

	/*정점 버퍼의 정점 개수, 정점의 바이트 수, 정점 데이터가 정점 버퍼의 어디에서부터 시작하는 가를 나타내는 변수를 선언한다.*/
	UINT m_nVertices;
	UINT m_nStride;
	UINT m_nOffset;

	//정점 데이터가 어떤 프리미티브를 표현하고 있는 가를 나타내는 멤버 변수를 선언한다.
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
	/*래스터라이저 상태 객체에 대한 인터페이스 포인터를 선언한다. 래스터라이저 상태 객체는 “CGameObject" 클래스의 멤버로 추가하여도 된다. 래스터라이저 상태 객체가 메쉬의 멤버일 때와 게임 객체의 멤버일 때의 의미상의 차이를 생각해보기를 바란다.*/
	ID3D11RasterizerState *m_pd3dRasterizerState;

	//정점 데이터를 렌더링하는 멤버 함수를 선언한다.
public:
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
};
class CTriangleMesh : public CMesh
{
public:
	CTriangleMesh(ID3D11Device *pd3dDevice);
	virtual ~CTriangleMesh();
	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};


class CCubeMesh : public CMesh
{
public:
	//직육면체의 가로, 세로, 높이의 크기를 지정하여 직육면체 메쉬를 생성한다.
	CCubeMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	CCubeMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR m_d3dxcDiffuse, int Rast );
	virtual ~CCubeMesh();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void CreateRasterizerStateSolid(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

class CLineMesh : public CMesh
{
public:
	CLineMesh(ID3D11Device *pd3dDevice, D3DXVECTOR3 begin, D3DXVECTOR3 end);
	~CLineMesh();	
	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};