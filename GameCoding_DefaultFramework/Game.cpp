#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hWnd = hwnd;

	//_graphics = make_shared<Graphics>(hwnd);
	_graphics = make_shared<Graphics>(_hWnd);
	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());

	//GO
	_gameObject = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
}

void Game::Update()
{
	//Scale Rotation Translation

	_gameObject->Update();
}

void Game::Render()
{
	_graphics->RenderBegin();
	{
		_gameObject->Render(_pipeline);
	}

	_graphics->RenderEnd();
}