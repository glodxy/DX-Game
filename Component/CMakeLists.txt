set(COMPONENT_INC
        BaseComponent.h Mesh.h Transform.h CameraController.h SpriteList.h Sprite.cpp Sprite.h Collider/Collider.cpp Collider/Collider.h WaterController.cpp WaterController.h ModelMesh.cpp ModelMesh.h Animator.cpp Animator.h Game/BallController.cpp Game/BallController.h Game/CharacterController.cpp Game/CharacterController.h Game/Enemy.cpp Game/Enemy.h)

set(COMPONENT_SRC
        Mesh.cpp Transform.cpp CameraController.cpp SpriteList.cpp Sprite.cpp Sprite.h Collider/Collider.cpp Collider/Collider.h WaterController.cpp WaterController.h ModelMesh.cpp ModelMesh.h Animator.cpp Animator.h Game/BallController.cpp Game/BallController.h Game/CharacterController.cpp Game/CharacterController.h Game/Enemy.cpp Game/Enemy.h)

add_library(Component STATIC ${COMPONENT_INC} ${COMPONENT_SRC})