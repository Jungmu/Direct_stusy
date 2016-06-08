#pragma once
#include "Object.h"
class CScene
{
public:
	CScene();
	~CScene();
	ID3D11Device *m_pd3dDevice;
	CLineMesh *m_pLineMesh;
	bool onClick = false;
	int lineRanderCount = 0;
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();	
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void ReflectObject();
	void Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera);
	CGameObject *PickObjectPointedByCursor(int xClient, int yClient);
	virtual CGameObject *PickObjectByRayIntersection(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);

private:
	CCamera *m_pCamera;
	CBoxObject *m_BoxObject;
	CGameObject **m_ppObjects;
	CGameObject *m_PlayerObject;
	CLineObject *m_LineObject;
	CGameObject *m_pSelectedObject;
	int m_nObjects;

};
