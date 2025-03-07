cmake_minimum_required(VERSION 3.13)
project(aml-rtos_docs)

# set input and output files
set(SDK_BASE $ENV{SDK_BASE})
set(DOXYGEN_IN ${SDK_BASE}/arch/${ARCH_NAME}/docs/Doxyfile)
set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)

    # request to configure the file
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Generating SDK documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
