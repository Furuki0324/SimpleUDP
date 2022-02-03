#include "BodyUDPComponent.h"
#include "Actor.h"
#include <stdexcept>

BodyUDPComponent::BodyUDPComponent(Actor* owner, u_short port, int updateOrder)
	:UDPSocket(owner, port, updateOrder, false)
{
	//MediapipeのPoseで取得する33箇所の座標データを保持するメモリを確保
	bodyPositions.reserve(33);

	std::vector<float> v;
	float a = 0;
	printf("The vector size:%d", bodyPositions.capacity());
	try
	{
		v = bodyPositions.at(1);
	}
	catch(const std::out_of_range& e)
	{
		printf("\x1b[41mAn error has occured.\x1b[m\n");
	}
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

		//33箇所の座標リストを作成
		while (bodyPositions.size() < 33)
		{
			//MediaPipeの識別番号(0 ~ 32)とX,Y,Z座標をまとめたリストを作成
			//データ形式ー＞[ID,X座標,Y座標,Z座標]
			std::vector<float> point;
			while (point.size() < 4)
			{
				auto pos = string.find(delimiter, offset);
				std::string s = "000";

				if (pos != std::string::npos)
				{
					//delimiterで指定した文字で区切る
					s = string.substr(offset, pos - offset);

					//区切ったstringに数字以外の記号が入っていたら削除する
					size_t c;
					int loop = 0;
					while ((c = s.find_first_of("[]")) != std::string::npos)
					{
						s.erase(c, 1);

						//無限ループ警告
						loop++;
						if (loop > 10)
						{
							printf("Infinite loop warning.");
							break;
						}
					}
				}
				else if (bodyPositions.size() == 32)	//ここはリファクタリング対象
				{
					//最後の項目（21番目の[2]）はコンマが無いため特例
					s = string.substr(offset);

					size_t c;
					int loop = 0;
					while ((c = s.find_first_of("[]")) != std::string::npos)
					{
						s.erase(c, 1);

						//無限ループ警告
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
	}
	
}

Vector2 BodyUDPComponent::GetPointPosition(int index)
{
	Vector2 vec;

	try
	{
		vec.x = bodyPositions.at(index).at(1);
		vec.y = bodyPositions.at(index).at(2);
	}
	catch (const std::out_of_range& e)
	{
		printf("\x1b[41mOut of bodypositions range.\x1b[m\n");
		vec.x = 0;
		vec.y = 0;
	}

	return vec;
}

std::vector<std::vector<float>> BodyUDPComponent::GetAllPosition()
{
	return bodyPositions;
}