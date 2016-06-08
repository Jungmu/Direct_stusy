#pragma once
struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX m_d3dxmtxWorld;
};

class CShader
{
private:
	int m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	CShader();
	virtual ~CShader();

	//쉐이더 소스 코드 파일에서 정점-쉐이더를 생성하는 함수이다. 
	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);

	//쉐이더 소스 코드 파일에서 픽셀-쉐이더를 생성하는 함수이다. 
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);

	//쉐이더를 생성하기 위해 호출하는 가상함수이다.
	virtual void CreateShader(ID3D11Device *pd3dDevice);

	//렌더링하기 위해 호출하는 가상함수이다.
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	//쉐이더 클래스의 상수 버퍼를 생성하고 반환하는 멤버 함수를 선언한다. 
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void ReleaseShaderVariables();
	//쉐이더 클래스의 상수 버퍼를 갱신하는 멤버 함수를 선언한다.
	virtual void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld);
	//컴파일된 이펙트 파일에서 정점 쉐이더와 픽셀 쉐이더를 생성하는 함수이다.
	void CreateVertexShaderFromCompiledFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements);
	void CreatePixelShaderFromCompiledFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName);

	//월드 변환 행렬을 위한 상수 버퍼에 대한 인터페이스 포인터를 선언한다.
	ID3D11Buffer *m_pd3dcbWorldMatrix;

	//정점-쉐이더 인터페이스 포인터와 입력-레이아웃 인터페이스 포인터를 선언한다.
	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11InputLayout *m_pd3dVertexLayout;

	//픽셀-쉐이더 인터페이스 포인터를 선언한다.
	ID3D11PixelShader *m_pd3dPixelShader;
};
