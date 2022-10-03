file(GLOB SRC_FILES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
)

target_sources(ProxyEstimateSystem PRIVATE 
    ${SRC_FILES}
)
