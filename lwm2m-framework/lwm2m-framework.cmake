# Provides LWM2M_FRAMEWORK_SOURCES_DIR, LWM2M_FRAMEWORK_SOURCES and LWM2M_FRAMEWORK_LIBRARIES variables.

set(LWM2M_FRAMEWORK_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(LWM2M_FRAMEWORK_SOURCES
    ${LWM2M_FRAMEWORK_SOURCES_DIR}/basic_lwm2m_framework.cpp
    )

set (HTTP_FRAMEWORK_LIBRARIES)
