#pragma once
class CShader
{
private:
	int m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
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

public:
	//정점-쉐이더 인터페이스 포인터와 입력-레이아웃 인터페이스 포인터를 선언한다.
	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11InputLayout *m_pd3dVertexLayout;

	//픽셀-쉐이더 인터페이스 포인터를 선언한다.
	ID3D11PixelShader *m_pd3dPixelShader;
};
