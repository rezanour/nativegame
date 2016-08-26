#include <Windows.h>
#include <SpriteBatch.h>
#include <assert.h>
#include <WICTextureLoader.h>
#include "Game.h"
#include "DefaultVS.h"
#include "DefaultPS.h"

using namespace Microsoft::WRL;

#define CHECKHR(x) if (FAILED(x)) { assert(false); return false; }

struct Vertex
{
    float x, y;
    float u, v;
};

Game::Game()
{
}

Game::~Game()
{
    CoUninitialize();
}

bool Game::Init(HWND window)
{
    UINT flags = 0;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    DXGI_SWAP_CHAIN_DESC swapDesc{};
    HRESULT hr = S_OK;

#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG
    hr = CoInitialize(nullptr);
    CHECKHR(hr);

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
    CHECKHR(hr);

    ComPtr<ID3D11Texture2D> backbuffer;
    hr = _swapChain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
    CHECKHR(hr);

    hr = _device->CreateRenderTargetView(backbuffer.Get(), nullptr, &_backBuffer);
    CHECKHR(hr);

    _context->OMSetRenderTargets(1, _backBuffer.GetAddressOf(), nullptr);
    
    D3D11_VIEWPORT viewport{};
    viewport.Width = 1280.f;
    viewport.Height = 720.f;
    viewport.MaxDepth = 1.f;
    _context->RSSetViewports(1, &viewport);

    Vertex bgQuad[6] = 
    {
        { -1, 1, 0, 0 },
        {  1, 1, 1, 0 },
        { -1, -1, 0, 1 },
        { -1, -1, 0, 1 },
        { 1, 1, 1, 0 },
        { 1, -1, 1, 1 }
    };

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.StructureByteStride = sizeof(Vertex);
    bufferDesc.ByteWidth = bufferDesc.StructureByteStride * _countof(bgQuad);
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = bgQuad;
    initData.SysMemPitch = bufferDesc.ByteWidth;
    initData.SysMemSlicePitch = bufferDesc.ByteWidth;
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    hr = _device->CreateBuffer(&bufferDesc, &initData, &_vertexBuffer);
    CHECKHR(hr);
    hr = _device->CreateVertexShader(DefaultVS, sizeof(DefaultVS), nullptr, &_defaultVS);
    CHECKHR(hr);
    hr = _device->CreatePixelShader(DefaultPS, sizeof(DefaultPS), nullptr, &_defaultPS);
    CHECKHR(hr);
    hr = _device->CreateSamplerState(&samplerDesc, &_sampler);
    CHECKHR(hr);

    D3D11_INPUT_ELEMENT_DESC inputElems[2]{};
    inputElems[0].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElems[0].SemanticName = "POSITION";
    inputElems[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElems[1].AlignedByteOffset = sizeof(float) * 2;
    inputElems[1].SemanticName = "TEXCOORD";

    hr = _device->CreateInputLayout(inputElems, _countof(inputElems), DefaultVS, sizeof(DefaultVS), &_inputLayout);
    CHECKHR(hr);

    _context->IASetInputLayout(_inputLayout.Get());
    _context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT vertexStride = sizeof(Vertex);
    UINT vertexOffset = 0;
    _context->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);

    _context->VSSetShader(_defaultVS.Get(), nullptr, 0);
    _context->PSSetShader(_defaultPS.Get(), nullptr, 0);

    ComPtr<ID3D11Resource> bgImage;
    hr = DirectX::CreateWICTextureFromFile(_device.Get(), L"dk.png", &bgImage, &_shaderResourceView);
    CHECKHR(hr);
    _context->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
    _context->PSSetSamplers(0, 1, _sampler.GetAddressOf());

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
    _context->Draw(6, 0);
    _swapChain->Present(1, 0);
}