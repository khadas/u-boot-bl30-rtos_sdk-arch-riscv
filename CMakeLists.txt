aml_add_library()

aml_library_include_directories(
	${CMAKE_SOURCE_DIR}
	${SDK_BASE}/drivers/${ARCH}/common
	${SDK_BASE}/soc/${ARCH}
	${SDK_BASE}/soc/${ARCH}/${CONFIG_SOC_NAME}
	${SDK_BASE}/boards/${ARCH}/${BOARD}
	${SDK_BASE}/products/${PRODUCT}/include
)

aml_library_sources(
	start.S
	vtable.S
	handlers.c
	n200_func.c
	interrupt_control.c
)

aml_library_link_libraries(kernel__${KERNEL})

aml_link_libraries()
