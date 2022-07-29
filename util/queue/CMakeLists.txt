set(HEADERS
    queue/RingQueue.hpp
    queue/LockQueue.hpp
    queue/CASQueue.hpp
   )

INSTALL(FILES ${HEADERS} DESTINATION include/util/queue)

