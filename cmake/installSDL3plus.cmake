include(FetchContent)

FetchContent_Declare(
    sdl3_plus
    GIT_REPOSITORY  https://github.com/LohksStudiosWorks/SDL3_plus
    GIT_TAG         master
)

FetchContent_MakeAvailable(sdl3_plus)