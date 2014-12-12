
set(SOURCE_FILES
  window.cpp
    )

add_library(theui STATIC ${SOURCE_FILES})
target_link_libraries(theui ${LIBS})
install(TARGETS theui DESTINATION lib)

