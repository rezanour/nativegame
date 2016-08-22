#include "Windows.h"
#include <SpriteBatch.h>
#include <memory>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
  ID3D11DeviceContext* context = nullptr;
  std::shared_ptr<DirectX::SpriteBatch> spriteBatch(new DirectX::SpriteBatch(context));

  UNREFERENCED_PARAMETER(instance);
  return 0;
}
