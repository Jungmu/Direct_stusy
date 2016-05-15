float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	/*입력 파라메터인 정점 위치 벡터(Pos)를 그대로 반환한다. 정점에 대한 변환을 하지 않으므로 이 정점의 위치 벡터는 투영 좌표계로 표현된다.*/
	return Pos;
}

float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	/*픽셀-쉐이더는 항상 색상을 나타내는 4개의 실수(float4)를 반환해야 한다. 입력 파라메터에 관계없이 노란색 색상을 반환한다. 그러므로 렌더링의 대상이 되는 모든 픽셀의 색상은 노란색이 된다(노란색 = 빨간색 + 녹색).*/
	return float4(1.0f, 1.0f, 0.0f, 1.0f);    // R=1.0f, G=1.0f, B=0.0f, A=1.0f, 
}
