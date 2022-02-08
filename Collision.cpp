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

//�܂���]�������̂̑Ή��͏o���Ȃ��B
bool Intersect(const Circle& a, const Box& b)
{
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