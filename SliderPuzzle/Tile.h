#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

class Tile
{
public:
    Tile();
    Tile(int difficulty, DirectX::SimpleMath::Vector2 initialPosition);
    ~Tile();

    void SlideUp();
    void SlideDown();
    void SlideLeft();
    void SlideRight();
    DirectX::SimpleMath::Vector2 GetPosition() const;
    void SetPosition(const DirectX::SimpleMath::Vector2& newPosition);
    float GetTileSize() const;
    DirectX::SimpleMath::Vector2 GetTexCoords() const;
    bool IsCorrectPosition();

private:
    float _tileSize;
    DirectX::SimpleMath::Vector2 _initPos;
    DirectX::SimpleMath::Vector2 _position;
    DirectX::SimpleMath::Vector2 _texcoords;
};