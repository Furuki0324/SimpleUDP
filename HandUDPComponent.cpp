#include "HandUDPComponent.h"
#include "Actor.h"

HandUDPComponent::HandUDPComponent(Actor* owner, u_short InPort)
	:UDPSocket(owner, InPort, 1, false)
{
	//MediapipeのHandで取得する21箇所の座標データを保持するメモリを確保
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

		//21箇所の座標リストを作成
		while (handPositions.size() < 21)
		{
			//Mediapipeの識別番号(0~20)とX,Y座標をまとめたリストを作成
			std::vector<float> fPoint;
			std::vector<std::string> strings;
			while (fPoint.size() < 3)
			{
				auto pos = string.find(delimiter, offset);
				std::string s = "000";
				if (pos != std::string::npos)
				{
					//delimiterで指定した文字で区切る
					s = string.substr(offset, pos - offset);

					//区切ったstringに数字以外の記号が入っていたら削除する
					size_t c;
					while ((c = s.find_first_of("[]")) != std::string::npos)
					{
						s.erase(c, 1);
					}
				}
				else if (handPositions.size() == 20)
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