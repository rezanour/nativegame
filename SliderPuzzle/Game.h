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

struct WindowSize
{
	int width, height;
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
	bool UpdateConstBuffer(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& texCoords, float size, bool drawBorder);
	bool UpdateBlurConstBuffer(const DirectX::SimpleMath::Vector2& direction);
	void InitializeTiles();
	bool CheckPuzzle();
	void MeasureMenuStrings();
	const wchar_t* GetFormattedTime(const double time);

	int _difficulty;
	bool _atMenu;
	std::vector<Tile> _tiles;
	int _numTiles;
	float _tileSize;
	DirectX::SimpleMath::Vector2 _emptyTile;
	TileAnimation _tileAnim{};
	time_t _gameTimer;
	time_t _gameStartTime;

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
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _blurTargetPass1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _blurTargetPass2;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _blurTexturePass1;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _blurTexturePass2;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _yellowTex;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _tileVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _borderVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexConstBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pixelConstBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _blurConstBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _defaultVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _defaultPS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _blurPS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvBackground;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvTile;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _blurResourcePass1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _blurResourcePass2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvYellow;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> _sampler;

    std::shared_ptr<DirectX::SpriteBatch> _spriteBatch;
    std::shared_ptr<DirectX::SpriteFont> _spriteFont;
};