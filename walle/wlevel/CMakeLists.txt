set(LEVEL_SRC
	arena
	block
	block_builder
	bloom
	builder
	c
	cache
	coding
	comparator
	crc32c
	db_impl
	db_iter
	dbformat
	env
	env_posix
	filename
	filter_block
	format
	hash
	iterator
	log_reader
	log_writer
	logging
	memtable
	merger
	options
	port_posix
	repair
	status
	table
	table_builder
	table_cache
	two_level_iterator
	version_edit
	version_set
	write_batch
)

SET(HEADS
	c.h
	cache.h
	comparator.h
	db.h
	env.h
	filter_policy.h
	iterator.h
	options.h
	slice.h
	status.h
	table.h
	table_builder.h
	write_batch.h
)

	add_library(wallelevel ${LEVEL_SRC})
	target_link_libraries(wallelevel z rt pthread)

	install(TARGETS wallelevel DESTINATION lib)
	install(FILES ${HEADS} DESTINATION include/walle/wlevel)
add_subdirectory(test)