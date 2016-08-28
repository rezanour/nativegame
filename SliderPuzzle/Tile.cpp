#include "Tile.h"

using namespace DirectX::SimpleMath;

Tile::Tile()
{

};

Tile::Tile(int difficulty, Vector2 initPos) 
{
    _position = _initPos = initPos;
    _texcoords = initPos / float(difficulty);
};

Tile::~Tile()
{

};

void Tile::SlideUp()
{
    _position.y -= 1;
};

void Tile::SlideDown()
{
    _position.y += 1;
};

void Tile::SlideLeft()
{
    _position.x -= 1;
};

void Tile::SlideRight()
{
    _position.x += 1;
};

Vector2 Tile::GetPosition() const
{
    return _position;
}

float Tile::GetTileSize() const
{
    return _tileSize;
}

Vector2 Tile::GetTexCoords() const
{
    return _texcoords;
}

void Tile::SetPosition(const Vector2& newPos)
{
    _position = newPos;
}

bool Tile::IsCorrectPosition()
{
    return ((_position - _initPos).Length() < 0.001);
}
