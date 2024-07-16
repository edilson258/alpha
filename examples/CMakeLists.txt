file(GLOB ExampleSources "*.c")
foreach(ExampleSource ${ExampleSources})
  get_filename_component(ExampleFileName ${ExampleSource} NAME_WE)
  add_executable(${ExampleFileName} ${ExampleSource})

  # link with alphahttp
  target_link_libraries(${ExampleFileName} alphahttp)
endforeach()
