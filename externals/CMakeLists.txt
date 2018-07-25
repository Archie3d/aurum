add_subdirectory(zlib-1.2.8)
add_subdirectory(jpeg-9a)
add_subdirectory(libpng-1.6.26)

add_subdirectory(vst3sdk)
set(SMTG_USE_STATIC_CRT ON CACHE BOOL "Force static CRT" FORCE)
set(SMTG_RUN_VST_VALIDATOR OFF CACHE BOOL "Run VST validator" FORCE)
set(SMTG_ADD_VST3_HOSTING_SAMPLES ON CACHE BOOL "VST3 hosting samples" FORCE)
set(SMTG_ADD_VST3_PLUGINS_SAMPLES OFF CACHE BOOL "VST3 plugins samples" FORCE)

set(VST_SDK_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/vst3sdk CACHE INTERNAL "VST SDK includes" FORCE)
set(VST_SDK_LIBS "base;sdk" CACHE INTERNAL "VST SDK libs" FORCE)
set(VST_GUI_LIBS "vstgui;vstgui_support;vstgui_uidescription" CACHE INTERNAL "VSG GUI libraries" FORCE)
set(VST_GUI_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/vst3sdk/vstgui4" CACHE INTERNAL "VST GUI includes" FORCE)

add_subdirectory(nana)
set(NANA_CMAKE_ENABLE_PNG ON CACHE BOOL "Enable PNG for nana" FORCE)
set(NANA_CMAKE_ENABLE_JPEG ON CACHE BOOL "Enable JPEF for nama" FORCE)
set(nana_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/nana/include CACHE INTERNAL "nana++ incudes" FORCE)
