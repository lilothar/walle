set(DB_SRC
	src/arena.cpp
	#src/block.cpp
	#src/block_builder.cpp
	src/bloom.cpp
	src/builder.cpp
	#src/c.cpp
	#src/cache.cpp
	src/coding.cpp
	#src/comparator.cpp
	#src/crc32c.cpp
	#src/db_impl.cpp
	#src/db_iter.cpp
	#src/dbformat.cpp
	src/env.cpp
	src/env_posix.cpp
	#src/filename.cpp
	#src/filter_block.cpp
	#src/filter_policy.cpp
	#src/format.cpp
	src/hash.cpp
	src/iterator.cpp
	#src/log_reader.cpp
	#src/log_writer.cpp
	#src/logging.cpp
	src/memtable.cpp
	src/merger.cpp
	src/options.cpp
	#src/port_posix.cpp
	src/status.cpp
	#src/table.cpp
	#src/table_builder.cpp
	#src/table_cache.cpp
	#src/two_level_iterator.cpp
	#src/version_edit.cpp
	#src/version_set.cpp
	#src/write_batch.cpp
)

add_library(walledb ${DB_SRC})
target_link_libraries(walledb pthread rt)
install(TARGETS walledb DESTINATION lib)
file(GLOB DBHEADERS "*.h")
install(FILES ${DBHEADERS} DESTINATION include/walle/storage)