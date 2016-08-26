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
    Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> _defaultVS;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> _defaultPS;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> _sampler;
};