if (gperf)

include_directories(.)
add_module(stunfist NWNXStunFist plugin-stunfist
)

set_target_properties(stunfist PROPERTIES COMPILE_FLAGS "-O0")

endif (gperf)
