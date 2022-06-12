#include "BodyUDPComponent.h"
#include "Actor.h"
#include <stdexcept>

BodyUDPComponent::BodyUDPComponent(Actor* owner, u_short port, int updateOrder)
	:UDPSocket(owner, port, updateOrder, false)
{
	//Mediapipe��Pose�Ŏ擾����33�ӏ��̍��W�f�[�^��ێ����郁�������m��
	bodyPositions.reserve(33);
	std::vector<float> temp(4, 0);
	bodyPositions = std::vector<std::vector<float>>(33, temp);

	MediaPipeData data{ 0,0,0,0 };
	mpData = std::vector<MediaPipeData>(33, data);
}

void BodyUDPComponent::UDP_Receive()
{
	memset(buf, 0, sizeof(buf));
	int n = recv(sock, buf, sizeof(buf), 0);

	if (n >= 1)
	{
		bodyPositions.clear();

		std::string string(buf);
		auto offset = std::string::size_type(0);

		//33�ӏ��̍��W���X�g���쐬
		while (bodyPositions.size() < 33)
		{
			//MediaPipe�̎��ʔԍ�(0 ~ 32)��X,Y,Z���W���܂Ƃ߂����X�g���쐬
			//�f�[�^�`���[��[ID,X���W,Y���W,Z���W]
			std::vector<float> point;
			while (point.size() < 4)
			{
				auto pos = string.find(delimiter, offset);
				std::string s = "000";

				if (pos != std::string::npos)
				{
					//delimiter�Ŏw�肵�������ŋ�؂�
					s = string.substr(offset, pos - offset);

					//��؂���string�ɐ����ȊO�̋L���������Ă�����폜����
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
				else if (bodyPositions.size() == 32)	//�����̓��t�@�N�^�����O�Ώ�
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
				point.push_back(f);
				offset = pos + delimiter_length;
			}
			//printf("[ID]%f [X]%f, [Y]%f [Z]%f\n", point[0], point[1], point[2], point[3]);

			if (bodyPositions.size() < bodyPositions.max_size()) { bodyPositions.push_back(point); }
			else { printf("\x1b[41mBodyPositions size over\x1b[m\n"); }
		}

		for (int index = 0; index < 33; ++index)
		{
			for (int i = 0; i < 4; ++i)
			{
				auto pos = string.find(delimiter, offset);
				std::string s = "000";

				if (pos != std::string::npos)
				{
					//delimiter�Ŏw�肵�������ŋ�؂�
					s = string.substr(offset, pos - offset);

					//��؂���string�ɐ����ȊO�̋L���������Ă�����폜����
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
				else if (index == 32)	//�����̓��t�@�N�^�����O�Ώ�
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

				switch (i)
				{
				case 0:
					mpData[index].id = std::stoi(s);
					break;

				case 1:
					mpData[index].x = std::stof(s);
					break;

				case 2:
					mpData[index].y = std::stof(s);
					break;

				case 3:
					mpData[index].z = std::stof(s);
					break;
				}

				offset = pos + delimiter_length;
			}
		}
	}
	
}

Vector2 BodyUDPComponent::GetPointPosition(int index)
{
	Vector2 vec;

	try
	{
		vec.x = mpData.at(index).x;
		vec.y = mpData.at(index).y;
	}
	catch (const std::out_of_range& e)
	{
		printf("\x1b[41mOut of bodypositions range.\x1b[m\n");
		printf("Require:%d, Size:%d\n", index, bodyPositions.size());
		vec.x = 0;
		vec.y = 0;
	}

	return vec;
}

const std::vector<std::vector<float>>& BodyUDPComponent::GetAllPosition()
{
	return bodyPositions;
}

const std::vector<MediaPipeData>& BodyUDPComponent::GetMediaPipeData()
{
	return mpData;
}