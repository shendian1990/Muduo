SET(BASE_SRCS event_loop.cc log4z.cc timestamp.cc channel.cc poller.cc)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/net)

ADD_LIBRARY(muduo_net ${BASE_SRCS})
TARGET_LINK_LIBRARIES(muduo_net pthread rt)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

ADD_SUBDIRECTORY(tests)