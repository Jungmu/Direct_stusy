#include "stdafx.h"
#include "Shader.h"

BYTE *ReadCompiledEffectFile(WCHAR *pszFileName, int *pnReadBytes)
{
	FILE *pFile = NULL;
	::_wfopen_s(&pFile, pszFileName, L"rb");
	::fseek(pFile, 0, SEEK_END);
	int nFileSize = ::ftell(pFile);
	BYTE *pByteCode = new BYTE[nFileSize];
	::rewind(pFile);
	*pnReadBytes = ::fread(pByteCode, sizeof(BYTE), nFileSize, pFile);
	::fclose(pFile);
	return(pByteCode);
}

CShader::CShader()
{
	m_pd3dVertexShader = NULL;
	m_pd3dPixelShader = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dcbWorldMatrix = NULL;
	m_nReferences = 0;
}

CShader::~CShader()
{
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	ReleaseShaderVariables();
}



void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 정점-쉐이더를 생성한다. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		//컴파일된 쉐이더 코드의 메모리 주소와 입력 레이아웃에서 정점 레이아웃을 생성한다. 
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}
void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 픽셀-쉐이더를 생성한다. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}
void CShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromCompiledFile(pd3dDevice, L"VS.fxo", d3dInputLayout, nElements);
	CreatePixelShaderFromCompiledFile(pd3dDevice, L"PS.fxo");
	//컴파일된 쉐이더 코드의 이름이 VS.fxo와 PS.fxo이다.

	CreateShaderVariables(pd3dDevice);
}


void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	//정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
}

void CShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	//월드 변환 행렬을 위한 상수 버퍼를 생성한다.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbWorldMatrix);
}
void CShader::ReleaseShaderVariables()
{
	//월드 변환 행렬을 위한 상수 버퍼를 반환한다.
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
}
void CShader::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	//월드 변환 행렬을 상수 버퍼에 복사한다.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);

	//상수 버퍼를 디바이스의 슬롯(VS_SLOT_WORLD_MATRIX)에 연결한다.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

//컴파일된 쉐이더 코드에서 정점 쉐이더를 생성한다.
void CShader::CreateVertexShaderFromCompiledFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements)
{
	int nReadBytes = 0;
	BYTE *pByteCode = ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreateVertexShader(pByteCode, nReadBytes, NULL, &m_pd3dVertexShader);
	pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pByteCode, nReadBytes, &m_pd3dVertexLayout);

	if (pByteCode) delete[] pByteCode;
}

//컴파일된 쉐이더 코드에서 픽셀 쉐이더를 생성한다.
void CShader::CreatePixelShaderFromCompiledFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName)
{
	int nReadBytes = 0;
	BYTE *pByteCode = ReadCompiledEffectFile(pszFileName, &nReadBytes);
	HRESULT hResult = pd3dDevice->CreatePixelShader(pByteCode, nReadBytes, NULL, &m_pd3dPixelShader);

	if (pByteCode) delete[] pByteCode;
}

