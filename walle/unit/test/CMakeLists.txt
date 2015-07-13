
set(TEST_SRC
	test.cpp
	)
	add_executable(utest ${TEST_SRC})
	target_link_libraries(utest walleunit)