#pragma once

#include <d3d11.h>
#include <wrl.h>

class Game 
{
public:
  Game();
  ~Game();
  bool Init(HWND window);
  
  bool Update();
  void Draw();

private:
  Microsoft::WRL::ComPtr<ID3D11Device> _device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
  Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _backBuffer;
};