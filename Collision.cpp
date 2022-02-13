#include "Collision.h"

LineSegment::LineSegment(const Vector2& start, const Vector2& end)
	:mStart(start)
	,mEnd(end)
{

}

Circle::Circle(float radius)
	:mCenter(Vector2::Zero)
	,mRadius(radius)
{

}

Box::Box(const Vector2& min, const Vector2& max)
	:mMin(min)
	,mMax(max)
{

}

bool Intersect(const LineSegment& a, const Box& b)
{
	if ((b.mMax - b.mMin).Length() == 0)
	{
		//対角線の長さが0(四角形の面積が0)の場合はその時点で偽
		return false;
	}

	bool intersected = false;
	Vector2 boxStart, boxEnd;

	//四角形の辺をそれぞれ線分として交差条件を確認する
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

		//線分の始点(ls)から線分の終点(le)、辺の始点(bs)と終点(be)の3点へ向かうベクトル
		Vector2 lsToLe = a.mEnd - a.mStart;
		Vector2 lsToBs = boxStart - a.mStart;
		Vector2 lsToBe = boxEnd - a.mStart;

		//この3ベクトルから2つの外積を求める
		float cross1 = (lsToLe.x * lsToBs.y) - (lsToLe.y * lsToBs.x);
		float cross2 = (lsToLe.x * lsToBe.y) - (lsToLe.y * lsToBe.x);

		//この外積の積が正の数であれば交わっていない
		if (cross1 * cross2 > 0.0f)
		{
			printf("\x1b[41mCross1 continue.\x1b[m\n");
			continue;
		}

		//辺の始点(bs)から辺の終点(be)、線分の始点(ls)と終点(le)の3点へ向かうベクトル
		Vector2 bsToBe = boxEnd - boxStart;
		Vector2 bsToLs = a.mStart - boxStart;
		Vector2 bsToLe = a.mEnd - boxStart;

		//外積以降の流れは同様
		cross1 = (bsToBe.x * bsToLs.y) - (bsToBe.y * bsToLs.x);
		cross2 = (bsToBe.x * bsToLe.y) - (bsToBe.y * bsToLe.x);
		if (cross1 * cross2 > 0.0f)
		{
			printf("\x1b[41mCross2 continue.\x1b[m\n");
			continue;
		}

		intersected = true;
		printf("\x1b[41mIntersected.\x1b[m\n");
		break;
	}
	return intersected;
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
	if ((b.mMax - b.mMin).Length() == 0)
	{
		//printf("\x1b[41mThe box is null.\x1b[m\n");
		return false;
	}

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