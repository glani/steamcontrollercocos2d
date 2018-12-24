# Steam Controller Cocos2d demo implementation
![s01o]

- Clone repository
- Run **git submodule update --init --recursive**
- Create new project using cocos2d utils **cocos new MyGame -p com.MyCompany.MyGame -l cpp -d ./MyGame**
- Copy all files except **Classes**, **Resources**, **CMakeLists.txt** (if you use cocos2d 3.17)

## Using other versions of cocos2d different from 3.17
Under Linux and Windows section in **CMakeLists.txt** add following lines:

    #steamcontrollercocos
    add_definitions(-D_USE_STEAM)
    list(APPEND DEPEND_COMMON_LIBS steamcontrollercocos)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/steamcontrollercocos)

Add following dependency to cocos_build_app macro 
    
    cocos_build_app(${APP_NAME}
                APP_SRC "${APP_SRC}"
                DEPEND_COMMON_LIBS "cocos2d" "steamcontrollercocos"
                DEPEND_ANDROID_LIBS "cocos2d_android"
                )

Add additional include directory

    include_directories(
            ...
            external/steamcontrollercocos/
            ...
    )
    
    
[s01o]: Screenshots/001.png "Example Cocos2D Steam Controller Implememntation"

