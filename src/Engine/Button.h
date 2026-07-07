#pragma once
#include "include.h"
#include "UI.h"

class Button : public UI
{
public:
	Button() = default;
	~Button() = default;
	
	void OnClick();
	void SetFunctions(std::vector < std::function<void* ()>> functions) { functionToCallOnActivate = functions; }
	void AddFunction(std::function<void* ()> function) { functionToCallOnActivate.push_back(function); }
private:
	std::vector<std::function<void* ()>> functionToCallOnActivate;
};

