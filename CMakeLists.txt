cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(MachineYukonSolitaire)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Our Project
# dean phar
add_executable(${PROJECT_NAME} Main.c
        Cardpile.h
        Card.c
        Card.h
        Cardpile.c
        MemoryManagement.c
        MemoryManagement.h
        Definitions.h
        GameUI.h
        GameUI.c
        GameInitialization.h
        GameInitialization.c
        Commands.h
        Commands.c

)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif ()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()