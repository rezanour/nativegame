#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <SimpleMath.h>
#include <vector>
#include <SpriteBatch.h>
#include <SpriteFont.h>

class Tile;

enum class Difficulty
{
    Easy = 3,
    Medium,
    Hard,
    VeryHard
};

struct TileAnimation
{
    const float increment = 0.1f;

    bool isAnimating;
    int index;
    DirectX::SimpleMath::Vector2 endpoint;
    DirectX::SimpleMath::Vector2 currPos;
};
struct MouseState
{
    bool clicked;
    int x, y;
};

class Game
{
public:
    static const int SCREEN_WIDTH = 1280;
    static const int SCREEN_HEIGHT = 720;

    Game();
    ~Game();

    bool Init(HWND window);
    int GetDifficulty() const;
    bool Update(const MouseState& mouseState);
    void Draw();

private:
    bool UpdateConstBuffer(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& texCoords, bool drawBorder);
    void InitializeTiles();
    bool CheckPuzzle();
	void MeasureMenuStrings();

    int _difficulty;
	bool _atMenu;
    std::vector<Tile> _tiles;
    int _numTiles;
    float _tileSize;
    DirectX::SimpleMath::Vector2 _emptyTile;
    TileAnimation _tileAnim{};

	//Menu options
	DirectX::SimpleMath::Rectangle _menuTitle;
	DirectX::SimpleMath::Rectangle _menuEasy;
	DirectX::SimpleMath::Rectangle _menuMedium;
	DirectX::SimpleMath::Rectangle _menuHard;
	DirectX::SimpleMath::Rectangle _menuVeryHard;

    Microsoft::WRL::ComPtr<ID3D11Device> _device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _backBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _tileVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexConstBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _pixelConstBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> _defaultVS;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> _defaultPS;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvBackground;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvTile;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> _sampler;

    std::shared_ptr<DirectX::SpriteBatch> _spriteBatch;
    std::shared_ptr<DirectX::SpriteFont> _spriteFont;
};