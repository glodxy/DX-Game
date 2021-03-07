set(LIBRARY_INC
        include/d3dx11effect.h
        include/d3dxGlobal.h
        include/DDSTextureLoader.h
        include/WICTextureLoader.h
        include/DirectXTex.h
        include/DirectXTex.inl)

set(LIBRARY_SRC
        src/DDSTextureLoader.cpp
        src/WICTextureLoader.cpp)

add_library(Library STATIC ${LIBRARY_INC} ${LIBRARY_SRC})
target_link_libraries(Library Effects11 assimp-vc140-mt.lib DirectXTex.lib)