#include "stdafx.h"
#include "Mesh.h"

CMesh::CMesh(ID3D11Device *pd3dDevice)
{
	m_nStride = sizeof(CVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nReferences = 0;
}

CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer) m_pd3dVertexBuffer->Release();
}
void CMesh::AddRef()
{
	m_nReferences++;
}
void CMesh::Release()
{
	m_nReferences--;
	if (m_nReferences <= 0) delete this;
}
void CMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pd3dVertexBuffer) pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dDeviceContext->Draw(m_nVertices, m_nOffset);
}
CTriangleMesh::CTriangleMesh(ID3D11Device *pd3dDevice) : CMesh(pd3dDevice)
{
	//삼각형의 세 꼭지점의 위치 벡터를 정의한다. 
	CVertex pVertices[3];
	pVertices[0] = CVertex(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
	pVertices[1] = CVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f));
	pVertices[2] = CVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f));

	//정점 버퍼를 생성한다. 
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CVertex) * 3;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	//정점 버퍼의 원소의 개수, 정점의 바이트 크기, 프리미티브 유형을 지정한다. 
	m_nVertices = 3;
	m_nStride = sizeof(CVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

CTriangleMesh ::~CTriangleMesh()
{
}
void CTriangleMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

