
set(TEST_SRC
	test
	)
	add_executable(utest ${TEST_SRC})
	target_link_libraries(utest walleunit)