set(GOBJ_INC
        GameObject.h Cube.h Camera/Camera.h Light/DirectionLight.h Light/PointLight.h Light/SpotLight.h Light/BaseLight.h
        Plane.h Terrain.h Scene.h Sphere.cpp Sphere.h Water.cpp Water.h Game/Ball.cpp Game/Ball.h
        PhysicalThing/PhysicalWorld.h PhysicalThing/octree.h)

set(GOBJ_SRC
        GameObject.cpp Cube.cpp Camera/Camera.cpp Light/DirectionLight.cpp Light/PointLight.cpp Light/SpotLight.cpp Light/BaseLight.cpp
        Plane.cpp Terrain.cpp Scene.cpp Sphere.cpp Sphere.h Water.cpp Water.h Game/Ball.cpp Game/Ball.h
        PhysicalThing/PhysicalWorld.cpp PhysicalThing/octree.cpp)

add_library(GameObject STATIC ${GOBJ_INC} ${GOBJ_SRC})
