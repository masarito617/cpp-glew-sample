cmake_minimum_required(VERSION 3.20)
project(OpenGLTest)

set(CMAKE_CXX_STANDARD 14)
add_executable(OpenGLTest src/main.cpp src/Game.cpp src/Game.h src/Components/Component.cpp src/Components/Component.h src/Actors/Actor.cpp src/Actors/Actor.h src/Commons/Vector2.cpp src/Commons/Vector2.h src/Components/SpriteComponent.cpp src/Components/SpriteComponent.h src/Commons/Math.h src/Actors/Ship.cpp src/Actors/Ship.h src/Commons/VertexArray.cpp src/Commons/VertexArray.h src/Commons/Shader.cpp src/Commons/Shader.h src/Commons/Texture.cpp src/Commons/Texture.h)

# SDL2のパスを設定
# 自身の環境に合わせて書き換えるべし
set(SDL2_LIB_PATH "/usr/local/Cellar/sdl2/2.0.14_1/lib/libSDL2-2.0.0.dylib")
set(SDL2_IMAGE_LIB_PATH "/usr/local/Cellar/sdl2_image/2.0.5/lib/libSDL2_image-2.0.0.dylib")
set(GLEW_LIB_PATH "/usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib")
include_directories("/usr/local/Cellar/sdl2/2.0.14_1/include/SDL2")
include_directories("/usr/local/Cellar/sdl2_image/2.0.5/include/SDL2")
include_directories("/usr/local/Cellar/glew/2.2.0_1/include")

target_link_libraries(${PROJECT_NAME} ${SDL2_LIB_PATH} ${SDL2_IMAGE_LIB_PATH} ${GLEW_LIB_PATH})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
