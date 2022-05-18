#pragma once

#include "../Engine/Apps/App.h"


class TestFlight : public IApp
{
public:
	TestFlight();
	~TestFlight();

	virtual bool IApp_OnCreate() override final;
	virtual void IApp_OnDestroy() override final;
	virtual void IApp_Update( const float deltaTime ) override final;

private:

};