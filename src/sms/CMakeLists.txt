set(LIBRARY_NAME sms)
set(SOURCE_FILES
        SMS.h
        SMS.cpp
        Memory.h
        Memory.cpp
        Z80.h
        Z80.cpp
        Registers.h
        Z80_Opcodes.cpp
        )

add_library(${LIBRARY_NAME} SHARED STATIC ${SOURCE_FILES})

install(TARGETS ${LIBRARY_NAME} DESTINATION ${SOMOS_INSTALL_LIB_DIR})
install(FILES SMS.h DESTINATION ${SOMOS_INSTALL_INCLUDE_DIR})