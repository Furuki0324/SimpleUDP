#include "HandUDPComponent.h"
#include "Actor.h"

HandUDPComponent::HandUDPComponent(Actor* owner, u_short InPort)
	:UDPSocket(owner, InPort, 1, false)
{
	//Mediapipe��Hand�Ŏ擾����21�ӏ��̍��W�f�[�^��ێ����郁�������m��
	handPositions.reserve(21);
}

void HandUDPComponent::UDP_Receive()
{
	memset(buf, 0, sizeof(buf));
	int n = recv(sock, buf, sizeof(buf), 0);

	if (n >= 1)
	{
		handPositions.clear();

		std::string string(buf);
		auto offset = std::string::size_type(0);

		//21�ӏ��̍��W���X�g���쐬
		while (handPositions.size() < 21)
		{
			//Mediapipe�̎��ʔԍ�(0~20)��X,Y���W���܂Ƃ߂����X�g���쐬
			std::vector<float> fPoint;
			std::vector<std::string> strings;
			while (fPoint.size() < 3)
			{
				auto pos = string.find(delimiter, offset);
				std::string s = "000";
				if (pos != std::string::npos)
				{
					//delimiter�Ŏw�肵�������ŋ�؂�
					s = string.substr(offset, pos - offset);

					//��؂���string�ɐ����ȊO�̋L���������Ă�����폜����
					size_t c;
					while ((c = s.find_first_of("[]")) != std::string::npos)
					{
						s.erase(c, 1);
					}
				}
				else if (handPositions.size() == 20)
				{
					//�Ō�̍��ځi21�Ԗڂ�[2]�j�̓R���}���������ߓ���
					s = string.substr(offset);

					size_t c;
					int loop = 0;
					while ((c = s.find_first_of("[]")) != std::string::npos)
					{
						s.erase(c, 1);

						//�������[�v�x��
						loop++;
						if (loop > 10)
						{
							printf("Infinite loop warning.");
							break;
						}
					}
				}

				float f = std::stof(s);

				fPoint.push_back(f);

				offset = pos + delimiter_length;
			}
			printf("[0]%f [1]%f [2]%f\n", fPoint[0], fPoint[1], fPoint[2]);

			handPositions.push_back(fPoint);
		}

	}
}

const Vector2 HandUDPComponent::GetPointPosition(int index)
{
	Vector2 vec;
	if (handPositions.empty())
	{
		vec.x = 0;
		vec.y = 0;
		return vec;
	}

	vec.x = handPositions[index][1];
	vec.y = handPositions[index][2];

	return vec;
}