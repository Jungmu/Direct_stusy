#pragma once
#include "Object.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
protected:
	CCamera *m_pCamera;

public:
	CPlayer();
	virtual ~CPlayer();

	//플레이어의 현재 카메라를 설정하고 반환하는 멤버 함수를 선언한다.
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	CCamera *GetCamera() { return(m_pCamera); }

	//플레이어의 상수 버퍼를 생성하고 갱신하는 멤버 함수를 선언한다.
	void CreateShaderVariables(ID3D11Device *pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);
};
