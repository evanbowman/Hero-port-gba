cmake_minimum_required(VERSION 3.5)

project(HerocoreEngine)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(SOURCE_DIR ${ROOT_DIR}/source)
set(IMAGE_DIR ${ROOT_DIR}/images)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_language(ASM)



option(GAMEBOY_ADVANCE "GameboyAdvance" ON)
option(GBA_AUTOBUILD_IMG "AutobuildImg" OFF)
option(GBA_AUTOBUILD_CONF "AutobuildConf" OFF)


if(GAMEBOY_ADVANCE AND NOT DEVKITARM)
  message(WARNING "Note: GAMEBOY_ADVANCE option is ON by default.")
  message(FATAL_ERROR "GAMEBOY_ADVANCE option is set, but missing ARM toolchain.")
endif()


if(GAMEBOY_ADVANCE AND WIN32)
  message(FATAL_ERROR "Gameboy Advance builds not supported in windows")
endif()


include_directories(${SOURCE_DIR})
include_directories(${ROOT_DIR}/external/)


if(APPLE)
  set(EMACS_FLYCHECK_FRONTEND clang)
else()
  set(EMACS_FLYCHECK_FRONTEND gcc)
endif()

# For emacs flycheck users
configure_file(dirlocals.in ${SOURCE_DIR}/.dir-locals.el)



set(SOURCES
  # Engine
  ${SOURCE_DIR}/graphics/overlay.cpp
  ${SOURCE_DIR}/graphics/sprite.cpp
  ${SOURCE_DIR}/number/numeric.cpp
  ${SOURCE_DIR}/number/random.cpp
  ${SOURCE_DIR}/graphics/view.cpp
  ${SOURCE_DIR}/engine.cpp
  ${SOURCE_DIR}/object.cpp
  ${SOURCE_DIR}/maps.cpp
  ${SOURCE_DIR}/taggedObject.cpp
  ${SOURCE_DIR}/objects/hero.cpp
  ${SOURCE_DIR}/localization.cpp
  ${SOURCE_DIR}/memory/pool.cpp
  ${SOURCE_DIR}/filesystem.cpp
  ${SOURCE_DIR}/string.cpp
  ${SOURCE_DIR}/start.cpp)


set(FILES_DIR ${SOURCE_DIR}/../)
set(FILES "")
set(FILE_DECLS "")


macro(add_file prefix file suffix)

  set(FILES ${FILES} "\n    {\"${prefix}\", \"${file}.${suffix}\", file_${file}},\n//")
  set(FILE_DECLS ${FILE_DECLS}
    "\nextern const unsigned char file_${file}[];\n//")

  if(GBA_AUTOBUILD_CONF)

    set(FILE_IN ${FILES_DIR}/${prefix}/${file}.${suffix})
    set(FILE_OUT ${SOURCE_DIR}/data/file_${file}.cpp)

    add_custom_command(OUTPUT ${FILE_OUT}
      COMMAND echo "extern const unsigned char file_${file}[] = {" > ${FILE_OUT}
      COMMAND cat ${FILE_IN} | xxd -i | tee -a ${FILE_OUT} > /dev/null
      COMMAND echo ", 0x00}\;" | tee -a ${FILE_OUT}
      DEPENDS ${FILE_IN})

    add_custom_target(convert_${file}_file DEPENDS ${FILE_OUT})

    add_dependencies(HerocoreEngine convert_${file}_file)

  endif()
endmacro()


if(GAMEBOY_ADVANCE)

  set(DATA_DIR ${SOURCE_DIR}/data/)

  set(SOURCES
    ${SOURCES}
    ${SOURCE_DIR}/platform/gba/gba_platform.cpp
    ${SOURCE_DIR}/platform/gba/memcpy.s
    ${SOURCE_DIR}/platform/gba/interrupt.c
    ${SOURCE_DIR}/platform/gba/interrupt_dispatch.s
    ${SOURCE_DIR}/platform/gba/bios_math.s
    ${SOURCE_DIR}/platform/gba/reset.s

    # Data files below. Because the gameboy advance does not have a filesystem,
    # all of the sounds and images need to be compiled into the ROM.
    ${DATA_DIR}/boss.s
    ${DATA_DIR}/zone5.s
    ${DATA_DIR}/zone1.s
    ${DATA_DIR}/overlay.s
    ${DATA_DIR}/tile0.s
    ${DATA_DIR}/spritesheet.s
    ${DATA_DIR}/overlay_text_key.s
    ${DATA_DIR}/charset0.s
    ${DATA_DIR}/charset1.s
    ${DATA_DIR}/sound_msg.s)

else()
  set(SOURCES
    ${SOURCES}
    ${SOURCE_DIR}/platform/desktop/desktop_platform.cpp
    ${SOURCE_DIR}/platform/desktop/resource_path.cpp)
endif()


if(APPLE)
  add_executable(HerocoreEngine MACOSX_BUNDLE ${SOURCES})


else()
  add_executable(HerocoreEngine
    ${SOURCES})
endif()


set(IMAGE_INCLUDES "")
set(IMAGE_SPR_STUBS "")
set(IMAGE_TILE_STUBS "")
set(IMAGE_OVERLAY_STUBS "")

macro(add_spritesheet filename mw mh flatten)

  set(IMAGE_INCLUDES ${IMAGE_INCLUDES}
    "\n#include \"data/${filename}.h\"\n//")

  set(IMAGE_SPR_STUBS ${IMAGE_SPR_STUBS}
    "\n    TEXTURE_INFO(${filename}),\n//")

  compile_image(${filename} ${mw} ${mh} ${flatten})
endmacro()


macro(add_tilesheet filename mw mh flatten)
  if(${flatten} STREQUAL "YES")
    set(FLATTENED_SUFFIX "_flattened")
  else()
    set(FLATTENED_SUFFIX "")
  endif()

  set(IMAGE_INCLUDES ${IMAGE_INCLUDES}
    "\n#include \"data/${filename}${FLATTENED_SUFFIX}.h\"\n//")

  set(IMAGE_TILE_STUBS ${IMAGE_TILE_STUBS}
    "\n    TEXTURE_INFO(${filename}${FLATTENED_SUFFIX}),\n//")

  compile_image(${filename} ${mw} ${mh} ${flatten})
endmacro()


macro(add_overlay filename mw mh flatten)
  if(${flatten} STREQUAL "YES")
    set(FLATTENED_SUFFIX "_flattened")
  else()
    set(FLATTENED_SUFFIX "")
  endif()

  set(IMAGE_INCLUDES ${IMAGE_INCLUDES}
    "\n#include \"data/${filename}${FLATTENED_SUFFIX}.h\"\n//")

  set(IMAGE_OVERLAY_STUBS ${IMAGE_OVERLAY_STUBS}
    "\n    TEXTURE_INFO(${filename}${FLATTENED_SUFFIX}),\n//")

  compile_image(${filename} ${mw} ${mh} ${flatten})
endmacro()


function(compile_image filename mw mh flatten)
  if(${flatten} STREQUAL "YES")
    add_custom_command(OUTPUT ${SOURCE_DIR}/data/${filename}_flattened.s
      COMMAND python3 prep_image.py ${ROOT_DIR} ${filename}.png yes
      COMMAND ${DEVKITPRO}/tools/bin/grit tmp/${filename}_flattened.png -gB4 -gTFF00FF
      COMMAND mv ${filename}_flattened.s ${SOURCE_DIR}/data/${filename}_flattened.s
      COMMAND mv ${filename}_flattened.h ${SOURCE_DIR}/data/${filename}_flattened.h
      DEPENDS ${IMAGE_DIR}/${filename}.png)

    add_custom_target(compile_image_${filename} DEPENDS ${SOURCE_DIR}/data/${filename}_flattened.s)

  elseif(${mw} STREQUAL "0" AND ${mh} STREQUAL "0")
    add_custom_command(OUTPUT ${SOURCE_DIR}/data/${filename}.s
      COMMAND python3 prep_image.py ${ROOT_DIR} ${filename}.png no
      COMMAND ${DEVKITPRO}/tools/bin/grit  tmp/${filename}.png -gB4 -gTFF00FF
      COMMAND mv ${filename}.s ${SOURCE_DIR}/data/${filename}.s
      COMMAND mv ${filename}.h ${SOURCE_DIR}/data/${filename}.h
      DEPENDS ${IMAGE_DIR}/${filename}.png)

    add_custom_target(compile_image_${filename} DEPENDS ${SOURCE_DIR}/data/${filename}.s)

  else()
    add_custom_command(OUTPUT ${SOURCE_DIR}/data/${filename}.s
      COMMAND python3 prep_image.py ${ROOT_DIR} ${filename}.png no
      COMMAND ${DEVKITPRO}/tools/bin/grit tmp/${filename}.png -gB4 -Mw ${mw} -Mh ${mh} -gTFF00FF
      COMMAND mv ${filename}.s ${SOURCE_DIR}/data/${filename}.s
      COMMAND mv ${filename}.h ${SOURCE_DIR}/data/${filename}.h
      DEPENDS ${IMAGE_DIR}/${filename}.png)

    add_custom_target(compile_image_${filename} DEPENDS ${SOURCE_DIR}/data/${filename}.s)

  endif()

  add_dependencies(HerocoreEngine compile_image_${filename})
endfunction()


if(APPLE OR UNIX OR GAMEBOY_ADVANCE)

  set(SHARED_COMPILE_OPTIONS
    -g3
    -O2
    -Wall
    -Werror
    # Lua seems not to compile with the pedantic option, looks like the
    # interpreter is direct-threaded, for one thing.
    # -pedantic
    -nostdlib
    -ffast-math
    -fno-math-errno
    -Wdouble-promotion
    -Wno-pointer-arith
    -fomit-frame-pointer
    -D__BLINDJUMP_ENABLE_LOGS)

  if(GAMEBOY_ADVANCE)
    set(SHARED_COMPILE_OPTIONS
      ${SHARED_COMPILE_OPTIONS}
      -fno-exceptions)
  endif()

elseif(WIN32)

  set(SHARED_COMPILE_OPTIONS
    /W4)

  include_directories(${ROOT_DIR}/external/SFML-2.5.1/include/)

  set(SFML_LIB_DIR ${ROOT_DIR}/external/SFML-2.5.1/lib/Release)

  target_link_libraries(HerocoreEngine
    ${SFML_LIB_DIR}/sfml-graphics.lib
    ${SFML_LIB_DIR}/sfml-window.lib
    ${SFML_LIB_DIR}/sfml-system.lib
    ${SFML_LIB_DIR}/sfml-audio.lib
    ${SFML_LIB_DIR}/sfml-network.lib)

  add_custom_command(TARGET HerocoreEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIB_DIR}/sfml-graphics-2.dll"
        $<TARGET_FILE_DIR:HerocoreEngine>)

  add_custom_command(TARGET HerocoreEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIB_DIR}/sfml-audio-2.dll"
        $<TARGET_FILE_DIR:HerocoreEngine>)

  add_custom_command(TARGET HerocoreEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIB_DIR}/sfml-window-2.dll"
        $<TARGET_FILE_DIR:HerocoreEngine>)

  add_custom_command(TARGET HerocoreEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIB_DIR}/sfml-network-2.dll"
        $<TARGET_FILE_DIR:HerocoreEngine>)

  add_custom_command(TARGET HerocoreEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_LIB_DIR}/sfml-system-2.dll"
        $<TARGET_FILE_DIR:HerocoreEngine>)

   add_custom_command(TARGET HerocoreEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ROOT_DIR}/external/SFML-2.5.1/extlibs/bin/x64/openal32.dll"
        $<TARGET_FILE_DIR:HerocoreEngine>)
endif()


if(GAMEBOY_ADVANCE)

  # I am setting CMAKE_AR in the toolchain file, but for some reason, the
  # variable no longer exists within the cmakelists... which is strange, because
  # other variables set in the toolchain file seem to be persistent...
  set(CMAKE_AR $ENV{DEVKITARM}/bin/arm-none-eabi-ar)

  add_library(ARM_ROUTINES ${SOURCE_DIR}/platform/gba/gba_arm_routines.cpp)

  target_compile_options(ARM_ROUTINES PRIVATE
    ${SHARED_COMPILE_OPTIONS})

  target_compile_options(ARM_ROUTINES PRIVATE
    -marm
    -mlong-calls
    -mthumb-interwork
    -mcpu=arm7tdmi
    -mtune=arm7tdmi)

  if(GBA_AUTOBUILD_CONF)
    configure_file(files.cpp.in ${SOURCE_DIR}/platform/gba/files.cpp)
  endif()

  if(GBA_AUTOBUILD_IMG)
    add_overlay(overlay 0 0 NO)
    add_overlay(overlay_text_key 0 0 NO)
    add_overlay(charset0 0 0 NO)
    add_overlay(charset1 0 0 NO)
    add_tilesheet(tile0 0 0 NO)
    add_spritesheet(spritesheet 2 2 NO)

    configure_file(images.cpp.in ${SOURCE_DIR}/platform/gba/images.cpp)
  endif()

  set(CMAKE_EXE_LINKER_FLAGS
    "-mthumb -mthumb-interwork -Wl,-Map,HerocoreEngine.elf.map -specs=gba.specs"
    CACHE INTERNAL "" FORCE)

  target_link_libraries(HerocoreEngine PRIVATE
    # ${DEVKITPRO}/libgba/lib/libgba.a
    ARM_ROUTINES)

  target_compile_options(HerocoreEngine PRIVATE
    -mthumb
    -mthumb-interwork
    -mcpu=arm7tdmi
    -mtune=arm7tdmi)

  add_custom_command(TARGET HerocoreEngine POST_BUILD
    COMMENT "building cartridge"
    COMMAND ${DEVKITARM}/bin/arm-none-eabi-objcopy -O binary HerocoreEngine HerocoreEngine.gba
    COMMAND ${DEVKITPRO}/tools/bin/gbafix -therocore -cCBJE -r0 -mEB HerocoreEngine.gba)

  target_compile_options(HerocoreEngine PRIVATE
    -D__GBA__)

elseif(APPLE)
  target_link_libraries(HerocoreEngine
    "-framework sfml-window -framework sfml-graphics -framework sfml-system -framework sfml-audio -framework sfml-network -framework Cocoa")
  set_target_properties(HerocoreEngine
    PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")

  include_directories("/usr/local/include/")

  add_custom_target(pkg ALL mkdir -p HerocoreEngine.app/Contents/Resources
    COMMAND mkdir -p HerocoreEngine.app/Contents/Frameworks
    COMMAND cp -r ${ROOT_DIR}/images/ HerocoreEngine.app/Contents/images/
    COMMAND cp -r ${ROOT_DIR}/shaders/ HerocoreEngine.app/Contents/shaders/
    COMMAND cp -r ${ROOT_DIR}/sounds/ HerocoreEngine.app/Contents/sounds/
    COMMAND cp -r ${ROOT_DIR}/scripts/ HerocoreEngine.app/Contents/scripts/
    # COMMAND cp macOS/icon.icns HerocoreEngine.app/Contents/Resources
    # COMMAND cp -r ${SFML_DIR}/lib/* HerocoreEngine.app/Contents/Frameworks
    # COMMAND cp -r ${SFML_DIR}/extlibs/libs-osx/Frameworks/* HerocoreEngine.app/Contents/Frameworks
    )

elseif(UNIX)

  target_link_libraries(HerocoreEngine
    -lpthread
    -lsfml-window
    -lsfml-graphics
    -lsfml-system
    -lsfml-audio
    -lsfml-network)

endif()



target_compile_options(HerocoreEngine PRIVATE
  ${SHARED_COMPILE_OPTIONS})


file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${SOURCE_DIR}/*.hpp")

add_custom_target(clang-format
  COMMAND clang-format --style=file -i ${SOURCES} ${HEADERS}
  COMMENT "Running clang-format"
  VERBATIM)
