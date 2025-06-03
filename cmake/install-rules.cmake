install(
    TARGETS sokobo_exe
    RUNTIME COMPONENT sokobo_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
