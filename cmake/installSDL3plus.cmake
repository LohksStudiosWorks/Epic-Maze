include(FetchContent)

FetchContent_Declare(
    sdl3_plus
    GIT_REPOSITORY  https://github.com/LohksStudiosWorks/SDL3_plus
    GIT_TAG         v1.0.2
)

FetchContent_MakeAvailable(sdl3_plus)