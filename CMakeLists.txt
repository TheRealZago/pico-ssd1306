add_library(pico_oled
        oled.cpp
        ssd1306.cpp
        sh1106.cpp
        frameBuffer/FrameBuffer.cpp
        shapeRenderer/ShapeRenderer.cpp)

add_subdirectory(textRenderer)

target_link_libraries(pico_oled
        oled_textRenderer
        hardware_i2c
        pico_stdlib
        )
target_include_directories (pico_oled PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})