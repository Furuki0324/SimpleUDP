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
		//�Ίp���̒�����0(�l�p�`�̖ʐς�0)�̏ꍇ�͂��̎��_�ŋU
		return false;
	}

	bool intersected = false;
	Vector2 boxStart, boxEnd;

	//�l�p�`�̕ӂ����ꂼ������Ƃ��Č����������m�F����
	for (int i = 0; i < 4; ++i)
	{
		switch (i)
		{
		case 0:		//���ォ��E��̕�
			boxStart = b.mMin;
			boxEnd = Vector2(b.mMax.x, b.mMin.y);
			break;

		case 1:		//�E�ォ��E���̕�
			boxStart = Vector2(b.mMax.x, b.mMin.y);
			boxEnd = b.mMax;
			break;

		case 2:		//�E�����獶���̕�
			boxStart = b.mMax;
			boxEnd = Vector2(b.mMin.x, b.mMax.y);
			break;

		case 3:		//�������獶��̕�
			boxStart = Vector2(b.mMin.x, b.mMax.y);
			boxEnd = b.mMin;
			break;
		}

		//�����̎n�_(ls)��������̏I�_(le)�A�ӂ̎n�_(bs)�ƏI�_(be)��3�_�֌������x�N�g��
		Vector2 lsToLe = a.mEnd - a.mStart;
		Vector2 lsToBs = boxStart - a.mStart;
		Vector2 lsToBe = boxEnd - a.mStart;

		//����3�x�N�g������2�̊O�ς����߂�
		float cross1 = (lsToLe.x * lsToBs.y) - (lsToLe.y * lsToBs.x);
		float cross2 = (lsToLe.x * lsToBe.y) - (lsToLe.y * lsToBe.x);

		//���̊O�ς̐ς����̐��ł���Ό�����Ă��Ȃ�
		if (cross1 * cross2 > 0.0f)
		{
			printf("\x1b[41mCross1 continue.\x1b[m\n");
			continue;
		}

		//�ӂ̎n�_(bs)����ӂ̏I�_(be)�A�����̎n�_(ls)�ƏI�_(le)��3�_�֌������x�N�g��
		Vector2 bsToBe = boxEnd - boxStart;
		Vector2 bsToLs = a.mStart - boxStart;
		Vector2 bsToLe = a.mEnd - boxStart;

		//�O�ψȍ~�̗���͓��l
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

//�܂���]�������̂̑Ή��͏o���Ȃ��B
bool Intersect(const Circle& a, const Box& b)
{
	if ((b.mMax - b.mMin).Length() == 0)
	{
		//printf("\x1b[41mThe box is null.\x1b[m\n");
		return false;
	}

	bool collided = false;
	//�l�p�`�̕ӂ����ꂼ������Ƃ��ĊO�ς̌v�Z������ŒZ�����𓱂��o��
	Vector2 boxStart;
	Vector2 boxEnd;

	for (int i = 0; i < 4; ++i)
	{
		switch (i)
		{
		case 0:		//���ォ��E��̕�
			boxStart = b.mMin;
			boxEnd = Vector2(b.mMax.x, b.mMin.y);
			break;

		case 1:		//�E�ォ��E���̕�
			boxStart = Vector2(b.mMax.x, b.mMin.y);
			boxEnd = b.mMax;
			break;

		case 2:		//�E�����獶���̕�
			boxStart = b.mMax;
			boxEnd = Vector2(b.mMin.x, b.mMax.y);
			break;

		case 3:		//�������獶��̕�
			boxStart = Vector2(b.mMin.x, b.mMax.y);
			boxEnd = b.mMin;
			break;
		}

		Vector2 startToCenter = a.mCenter - boxStart;
		Vector2 endToCenter = a.mCenter - boxEnd;
		Vector2 startToEnd = boxEnd - boxStart;

		//startToEnd�𐳋K��
		Vector2::Normalize(startToEnd);
		float distance = (startToCenter.x * startToEnd.y) - (startToEnd.x * startToCenter.y);

		//���̎��_�ŋ��������a���傫���ꍇ�͐ڐG���Ă��Ȃ�
		if (fabs(distance) > a.mRadius) 
		{
			continue;
		}
		//�ȏ�Ő��Ɖ~���ڐG���Ă��邱�Ƃ͊m�F�ł����B

		//��������͓��ς̐����𗘗p���āA�����Ɖ~����ӏ��ŐڐG���Ă��邱�Ƃ��m�F����
		float dotStartAndCenter = Vector2::Dot(startToCenter, startToEnd);
		float dotEndAndCenter = Vector2::Dot(endToCenter, startToEnd);

		if (dotStartAndCenter * dotEndAndCenter <= 0.0f)
		{
			collided = true;
			break;		//���ł��ڐG���Ă��邱�Ƃ�������Ώ\���Ȃ��߃��[�v�E�o
		}

		//�~�Ɛ�������ӏ��ŐڐG���Ă��邩�m�F
		if (startToCenter.Length() < a.mRadius
			|| endToCenter.Length() < a.mRadius)
		{
			collided = true;
			break;
		}
	}
	return collided;
}