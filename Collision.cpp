#include "Collision.h"

Circle::Circle(float radius)
	:mCenter(Vector2::Zero)
	,mRadius(radius)
{

}

Box::Box(Vector2 min, Vector2 max)
	:mMin(min)
	,mMax(max)
{

}

bool Intersect(const Circle& a, const Circle& b)
{
	float betweenCenters = (a.mCenter - b.mCenter).Length();
	float sumRad = a.mRadius + b.mRadius;
	return betweenCenters <= sumRad;
}

//まだ回転した物体の対応は出来ない。
bool Intersect(const Circle& a, const Box& b)
{
	bool collided = false;
	//四角形の辺をそれぞれ線分として外積の計算式から最短距離を導き出す
	Vector2 boxStart;
	Vector2 boxEnd;

	for (int i = 0; i < 4; ++i)
	{
		switch (i)
		{
		case 0:		//左上から右上の辺
			boxStart = b.mMin;
			boxEnd = Vector2(b.mMax.x, b.mMin.y);
			break;

		case 1:		//右上から右下の辺
			boxStart = Vector2(b.mMax.x, b.mMin.y);
			boxEnd = b.mMax;
			break;

		case 2:		//右下から左下の辺
			boxStart = b.mMax;
			boxEnd = Vector2(b.mMin.x, b.mMax.y);
			break;

		case 3:		//左下から左上の辺
			boxStart = Vector2(b.mMin.x, b.mMax.y);
			boxEnd = b.mMin;
			break;
		}
		Vector2 startToCenter = a.mCenter - boxStart;
		Vector2 endToCenter = a.mCenter - boxEnd;
		Vector2 startToEnd = boxEnd - boxStart;

		//startToEndを正規化
		Vector2::Normalize(startToEnd);
		float distance = (startToCenter.x * startToEnd.y) - (startToEnd.x * startToCenter.y);

		//この時点で距離が半径より大きい場合は接触していない
		if (fabs(distance) > a.mRadius) 
		{
			continue;
		}
		//以上で線と円が接触していることは確認できた。

		//ここからは内積の性質を利用して、線分と円が二箇所で接触していることを確認する
		float dotStartAndCenter = Vector2::Dot(startToCenter, startToEnd);
		float dotEndAndCenter = Vector2::Dot(endToCenter, startToEnd);

		if (dotStartAndCenter * dotEndAndCenter <= 0.0f)
		{
			collided = true;
			break;		//一回でも接触していることが分かれば十分なためループ脱出
		}

		//円と線分が一箇所で接触しているか確認
		if (startToCenter.Length() < a.mRadius
			|| endToCenter.Length() < a.mRadius)
		{
			collided = true;
			break;
		}
	}

	return collided;
}