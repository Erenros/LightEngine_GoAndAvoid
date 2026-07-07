#include "Utils.h"
#include <string>



int8 GenerateRandomNumber(int8 min, int8 max){
	if (max == 0) {
		max += 1;
	}
	return (rand() % max) + min;
}
 
std::string MoveElementInVector(std::string string, int32 startPos, int32 endPos)
{
	string.insert(string.begin() + endPos, string[startPos]);

	string;

	if (startPos < endPos)
	{
		string.erase(startPos, startPos);
	}

	else
	{
		string.erase(startPos + 1, startPos + 1);
	}

	return string;
}

std::string NumberToString(float64 value)
{
	std::string string = std::to_string(value);

	return string;
}

std::string RemoveZeroes(std::string string)
{
	bool isInteger = true;

	for (int32 i = 0; (i < string.size() - 1) && (isInteger == true); i++)
	{
		if (string[i] == '.')
		{
			isInteger = false;
		}
	}

	if (isInteger == false)
	{
		for (int32 j = static_cast<int32>(string.size()) - 2; string[j] == '0'; j--)
		{
			string.erase(j);
		}

		if (string[string.size() - 1] == '.')
		{
			string.erase(string.size() - 1);
		}
	}

	string;

	return string;
}

std::string RemoveZeroes(float64 value)
{
	return std::string();
}

std::string RemoveZeroes(long double value)
{
	return RemoveZeroes(NumberToString(value));
} 

std::string ChangeUnit(std::string string, int32 precision)
{
	string = RemoveZeroes(string); 

	int amountOfDigits = 0;
	int amountOfDigitsAfterDot = 0;

	int dotPos = -1; 

	for (int32 i = static_cast<int32>(string.size()) - 1; i >= 0; i--)
	{
		if (string[i] == '.')
		{
			amountOfDigitsAfterDot = amountOfDigits;
			amountOfDigits = 0;
			dotPos = i;
		}

		else
		{
			amountOfDigits++;
		}
	}

	if ((dotPos == -1) and (amountOfDigits > 3))
	{
		string.append(".");

		dotPos = static_cast<int32>(string.size()) - 1;
	}

	int amountToMove = 0;
	std::string stringToAppend;

	if (amountOfDigits > 21)
	{
		amountToMove = 21;
		stringToAppend = "Sx";
	}

	else if (amountOfDigits > 18)
	{
		amountToMove = 18;
		stringToAppend = "Qi";
	}

	else if (amountOfDigits > 15)
	{
		amountToMove = 15;
		stringToAppend = "Qa";
	}

	else if (amountOfDigits > 12)
	{
		amountToMove = 12;
		stringToAppend = "T";
	}

	else if (amountOfDigits > 9)
	{
		amountToMove = 9;
		stringToAppend = "B";
	}

	else if (amountOfDigits > 6)
	{
		amountToMove = 6;
		stringToAppend = "M";
	}

	else if (amountOfDigits > 3)
	{
		amountToMove = 3;
		stringToAppend = "K";
	}

	if (dotPos != -1)
	{
		string = MoveElementInVector(string, dotPos, dotPos - amountToMove);


		int startPopping = (dotPos - amountToMove) + 1 + precision;


		while (string.size() > startPopping)
		{
			string.pop_back();
		}
	}

	if (string[string.size() - 1] == '.')
	{
		string.pop_back();
	}

	string.append(stringToAppend);

	return string;
} 

std::string ChangeUnit(float64 value, int32 precision)
{
	std::string string = RemoveZeroes(value);

	string = ChangeUnit(string, precision);

	return string;
}


AABB GetRotatedAABB(Vector2<float32> center, Vector2<float32> halfSize, Radians rotation) {

	float32 c = std::cos(rotation);
	float32 s = std::sin(rotation);

	Vector2<float32> localCorners[4]{
		{-halfSize.x, -halfSize.y},
		{halfSize.x, -halfSize.y},
		{halfSize.x, halfSize.y},
		{-halfSize.x, halfSize.y}
	};

	float32 minX = FLT_MAX, maxX = -FLT_MAX;
	float32 minY = FLT_MAX, maxY = -FLT_MAX;

	for (auto& corner : localCorners) {
		float32 worldX = center.x + (corner.x * c - corner.y * s);
		float32 worldY = center.y + (corner.x * s + corner.y * c);

		minX = std::min(minX, worldX);
		maxX = std::max(maxX, worldX);
		minY = std::min(minY, worldY);
		maxY = std::max(maxY, worldY);
	}

	return { minX, minY, maxX, maxY };
}

bool AABB::overlaps(const AABB& other)
{
	return minX < other.maxX && minY < other.maxY && maxX > other.minX && maxY > other.minY;
}

bool AABB::include(const AABB& other)
{
	return other.minX >= minX && other.maxX <= maxX && other.minY >= minY && other.maxY <= maxY;
}
