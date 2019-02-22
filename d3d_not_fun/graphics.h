#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "eclwin.h"
#include <d3d11.h>

class Graphics
{
public:
    Graphics(HWND hWnd);
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    ~Graphics();

    void FlipAndPresentFrame();
    void ClearRenderTarget(float r, float g, float b) noexcept;
private:
    ID3D11Device* m_pDevice = nullptr;
    IDXGISwapChain* m_pSwap = nullptr;
    ID3D11DeviceContext* m_pContext = nullptr;
    ID3D11RenderTargetView* m_pTarget = nullptr;
};

#endif // !__GRAPHICS_H__
