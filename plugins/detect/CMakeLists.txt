if (gperf)

include_directories(.)
add_module(detect NWNXDetect plugin-detect
)

set_target_properties(detect PROPERTIES COMPILE_FLAGS "-O0")

endif (gperf)
