#include "BodyUDPComponent.h"
#include "Actor.h"

BodyUDPComponent::BodyUDPComponent(Actor* owner, u_short port)
	:UDPSocket(owner, port, false)
{

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
			printf("[ID]%f [X]%f, [Y]%f [Z]%f\n", point[0], point[1], point[2], point[3]);

			bodyPositions.push_back(point);
		}
	}
	
}

const Vector2 BodyUDPComponent::GetPointPosition(int index)
{
	Vector2 vec;

	vec.x = 0;
	vec.y = 0;

	return vec;
}

std::vector<std::vector<float>> BodyUDPComponent::GetAllPosition()
{
	return bodyPositions;
}