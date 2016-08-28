#include <Windows.h>
#include <SpriteBatch.h>
#include <assert.h>
#include <WICTextureLoader.h>
#include <time.h>
#include "Game.h"
#include "Tile.h"
#include "DefaultVS.h"
#include "DefaultPS.h"

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

#define CHECKHR(x) if (FAILED(x)) { assert(false); return false; }


struct Vertex
{
    float x, y;
    float u, v;
};

struct VertexConst
{
    float offsetX, offsetY;
    float texOffsetX, texOffsetY;
    float screenWidth, screenHeight;
    float pad0, pad1;
};

struct PixelConst
{
    float tileX, tileY;
    float tileSize;
    int enabled;
};

Game::Game()
{
    _difficulty = int(Difficulty::Easy);
    _numTiles = _difficulty * _difficulty - 1;
    _tileSize = 720 * 0.9f / _difficulty;

    InitializeTiles();
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
    swapDesc.BufferDesc.Width = SCREEN_WIDTH;
    swapDesc.BufferDesc.Height = SCREEN_HEIGHT;
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
    viewport.Width = float(SCREEN_WIDTH);
    viewport.Height = float(SCREEN_HEIGHT);
    viewport.MaxDepth = 1.f;
    _context->RSSetViewports(1, &viewport);

    float screenMarginX = 1.8f * SCREEN_HEIGHT / SCREEN_WIDTH;
    float screenMarginY = 1.8f;

    Vertex vertices[] =
    {
        //background
        { -1, 1, 0, 0 },
        { 1, 1, 1, 0 },
        { -1, -1, 0, 1 },
        { -1, -1, 0, 1 },
        { 1, 1, 1, 0 },
        { 1, -1, 1, 1 },
        //tile
        { -1, 1, 0, 0 },
        { -1 + (screenMarginX / _difficulty), 1, 1.f / _difficulty, 0 },
        { -1, 1 - (screenMarginY / _difficulty), 0, 1.f / _difficulty },
        { -1, 1 - (screenMarginY / _difficulty), 0, 1.f / _difficulty },
        { -1 + (screenMarginX / _difficulty), 1, 1.f / _difficulty, 0 },
        { -1 + (screenMarginX / _difficulty), 1 - (screenMarginY / _difficulty), 1.f / _difficulty, 1.f / _difficulty }
    };


    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.StructureByteStride = sizeof(Vertex);
    bufferDesc.ByteWidth = bufferDesc.StructureByteStride * _countof(vertices);

    D3D11_SUBRESOURCE_DATA vertexSubData{};
    vertexSubData.pSysMem = vertices;
    vertexSubData.SysMemPitch = bufferDesc.ByteWidth;
    vertexSubData.SysMemSlicePitch = bufferDesc.ByteWidth;
    hr = _device->CreateBuffer(&bufferDesc, &vertexSubData, &_vertexBuffer);
    CHECKHR(hr);

    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.StructureByteStride = sizeof(VertexConst);
    bufferDesc.ByteWidth = bufferDesc.StructureByteStride;
    hr = _device->CreateBuffer(&bufferDesc, nullptr, &_vertexConstBuffer);
    CHECKHR(hr);

    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.StructureByteStride = sizeof(PixelConst);
    bufferDesc.ByteWidth = bufferDesc.StructureByteStride;
    hr = _device->CreateBuffer(&bufferDesc, nullptr, &_pixelConstBuffer);
    CHECKHR(hr);

    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

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

    ComPtr<ID3D11Resource> bgImage;
    //hr = DirectX::CreateWICTextureFromFile(_device.Get(), L"dk.png", bgImage.ReleaseAndGetAddressOf(), &_srvBackground);
    CHECKHR(hr);    
    hr = DirectX::CreateWICTextureFromFile(_device.Get(), L"tileimage.jpg", bgImage.ReleaseAndGetAddressOf(), &_srvTile);
    CHECKHR(hr);

    //Sprite Batch / Sprite Font
    _spriteBatch.reset(new DirectX::SpriteBatch(_context.Get()));
    _spriteBatch->SetViewport(viewport);

    _spriteFont.reset(new DirectX::SpriteFont(_device.Get(), L"default.spritefont"));

    return true;
}

int Game::GetDifficulty() const
{
    return _difficulty;
}

bool Game::Update(const MouseState& mouseState)
{
    if (_tileAnim.isAnimating)
    {
        Vector2 dir = _tileAnim.endpoint - _tileAnim.currPos;
        float length = dir.Length();
        float increment = min(length, _tileAnim.increment);
        dir.Normalize();
        _tileAnim.currPos += increment * dir;
        _tileAnim.isAnimating = (_tileAnim.endpoint - _tileAnim.currPos).Length() > 0.001;
        if (!_tileAnim.isAnimating && CheckPuzzle())
        {
            //End game stuff
            return false;
        }
    }
    else if (mouseState.clicked)
    {
        Vector2 mousePos = Vector2(mouseState.x / _tileSize, mouseState.y / _tileSize);
        for (int i = 0; i < _numTiles; i++)
        {
            if (mousePos.x >= _tiles[i].GetPosition().x && mousePos.x < _tiles[i].GetPosition().x + 1 &&
                mousePos.y >= _tiles[i].GetPosition().y && mousePos.y < _tiles[i].GetPosition().y + 1)
            {
                _tileAnim.currPos = _tiles[i].GetPosition();
                _tileAnim.index = i;

                if (_tiles[i].GetPosition().x + 1 == _emptyTile.x && _tiles[i].GetPosition().y == _emptyTile.y)
                {
                    _tileAnim.isAnimating = true;
                    _emptyTile = _tiles[i].GetPosition();
                    _tiles[i].SlideRight();
                }
                else if (_tiles[i].GetPosition().x - 1 == _emptyTile.x && _tiles[i].GetPosition().y == _emptyTile.y)
                {
                    _tileAnim.isAnimating = true;
                    _emptyTile = _tiles[i].GetPosition();
                    _tiles[i].SlideLeft();
                }
                else if (_tiles[i].GetPosition().x == _emptyTile.x && _tiles[i].GetPosition().y + 1 == _emptyTile.y)
                {
                    _tileAnim.isAnimating = true;
                    _emptyTile = _tiles[i].GetPosition();
                    _tiles[i].SlideDown();
                }
                else if (_tiles[i].GetPosition().x == _emptyTile.x && _tiles[i].GetPosition().y - 1 == _emptyTile.y)
                {
                    _tileAnim.isAnimating = true;
                    _emptyTile = _tiles[i].GetPosition();
                    _tiles[i].SlideUp();
                }

                _tileAnim.endpoint = _tiles[i].GetPosition();
                break;
            }
        }
    }

    return true;
}

void Game::Draw()
{
    float background[4] = { 0, 0, 0.5, 1 };
    _context->ClearRenderTargetView(_backBuffer.Get(), background);
    _context->IASetInputLayout(_inputLayout.Get());
    _context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    UINT vertexStride = sizeof(Vertex);
    UINT vertexOffset = 0;
    _context->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);
    _context->VSSetShader(_defaultVS.Get(), nullptr, 0);
    _context->VSSetConstantBuffers(0, 1, _vertexConstBuffer.GetAddressOf());
    _context->PSSetShader(_defaultPS.Get(), nullptr, 0);
    _context->PSSetSamplers(0, 1, _sampler.GetAddressOf());
    _context->PSSetConstantBuffers(0, 1, _pixelConstBuffer.GetAddressOf());

    //Draw background
    //_context->PSSetShaderResources(0, 1, _srvBackground.GetAddressOf());
    //UpdateConstBuffer(Vector2::Zero, Vector2::Zero, false);
    //_context->Draw(6, 0);

    //Draw tiles
    _context->PSSetShaderResources(0, 1, _srvTile.GetAddressOf());

    for (int i = 0; i < _numTiles; i++)
    {
        Vector2 tilePos = _tiles[i].GetPosition();
        if (_tileAnim.isAnimating && _tileAnim.index == i)
        {
            tilePos = _tileAnim.currPos;
        }
        UpdateConstBuffer(tilePos * _tileSize, _tiles[i].GetTexCoords(), true);
        _context->Draw(6, 6);
    }

    //Draw text
    _spriteBatch->Begin();
    _spriteFont->DrawString(_spriteBatch.get(), L"Gooz master zero", Vector2(900, 100), Vector4(1, 1, 0, 1));
    _spriteBatch->End();

    _swapChain->Present(1, 0);
}

bool Game::UpdateConstBuffer(const Vector2& pos, const Vector2& texCoords, bool drawBorder)
{
    VertexConst tileVSConst = { pos.x, pos.y, texCoords.x, texCoords.y, 1280.f, 720.f };
    _context->UpdateSubresource(_vertexConstBuffer.Get(), 0, nullptr, &tileVSConst, sizeof(VertexConst), sizeof(VertexConst));
    PixelConst tilePSConst = { pos.x, pos.y, _tileSize, drawBorder ? 1 : 0 };
    _context->UpdateSubresource(_pixelConstBuffer.Get(), 0, nullptr, &tilePSConst, sizeof(PixelConst), sizeof(PixelConst));
    return true;
}

void Game::InitializeTiles()
{
    int shuffles = 10;
    int current = 0;
    for (int i = 0; i < _numTiles; i++)
    {
        _tiles.push_back(Tile(_difficulty, Vector2(float(i % _difficulty), float(i / _difficulty))));
    }

    _emptyTile = Vector2(float(_difficulty - 1), float(_difficulty - 1));
    srand(UINT(time(nullptr)));
    
    while (current < shuffles)
    //for (int i = 0; i < 100; i++)
    {
        if (_tileAnim.isAnimating)
        {
            Vector2 dir = _tileAnim.endpoint - _tileAnim.currPos;
            float length = dir.Length();
            float increment = min(length, _tileAnim.increment);
            dir.Normalize();
            _tileAnim.currPos += increment * dir;
            _tileAnim.isAnimating = (_tileAnim.endpoint - _tileAnim.currPos).Length() > 0.001;
        }

        if (!_tileAnim.isAnimating)
        {
            std::vector<Vector2> possibleDirs;
            if (_emptyTile.x < _difficulty - 1)  // Empty can move right
            {
                possibleDirs.push_back(Vector2(1, 0));
            }
            if (_emptyTile.x > 0)  // Empty can move left
            {
                possibleDirs.push_back(Vector2(-1, 0));
            }
            if (_emptyTile.y < _difficulty - 1)  // Empty can move down
            {
                possibleDirs.push_back(Vector2(0, 1));
            }
            if (_emptyTile.y > 0)  // Empty can move up
            {
                possibleDirs.push_back(Vector2(0, -1));
            }

            int dir = rand() % possibleDirs.size();
            Vector2 tileToMove = _emptyTile + possibleDirs[dir];

            //TODO: Make a Board object that manages all tile positions
            for (int i = 0; i < _numTiles; i++)
            {
                if ((_tiles[i].GetPosition() - tileToMove).Length() < 0.001f)
                {
                    //Could figure direction and actually slide
                    if ((possibleDirs[dir] - Vector2(1, 0)).Length() < 0.001f)
                    {
                        _tileAnim.isAnimating = true;
                        _emptyTile = tileToMove;
                        _tiles[i].SlideLeft();
                        break;
                    }
                    else
                    {
                        _tiles[i].SetPosition(_emptyTile);
                        _emptyTile = tileToMove;
                        break;
                    }
                }
            }
            current++;
        }
    }
}

bool Game::CheckPuzzle() 
{
    bool win = true;

    for (int i = 0; i < _numTiles; i++)
    {
        if (!_tiles[i].IsCorrectPosition()) {
            win = false;
            break;
        }
    }

    return win;
}