function(add_expidus_compositor)
  cmake_parse_arguments(PARSED_ARGS
    ""
    "BINARY_NAME;APPLICATION_ID"
    "PLUGIN_BUNDLED_LIBRARIES"
    ${ARGN})

  if(NOT PARSED_ARGS_BINARY_NAME)
    message(FATAL_ERROR "Missing argument: BINARY_NAME")
  endif()

  if(NOT PARSED_ARGS_APPLICATION_ID)
    message(FATAL_ERROR "Missing argument: APPLICATION_ID")
  endif()

  include(ExpidusRuntimePackageSetup)
  set(BINARY_SOURCES "")
  set(INSTALL_BUNDLE_DATA_DIR "${CMAKE_INSTALL_PREFIX}/data")
  add_subdirectory(${FLUTTER_MANAGED_DIR})

  if(NOT UNIX)
    list(APPEND BINARY_SOURCES "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc")
    set(INSTALL_BUNDLE_LIB_DIR "${CMAKE_INSTALL_PREFIX}")
    set(BUILD_BUNDLE_DIR "$<TARGET_FILE_DIR:${PARSED_ARGS_BINARY_NAME}>")
    set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)
  else()
    set(INSTALL_BUNDLE_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
    set(BUILD_BUNDLE_DIR "${PROJECT_BINARY_DIR}/bundle")
    list(APPEND BINARY_SOURCES "${EXPIDUS_RUNTIME_DIR}/compositor/runner/linux/main.c")
    add_subdirectory("${EXPIDUS_RUNTIME_DIR}/compositor/linux" plugins/expidus_runtime_compositor)
  endif()

  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${BUILD_BUNDLE_DIR}" CACHE PATH "..." FORCE)
  endif()

  install(CODE "
    file(REMOVE_RECURSE \"${BUILD_BUNDLE_DIR}/\")
    " COMPONENT Runtime)

  add_executable(${PARSED_ARGS_BINARY_NAME} ${BINARY_SOURCES})
  apply_standard_settings(${PARSED_ARGS_BINARY_NAME})
  add_dependencies(${PARSED_ARGS_BINARY_NAME} flutter_assemble)

  set_target_properties(${PARSED_ARGS_BINARY_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/intermediates_do_not_run")

  if(UNIX)
    target_include_directories(${PARSED_ARGS_BINARY_NAME} INTERFACE "${EXPIDUS_RUNTIME_DIR}/compositor/linux/include")
    target_link_libraries(${PARSED_ARGS_BINARY_NAME} PRIVATE expidus_runtime_compositor)
    list(APPEND PLUGIN_BUNDLED_LIBRARIES $<TARGET_FILE:expidus_runtime_compositor>)
    list(APPEND PLUGIN_BUNDLED_LIBRARIES ${expidus_runtime_compositor_bundled_libraries})

    foreach(bundled_library ${PLUGIN_BUNDLED_LIBRARIES})
      install(FILES "${bundled_library}"
        DESTINATION "${INSTALL_BUNDLE_LIB_DIR}"
        COMPONENT Runtime)
    endforeach(bundled_library)
  elseif(WIN32)
    include("${FLUTTER_MANAGED_DIR}/generated_plugins.cmake")

    if(PLUGIN_BUNDLED_LIBRARIES)
      install(FILES "${PLUGIN_BUNDLED_LIBRARIES}"
        DESTINATION "${INSTALL_BUNDLE_LIB_DIR}"
        COMPONENT Runtime)
    endif()

    install(FILES "${FLUTTER_LIBRARY}" DESTINATION "${INSTALL_BUNDLE_LIB_DIR}" COMPONENT Runtime)
  endif()

  install(FILES "${FLUTTER_ICU_DATA_FILE}" DESTINATION "${INSTALL_BUNDLE_DATA_DIR}" COMPONENT Runtime)
  install(TARGETS ${PARSED_ARGS_BINARY_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}" COMPONENT Runtime) 

  set(FLUTTER_ASSET_DIR_NAME "flutter_assets")
  install(CODE "
    file(REMOVE_RECURSE \"${INSTALL_BUNDLE_DATA_DIR}/${FLUTTER_ASSET_DIR_NAME}\")
    " COMPONENT Runtime)
  install(DIRECTORY "${PROJECT_BUILD_DIR}/${FLUTTER_ASSET_DIR_NAME}" DESTINATION "${INSTALL_BUNDLE_DATA_DIR}" COMPONENT Runtime)

  if(WIN32)
    install(FILES "${AOT_LIBRARY}" DESTINATION "${INSTALL_BUNDLE_DATA_DIR}" CONFIGURATIONS Profile;Release COMPONENT Runtime)
  else()
    if(NOT CMAKE_BUILD_TYPE MATCHES "Debug")
      install(FILES "${AOT_LIBRARY}" DESTINATION "${INSTALL_BUNDLE_LIB_DIR}" COMPONENT Runtime)
    endif()
  endif()
endfunction()
