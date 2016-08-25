#include <Windows.h>
#include <SpriteBatch.h>
#include "Game.h"

using namespace Microsoft::WRL;

Game::Game()
{
}

Game::~Game() 
{
}

bool Game::Init(HWND window)
{
  UINT flags = 0;
  D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
  DXGI_SWAP_CHAIN_DESC swapDesc {};
  HRESULT hr = S_OK;

#ifdef _DEBUG
  flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG
  
  swapDesc.BufferCount = 2;
  swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapDesc.BufferDesc.Width = 1280;
  swapDesc.BufferDesc.Height = 720;
  swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapDesc.OutputWindow = window;
  swapDesc.SampleDesc.Count = 1;
  swapDesc.Windowed = TRUE;
  
  hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
    &featureLevel, 1, D3D11_SDK_VERSION, &swapDesc, &_swapChain, &_device, nullptr, &_context);
  if (FAILED(hr)) 
  {
    return false;
  }

  ComPtr<ID3D11Texture2D> backbuffer;
  hr = _swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
  if (FAILED(hr))
  {
    return false;
  }

  hr = _device->CreateRenderTargetView(backbuffer.Get(), nullptr, &_backBuffer);
  if (FAILED(hr))
  {
    return false;
  }

  _context->OMSetRenderTargets(1, _backBuffer.GetAddressOf(), nullptr);

  return true;
}

bool Game::Update()
{

  return true;
}

void Game::Draw()
{
  float background[4] = { 0, 0, 0.5, 1 };
  _context->ClearRenderTargetView(_backBuffer.Get(), background);
  //Render everything here
  _swapChain->Present(1, 0);
}