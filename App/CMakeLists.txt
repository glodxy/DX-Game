set(D3DAPP_INC
        D3DApp.h
        D3DUtil.h D3DShader.h D3DMesh.h D3DSourceManager.h D3DTexture.h GeometryGenerator.h Mouse.h Keyboard.h D3DSpriteList.h D3DRenderTarget.cpp D3DRenderTarget.h D3DSprite.cpp D3DSprite.h D3DAnim.cpp D3DAnim.h AnimManager.cpp AnimManager.h)

set(D3DAPP_SRC
        D3DApp.cpp
        D3DUtil.cpp D3DShader.cpp D3DMesh.cpp D3DSourceManager.cpp D3DTexture.cpp GeometryGenerator.cpp D3DSpriteList.cpp D3DRenderTarget.cpp D3DRenderTarget.h D3DSprite.cpp D3DSprite.h D3DAnim.cpp D3DAnim.h AnimManager.cpp AnimManager.h)


message("test")
message("${PROJECT_SOURCE_DIR}")


add_library(D3DApp STATIC ${D3DAPP_INC} ${D3DAPP_SRC})

target_link_libraries(D3DApp Effects11)

