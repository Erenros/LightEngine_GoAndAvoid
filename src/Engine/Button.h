#pragma once
#include "include.h"
#include "UI.h"

class Button : public UI
{
public:
	Button() ;
	~Button() = default;

	void Click() ;
	virtual void OnClick() {};

	void SetFunctions(std::vector < std::function<void()>> functions) { m_FunctionToCallOnActivate = functions; }
	void AddFunction(std::function<void()> function) { m_FunctionToCallOnActivate.push_back(function); }

private:
	std::vector<std::function<void()>> m_FunctionToCallOnActivate;
};