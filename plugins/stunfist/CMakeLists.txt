if (gperf)

include_directories(.)
add_module(stunfist NWNXStunFist plugin-stunfist
	hooks/h_StunFistDC hooks/h_StunFistUse
)

set_target_properties(stunfist PROPERTIES COMPILE_FLAGS "-O0")

endif (gperf)
