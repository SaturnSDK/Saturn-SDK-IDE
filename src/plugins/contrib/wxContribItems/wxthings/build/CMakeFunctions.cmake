# ---------------------------------------------------------------------------
# This file contains a number of CMake functions/macros to make life easier.
# It can be called multiple times, but it will only be read the first time.
#
# Author:  John Labenski
# Created: Oct 15, 2011
#
# GET_CMAKE_CURRENT_LIST_DIR() - Get CMAKE_CURRENT_LIST_DIR var for CMake < 2.8.3
#
# HAS_WILDCARD()     - checks if input string has * or ? in it
# ADD_FILE_TO_LIST() - adds a file to a list after expanding wildcards and getting path
# GET_OBJECT_FILES() - returns a list of object files CMake would create for a list of source files.
#
# PRINT_ADD_SUBDIRECTORY() - Prints what paths ADD_SUBDIRECTORY() was called on
# PRINT_ADD_LIBRARY()      - Prints all ADD_LIBRARY() target names and their paths
# PRINT_ADD_EXECUTABLE()   - Prints all ADD_EXECTUABLE() target names and their paths
#
# ADD_SUBDIRECTORY() - overrides CMake's ADD_SUBDIRECTORY(), remembers what was added
# ADD_LIBRARY()      - overrides CMake's ADD_LIBRARY(), remembers what was added
# ADD_EXECUTABLE()   - overrides CMake's ADD_EXECUTABLE(), remembers what was added
# ADD_TEST()         - overrides CMake's ADD_TEST(), remembers what was added
# ADD_GTEST()        - Add a test using Google's GTest, tests for failure.
# ADD_CPPCHECK_TEST() - Add a static analysis test using cppcheck (if available)
#
# ADD_LIBRARY_FULL()    - Complete wrapper for ADD_LIBRARY() with many conveniences.
# ADD_EXECUTABLE_FULL() - Complete wrapper for ADD_EXECUTABLE() with many conveniences.
#
# ADD_TARGET_INCLUDE_PATHS() - Easily add multiple include paths to a single target.
# APPEND_TARGET_PROPERTIES() - Append instead of SET_TARGET_PROPERTIES()
# APPEND_SOURCE_FILES_PROPERTIES() - Append instead of SET_SOURCE_FILES_PROPERTIES()
# TARGET_PRECOMPILED_HEADER() - Set precomp header for a library or executable target.
# ADD_DOXYGEN() - Add a target for building Doxygen docs.
#
# GET_ALL_CMAKE_VARIABLES()   - Get all CMake vars into a list variable
# PRINT_ALL_CMAKE_VARIABLES() - Print all CMake vars, useful for debugging cmake scripts
# PRINT_NEW_CMAKE_VARIABLES() - Print new CMake vars since last call to PRINT_ALL/NEW_CMAKE_VARIABLES() call.
# ---------------------------------------------------------------------------

# ===========================================================================
# This file should only be called once during configuration, but the values
# are not cached so it needs to be be run for each configuration.
# Using properties does the trick of storing a variable per configuration run,
# accessible at all scopes like a cache var, but it is cleared each run.
# ===========================================================================

get_property(CMAKEFUNCTIONS_RUN_ONCE_CALLED DIRECTORY ${CMAKE_HOME_DIRECTORY}
             PROPERTY CMAKEFUNCTIONS_RUN_ONCE SET)

if (CMAKEFUNCTIONS_RUN_ONCE_CALLED)
    return()
endif()

set_property(DIRECTORY ${CMAKE_HOME_DIRECTORY}
             PROPERTY CMAKEFUNCTIONS_RUN_ONCE TRUE)

# ===========================================================================
# Helper functions
# ===========================================================================

MACRO( GET_CMAKE_CURRENT_LIST_DIR )
    # Backwards compat to CMake < 2.8.3
    if ("${CMAKE_CURRENT_LIST_DIR}" STREQUAL "")
        get_filename_component(CMAKE_CURRENT_LIST_DIR
                               ${CMAKE_CURRENT_LIST_FILE} PATH ABSOLUTE)
    endif()
ENDMACRO()

# ---------------------------------------------------------------------------
# Set bool variables IS_32_BIT and IS_64_BIT
# ---------------------------------------------------------------------------

if ((CMAKE_SIZEOF_VOID_P MATCHES 4) OR (CMAKE_CL_64 MATCHES 0))
    set(IS_32_BIT TRUE  CACHE INTERNAL "True if compiling for 32-bit.")
    set(IS_64_BIT FALSE CACHE INTERNAL "True if compiling for 64-bit.")
elseif((CMAKE_SIZEOF_VOID_P MATCHES 8) OR (CMAKE_CL_64 MATCHES 1))
    set(IS_32_BIT FALSE CACHE INTERNAL "True if compiling for 32-bit.")
    set(IS_64_BIT TRUE  CACHE INTERNAL "True if compiling for 64-bit.")
elseif(NOT DEFINED IS_32_BIT)
    # Sometimes CMake doesn't set CMAKE_SIZEOF_VOID_P, so we remember the last good value.
    # http://www.cmake.org/pipermail/cmake/2011-January/042058.html
    MESSAGE(WARNING "Oops, unable to determine if using 32 or 64 bit compilation, please rerun CMake.")
endif()

# ---------------------------------------------------------------------------
# Determine the number of processors on this machine
# ---------------------------------------------------------------------------

FUNCTION( FIND_PROCESSOR_COUNT )
    # only run the find code once and store it in the internal var PROCESSOR_COUNT_
    if(NOT DEFINED PROCESSOR_COUNT_)
        # Unknown:
        set(PROCESSOR_COUNT 1)

        # Linux:
        set(cpuinfo_file "/proc/cpuinfo")
        if(EXISTS "${cpuinfo_file}")
            file(STRINGS "${cpuinfo_file}" procs REGEX "^processor.: [0-9]+$")
            list(LENGTH procs PROCESSOR_COUNT)
        endif()

        # Mac:
        if(APPLE)
            find_program(cmd_sys_pro "system_profiler")
            if(cmd_sys_pro)
                execute_process(COMMAND ${cmd_sys_pro} OUTPUT_VARIABLE info)
                string(REGEX REPLACE "^.*Total Number Of Cores: ([0-9]+).*$" "\\1"
                       PROCESSOR_COUNT "${info}")
            endif()
        endif()

        # Windows:
        if(WIN32)
            set(PROCESSOR_COUNT "$ENV{NUMBER_OF_PROCESSORS}")
        endif()

        if ("${PROCESSOR_COUNT}" LESS "1")
            set(PROCESSOR_COUNT 1)
        endif()

        set(PROCESSOR_COUNT_ ${PROCESSOR_COUNT} CACHE INTERNAL "Number of CPUs found on this machine." FORCE)
    endif()

    # Copy the found # of processors to PROCESSOR_COUNT, in case someone changed it.
    set(PROCESSOR_COUNT ${PROCESSOR_COUNT_} CACHE STRING "Number of CPUs found on this machine." FORCE)
    mark_as_advanced( PROCESSOR_COUNT )

ENDFUNCTION()

# ---------------------------------------------------------------------------
# CONFIGURE_FILE_TO_ALL_BUILD_CONFIGURATION_DIRS(filename input_dir output_dir [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
#
# Configure a file from ${input_dir}/${filename}
#                    to ${output_dir}/[Debug/Release/...]/${filename}
#
# In MSVC: Configure a file to the ${output_dir}/[each of ${CMAKE_CONFIGURATION_TYPES}]
# In UNIX: Configure a file to the ${output_dir}/${CMAKE_BUILD_TYPE}
# ---------------------------------------------------------------------------

FUNCTION( CONFIGURE_FILE_TO_BUILD_CONFIGURATION_DIRS filename input_dir output_dir)
    if (MSVC)
        # Copy them to all the output dirs for msvc since you can change the build in MSVC GUI.
        # Note that CMAKE_CONFIGURATION_TYPES is empty if the generator doesn't support it
        foreach (t ${CMAKE_CONFIGURATION_TYPES})
            configure_file("${input_dir}/${filename}" "${output_dir}/${t}/${filename}" ${ARGN})
        endforeach()
    else() # MSW
        # Only a single build type in Unix, don't create other dirs and confuse people
        configure_file("${input_dir}/${filename}" "${output_dir}/${filename}" ${ARGN})
    endif()
ENDFUNCTION()

# ---------------------------------------------------------------------------
# REMOVE_FILE_FROM_ALL_BUILD_CONFIGURATION_DIRS(build_dir filename1 filename2 ...)
#
# Remove file(s) from ${build_dir}/[Debug/Release/RelWithDebInfo/MinSizeRel]/${filename}
#
# In MSVC: Remove file(s) from ${build_dir}/[each of ${CMAKE_CONFIGURATION_TYPES}]
# In UNIX: Remove file(s) from ${output_dir}/${CMAKE_BUILD_TYPE}
# ---------------------------------------------------------------------------

FUNCTION( REMOVE_FILE_FROM_BUILD_CONFIGURATION_DIRS build_dir )
    if (MSVC)
        # Note that CMAKE_CONFIGURATION_TYPES is empty if the generator doesn't support it
        foreach (a ${ARGN})
            foreach (t ${CMAKE_CONFIGURATION_TYPES})
                file(REMOVE "${build_dir}/${t}/${a}")
            endforeach()
        endforeach()
    else() # MSW
        # Only a single build type in Unix
        foreach (a ${ARGN})
            file(REMOVE "${build_dir}/${CMAKE_BUILD_TYPE}/${a}")
        endforeach()
    endif()
ENDFUNCTION()

FUNCTION( REMOVE_MATCHING_FILE_FROM_BUILD_CONFIGURATION_DIRS build_dir original_file )
    if (MSVC)
        # Note that CMAKE_CONFIGURATION_TYPES is empty if the generator doesn't support it
        foreach (a ${ARGN})
            foreach (t ${CMAKE_CONFIGURATION_TYPES})            
                execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files ${original_file} "${build_dir}/${t}/${a}"
                                RESULT_VARIABLE compare_result
                                OUTPUT_QUIET ERROR_QUIET)

                if (compare_result EQUAL 0)
                    file(REMOVE "${build_dir}/${t}/${a}")
                endif()
            endforeach()
        endforeach()
    else() # MSW
        # Only a single build type in Unix
        foreach (a ${ARGN})
            execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files ${original_file} "${build_dir}/${CMAKE_BUILD_TYPE}/${a}"
                            RESULT_VARIABLE file_is_not_the_same
                            OUTPUT_QUIET ERROR_QUIET)

            if (compare_result EQUAL 0)
                file(REMOVE "${build_dir}/${CMAKE_BUILD_TYPE}/${a}")
            endif()
        endforeach()
    endif()
ENDFUNCTION()

# ---------------------------------------------------------------------------
# HAS_WILDCARD( has_wild file_name )
#
# Set variable has_wild to TRUE if the file_name contains either '*' or '?'
# Usage : HAS_WILDCARD(has_wild "*.txt") ... if (has_wild) ...
# ---------------------------------------------------------------------------

FUNCTION( HAS_WILDCARD has_wild file_name )
    set(${has_wild} FALSE PARENT_SCOPE)
    string(REGEX MATCH "[*?]+" re_match ${file_name})

    if (re_match)
        set(${has_wild} TRUE PARENT_SCOPE)
    endif()
ENDFUNCTION()

# ---------------------------------------------------------------------------
# ADD_FILE_TO_LIST(file_list filename [ALLOW_MISSING])
#
# Append to the list the file(s) in filename which may be a list and/or contain wildcards.
# Note that CMake lists are simply items separated by a semicolon ';'
# If the filename exists or is wild the path is expanded using file(GLOB ...)
# A warning is given if a file is specified that is not on disk unless
#   ALLOW_MISSING is specified.
#
# Usage :
#  ADD_FILE_TO_LIST(myfiles "*.txt")       ; myfiles = "/path/to/a.txt;/path/to/b.txt;..."
#  ADD_FILE_TO_LIST(myfiles "c.txt;d.jpg") ; myfiles = "/path/to/a.txt;/path/to/b.txt;...;/path/to/c.txt;/path/to/d.jpg"
# ---------------------------------------------------------------------------

FUNCTION( ADD_FILE_TO_LIST file_list filename )

    set( _ALLOW_MISSING FALSE )

    foreach(arg ${ARGN})
        if ("${arg}" STREQUAL "ALLOW_MISSING")
            set(_ALLOW_MISSING TRUE)
        else ()
            message(FATAL_ERROR "Unknown parameter '${arg}', only ALLOW_MISSING is allowed.")
        endif()
    endforeach()

    # Backwards compat to CMake <= 2.8.4
    if ("${CMAKE_CURRENT_LIST_DIR}" STREQUAL "")
        get_filename_component(CMAKE_CURRENT_LIST_DIR
                               ${CMAKE_CURRENT_LIST_FILE} PATH ABSOLUTE)
    endif()

    foreach( f ${filename})
        set(filenames )
        HAS_WILDCARD(is_wild "${f}")

        if (is_wild)
            # Expand the path and handle wildcards.
            # Note "*.cxx;*.cpp" doesn't work so we have to do one at a time.
            file( GLOB filenames ${f} )
            set(${file_list} ${${file_list}} ${filenames} PARENT_SCOPE)
        elseif ((EXISTS ${f}) OR (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${f}"))
            # Cleanup the path
            file( GLOB filenames ${f} )
            set(${file_list} ${${file_list}} ${filenames} PARENT_SCOPE)
        elseif (_ALLOW_MISSING)
            # Just add the file which doesn't exist right now
            set(${file_list} ${${file_list}} ${f} PARENT_SCOPE)
        else()
            # warn, then add the file which doesn't exist right now anyway
            message(WARNING "File doesn't exist on disk : '${f}' from dir '${CMAKE_CURRENT_LIST_DIR}'")
            set(${file_list} ${${file_list}} ${f} PARENT_SCOPE)
        endif()
    endforeach()
ENDFUNCTION()



# ===========================================================================
# Replace some CMake functions to be able to track the targets
# that will be in the generated Makefiles
# ===========================================================================

UNSET(ADD_SUBDIRECTORY_LIST             CACHE)
UNSET(ADD_LIBRARY_LIST                  CACHE)
UNSET(ADD_LIBRARY_DIR_LIST              CACHE)
UNSET(ADD_LIBRARY_DIR_NAME_LIST         CACHE)
UNSET(ADD_EXECUTABLE_LIST               CACHE)
UNSET(ADD_EXECUTABLE_DIR_LIST           CACHE)
UNSET(ADD_EXECUTABLE_DIR_NAME_LIST      CACHE)
UNSET(ADD_TEST_LIST                     CACHE)
UNSET(ADD_TEST_DIR_LIST                 CACHE)
UNSET(ADD_TEST_DIR_NAME_LIST            CACHE)
UNSET(ADD_CPPCHECK_TEST_LIST            CACHE)
UNSET(ADD_CPPCHECK_TEST_DIR_LIST        CACHE)
UNSET(ADD_CPPCHECK_TEST_DIR_NAME_LIST   CACHE)

# ---------------------------------------------------------------------------
# PRINT_TARGET() - Print all the information about the target.
# ---------------------------------------------------------------------------
FUNCTION( PRINT_TARGET target_name )

    message( STATUS "----------------------------------------------------------------------------")
    message( STATUS "- Printing all information about target : ${target_name}")
    message( STATUS "----------------------------------------------------------------------------")

    set(_CONFIGS
        Release
        Debug
        RelWithDebInfo
        MinSizeRel
    )

    set(VAR_POSTFIXES
        _BINARY_DIR
        _LIB_DEPENDS
        _SOURCE_DIR

        _ROOT_DIR
        _SETUP_INCLUDE_PATH

        # These are generated by ADD_LIBRARY/EXECUTABLE_FULL()
        _CXX_FLAGS
        _CXX_FLAGS_<CONFIG>
        _DIRECTORY
        _HEADER_FILES
        _OBJECT_FILES
        _SOURCE_FILES
    )

    # get the flags if they haven't already
    if (NOT DEFINED ${target_name}_CXX_FLAGS)
        DO_GET_TARGET_FLAGS(${target_name})
    endif()

    foreach(var_postfix ${VAR_POSTFIXES})
        foreach(c ${_CONFIGS})
            string(REPLACE "<CONFIG>" "${c}" var_postfix2 "${var_postfix}")

            if (DEFINED ${target_name}${var_postfix2})
                MESSAGE(STATUS " - ${target_name}${var_postfix2} = ${${target_name}${var_postfix2}}")
            endif()

            # We do it this way since foreach() won't even be run once for an "" empty string
            if ("${var_postfix}" STREQUAL "${var_postfix2}")
                 break()
            endif()
        endforeach()
    endforeach()

    set(_TARGET_PROPS
        #<CONFIG>_OUTPUT_NAME deprecated
        #<CONFIG>_POSTFIX
        ARCHIVE_OUTPUT_DIRECTORY
        ARCHIVE_OUTPUT_DIRECTORY_<CONFIG>
        ARCHIVE_OUTPUT_NAME
        ARCHIVE_OUTPUT_NAME_<CONFIG>
        AUTOMOC
        AUTOMOC_MOC_OPTIONS
        BUILD_WITH_INSTALL_RPATH
        BUNDLE
        BUNDLE_EXTENSION
        COMPILE_DEFINITIONS
        COMPILE_DEFINITIONS_<CONFIG>
        COMPILE_FLAGS
        DEBUG_POSTFIX
        DEFINE_SYMBOL
        ENABLE_EXPORTS
        EXCLUDE_FROM_ALL
        EchoString
        FOLDER
        FRAMEWORK
        Fortran_FORMAT
        Fortran_MODULE_DIRECTORY
        GENERATOR_FILE_NAME
        GNUtoMS
        HAS_CXX
        IMPLICIT_DEPENDS_INCLUDE_TRANSFORM
        IMPORTED
        IMPORTED_CONFIGURATIONS
        IMPORTED_IMPLIB
        IMPORTED_IMPLIB_<CONFIG>
        IMPORTED_LINK_DEPENDENT_LIBRARIES
        IMPORTED_LINK_DEPENDENT_LIBRARIES_<CONFIG>
        IMPORTED_LINK_INTERFACE_LANGUAGES
        IMPORTED_LINK_INTERFACE_LANGUAGES_<CONFIG>
        IMPORTED_LINK_INTERFACE_LIBRARIES
        IMPORTED_LINK_INTERFACE_LIBRARIES_<CONFIG>
        IMPORTED_LINK_INTERFACE_MULTIPLICITY
        IMPORTED_LINK_INTERFACE_MULTIPLICITY_<CONFIG>
        IMPORTED_LOCATION
        IMPORTED_LOCATION_<CONFIG>
        IMPORTED_NO_SONAME
        IMPORTED_NO_SONAME_<CONFIG>
        IMPORTED_SONAME
        IMPORTED_SONAME_<CONFIG>
        IMPORT_PREFIX
        IMPORT_SUFFIX
        INSTALL_NAME_DIR
        INSTALL_RPATH
        INSTALL_RPATH_USE_LINK_PATH
        INTERPROCEDURAL_OPTIMIZATION
        INTERPROCEDURAL_OPTIMIZATION_<CONFIG>
        LABELS
        LIBRARY_OUTPUT_DIRECTORY
        LIBRARY_OUTPUT_DIRECTORY_<CONFIG>
        LIBRARY_OUTPUT_NAME
        LIBRARY_OUTPUT_NAME_<CONFIG>
        LINKER_LANGUAGE
        LINK_DEPENDS
        LINK_FLAGS
        LINK_FLAGS_<CONFIG>
        LINK_INTERFACE_LIBRARIES
        LINK_INTERFACE_LIBRARIES_<CONFIG>
        LINK_INTERFACE_MULTIPLICITY
        LINK_INTERFACE_MULTIPLICITY_<CONFIG>
        LINK_SEARCH_END_STATIC
        LINK_SEARCH_START_STATIC
        LOCATION
        LOCATION_<CONFIG>
        MACOSX_BUNDLE
        MACOSX_BUNDLE_INFO_PLIST
        MACOSX_FRAMEWORK_INFO_PLIST
        MAP_IMPORTED_CONFIG_<CONFIG>
        OSX_ARCHITECTURES
        OSX_ARCHITECTURES_<CONFIG>
        OUTPUT_NAME
        OUTPUT_NAME_<CONFIG>
        POST_INSTALL_SCRIPT
        PREFIX
        PRE_INSTALL_SCRIPT
        PRIVATE_HEADER
        PROJECT_LABEL
        PUBLIC_HEADER
        RESOURCE
        RULE_LAUNCH_COMPILE
        RULE_LAUNCH_CUSTOM
        RULE_LAUNCH_LINK
        RUNTIME_OUTPUT_DIRECTORY
        RUNTIME_OUTPUT_DIRECTORY_<CONFIG>
        RUNTIME_OUTPUT_NAME
        RUNTIME_OUTPUT_NAME_<CONFIG>
        SKIP_BUILD_RPATH
        SOURCES
        SOVERSION
        STATIC_LIBRARY_FLAGS
        STATIC_LIBRARY_FLAGS_<CONFIG>
        SUFFIX
        TYPE
        VERSION
        VS_DOTNET_REFERENCES
        #VS_GLOBAL_<variable>
        VS_GLOBAL_KEYWORD
        VS_GLOBAL_PROJECT_TYPES
        VS_KEYWORD
        VS_SCC_AUXPATH
        VS_SCC_LOCALPATH
        VS_SCC_PROJECTNAME
        VS_SCC_PROVIDER
        WIN32_EXECUTABLE
        #XCODE_ATTRIBUTE_<an-attribute>
    )

    message(STATUS " - TARGET PROPERTIES")

    foreach(p ${_TARGET_PROPS})
        foreach(c ${_CONFIGS})
            string(REPLACE "<CONFIG>" "${c}" p2 "${p}")

            get_target_property(_${p2} ${target_name} "${p2}")
            if(NOT "_${p2}" MATCHES "-NOTFOUND$")
                message(STATUS " - - ${p2} = ${_${p2}}")
            endif()

            # We do it this way since foreach() won't even be run once for an "" empty string
            if ("${p}" STREQUAL "${p2}")
                 break()
            endif()
        endforeach()
    endforeach()

    message( STATUS "----------------------------------------------------------------------------")
    message( STATUS "- End printing all information about target : ${target_name}")
    message( STATUS "----------------------------------------------------------------------------")
    MESSAGE( STATUS " ")
ENDFUNCTION()

# ---------------------------------------------------------------------------
# PRINT_ADD_SUBDIRECTORY(VERBOSE) - Print the ADD_SUBDIRECTORY() projects or their count to the console.
# ---------------------------------------------------------------------------
FUNCTION( PRINT_ADD_SUBDIRECTORY VERBOSE )
    if (VERBOSE)
        MESSAGE( STATUS "* Subdirectories that contain projects from ADD_SUBDIRECTORY():" )

        SET(ADD_SUBDIRECTORY_LIST_SORTED ${ADD_SUBDIRECTORY_LIST})
        LIST(SORT ADD_SUBDIRECTORY_LIST_SORTED)
        STRING(REGEX REPLACE ";" "\n* - " ADD_SUBDIRECTORY_LIST_STR "${ADD_SUBDIRECTORY_LIST_SORTED}")
        MESSAGE( STATUS "* - ${ADD_SUBDIRECTORY_LIST_STR}")
    endif()

    list(LENGTH ADD_SUBDIRECTORY_LIST ADD_SUBDIRECTORY_LIST_LENGTH)
    MESSAGE( STATUS "* - Number of subdirectories: ${ADD_SUBDIRECTORY_LIST_LENGTH}")
ENDFUNCTION()

# ---------------------------------------------------------------------------
# PRINT_ADD_LIBRARY(VERBOSE) - Print the ADD_LIBRARY() projects or their count to the console.
# ---------------------------------------------------------------------------
FUNCTION( PRINT_ADD_LIBRARY VERBOSE )
    if (VERBOSE)
        MESSAGE( STATUS "* Library targets that can be built from ADD_LIBRARY():" )

        SET(ADD_LIBRARY_DIR_NAME_LIST_SORTED ${ADD_LIBRARY_DIR_NAME_LIST})
        LIST(SORT ADD_LIBRARY_DIR_NAME_LIST_SORTED)
        STRING(REGEX REPLACE ";" "\n* - " ADD_LIBRARY_DIR_NAME_LIST_SORTED_STR "${ADD_LIBRARY_DIR_NAME_LIST_SORTED}")
        MESSAGE( STATUS "* - ${ADD_LIBRARY_DIR_NAME_LIST_SORTED_STR}")
    endif()
    list(LENGTH ADD_LIBRARY_DIR_NAME_LIST ADD_LIBRARY_DIR_NAME_LIST_LENGTH)
    MESSAGE( STATUS "* - Number of libraries: ${ADD_LIBRARY_DIR_NAME_LIST_LENGTH}")
ENDFUNCTION()

# ---------------------------------------------------------------------------
# PRINT_ADD_EXECUTABLE(VERBOSE) - Print the ADD_EXECUTABLE() projects or their count to the console.
# ---------------------------------------------------------------------------
FUNCTION( PRINT_ADD_EXECUTABLE VERBOSE )
    if (VERBOSE)
        MESSAGE( STATUS "* Executable targets that can be built from ADD_EXECUTABLE():" )

        SET(ADD_EXECUTABLE_DIR_NAME_LIST_SORTED ${ADD_EXECUTABLE_DIR_NAME_LIST})
        LIST(SORT ADD_EXECUTABLE_DIR_NAME_LIST_SORTED)
        STRING(REGEX REPLACE ";" "\n* - " ADD_EXECUTABLE_DIR_NAME_LIST_SORTED_STR "${ADD_EXECUTABLE_DIR_NAME_LIST_SORTED}")
        MESSAGE( STATUS "* - ${ADD_EXECUTABLE_DIR_NAME_LIST_SORTED_STR}")
    endif()
    list(LENGTH ADD_EXECUTABLE_DIR_NAME_LIST ADD_EXECUTABLE_DIR_NAME_LIST_LENGTH)
    MESSAGE( STATUS "* - Number of executables: ${ADD_EXECUTABLE_DIR_NAME_LIST_LENGTH}")
ENDFUNCTION()

# ---------------------------------------------------------------------------
# Replace the ADD_SUBDIRECTORY(source_dir) function to remember what
# subdirectories were added so we can do things with them.
# ---------------------------------------------------------------------------
FUNCTION( ADD_SUBDIRECTORY source_dir )

    # Get path from root CMake directory to where we are now
    STRING(REGEX REPLACE "${CMAKE_HOME_DIRECTORY}[/]*" "" PROJ_DIR_TEMP ${CMAKE_CURRENT_SOURCE_DIR})

    IF (PROJ_DIR_TEMP)
        SET(PROJ_DIR_TEMP "${PROJ_DIR_TEMP}/")
    ENDIF()

    # The message from ADD_SUBDIRECTORY() is very cryptic we need to show
    # people where the problem came from
    IF (IS_ABSOLUTE ${source_dir})
        IF (EXISTS "${source_dir}" )
            SET(ADD_SUBDIRECTORY_LIST ${ADD_SUBDIRECTORY_LIST} "${source_dir}" CACHE INTERNAL "")
            _ADD_SUBDIRECTORY(${source_dir} ${ARGN})
        ELSE()
            MESSAGE(WARNING "Missing directory in ADD_SUBDIRECTORY(${source_dir})")
        ENDIF()
    ELSE()
        IF (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}")
            SET(ADD_SUBDIRECTORY_LIST ${ADD_SUBDIRECTORY_LIST} "${PROJ_DIR_TEMP}${source_dir}" CACHE INTERNAL "")
            _ADD_SUBDIRECTORY(${source_dir} ${ARGN})
        ELSE()
            MESSAGE(WARNING "Missing directory in ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/${source_dir})")
        ENDIF()
    ENDIF()
ENDFUNCTION()

# ---------------------------------------------------------------------------
# Replace the ADD_LIBRARY(name ...) function to remember what
# library targets were added so we can do things with them.
# ---------------------------------------------------------------------------
FUNCTION( ADD_LIBRARY name)
    _ADD_LIBRARY(${name} ${ARGN})

    # Get the last part of the directory we're currently in
    STRING(REPLACE "${CMAKE_HOME_DIRECTORY}/" "" PROJ_DIR_TEMP ${CMAKE_CURRENT_SOURCE_DIR})

    SET(ADD_LIBRARY_LIST          ${ADD_LIBRARY_LIST}          "${name}" CACHE INTERNAL "")
    SET(ADD_LIBRARY_DIR_LIST      ${ADD_LIBRARY_DIR_LIST}      "${PROJ_DIR_TEMP}" CACHE INTERNAL "")
    SET(ADD_LIBRARY_DIR_NAME_LIST ${ADD_LIBRARY_DIR_NAME_LIST} "${PROJ_DIR_TEMP}/${name}" CACHE INTERNAL "")
ENDFUNCTION()

# ---------------------------------------------------------------------------
# Replace the ADD_EXECUTABLE(name ...) function to remember what
# executable targets were added so we can do things with them.
# ---------------------------------------------------------------------------
FUNCTION( ADD_EXECUTABLE name)
    _ADD_EXECUTABLE(${name} ${ARGN})

    # Get the last part of the directory we're currently in
    STRING(REPLACE "${CMAKE_HOME_DIRECTORY}/" "" PROJ_DIR_TEMP ${CMAKE_CURRENT_SOURCE_DIR})

    SET(ADD_EXECUTABLE_LIST          ${ADD_EXECUTABLE_LIST}          "${name}" CACHE INTERNAL "")
    SET(ADD_EXECUTABLE_DIR_LIST      ${ADD_EXECUTABLE_DIR_LIST}      "${PROJ_DIR_TEMP}" CACHE INTERNAL "")
    SET(ADD_EXECUTABLE_DIR_NAME_LIST ${ADD_EXECUTABLE_DIR_NAME_LIST} "${PROJ_DIR_TEMP}/${name}" CACHE INTERNAL "")
ENDFUNCTION()

# ---------------------------------------------------------------------------
# Replace the ADD_TEST(name ...) function to remember what
# test targets were added so we can do things with them.
# ---------------------------------------------------------------------------
FUNCTION( ADD_TEST )
    _ADD_TEST(${ARGN})

    list(GET ARGN 0 ARG0)
    list(GET ARGN 1 ARG1)

    # ADD_TEST's name can be either the first or second parameter
    if ("${ARG0}" STREQUAL "NAME")
        set(name "${ARG1}")
    else()
        set(name "${ARG0}")
    endif()

    # Get the last part of the directory we're currently in
    STRING(REPLACE "${CMAKE_HOME_DIRECTORY}/" "" PROJ_DIR_TEMP ${CMAKE_CURRENT_SOURCE_DIR})

    SET(ADD_TEST_LIST          ${ADD_TEST_LIST}          "${name}" CACHE INTERNAL "")
    SET(ADD_TEST_DIR_LIST      ${ADD_TEST_DIR_LIST}      "${PROJ_DIR_TEMP}" CACHE INTERNAL "")
    SET(ADD_TEST_DIR_NAME_LIST ${ADD_TEST_DIR_NAME_LIST} "${PROJ_DIR_TEMP}/${name}" CACHE INTERNAL "")
ENDFUNCTION()

# ---------------------------------------------------------------------------
# ADD_GTEST(exe_target_name)
# Add a function to create an ADD_TEST() that properly checks the output
# of Google GTests. Use this function instead of ADD_TEST for running GTests in your exe.
# Usage: ADD_GTEST( existing_exe_target_name [optional args to exe] )
# ---------------------------------------------------------------------------

FUNCTION( ADD_GTEST exe_target_name )
    #ADD_TEST(${exe_target_name} ${CMAKE_CURRENT_BINARY_DIR}/${exe_target_name} ${ARGN})

    get_target_property(EXE_FILEPATH ${exe_target_name} LOCATION)
    get_filename_component(EXE_PATH ${EXE_FILEPATH} PATH ABSOLUTE)
    get_filename_component(EXE_NAME ${EXE_FILEPATH} NAME_WE)

    string(REPLACE "$(OutDir)" "" EXE_PATH "${EXE_PATH}")

    ADD_TEST(NAME "${exe_target_name}"
             WORKING_DIRECTORY ${EXE_PATH}
             COMMAND ${EXE_NAME})
             #WORKING_DIRECTORY $<TARGET_FILE_DIR:${exe_target_name}>
             #COMMAND $<TARGET_FILE:${exe_target_name}> )

    SET_TESTS_PROPERTIES( "${exe_target_name}" PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED" )
ENDFUNCTION()

# ---------------------------------------------------------------------------
# ADD_CPPCHECK_TEST( name )
# Add a function to call cppcheck to do a static analysis test on the project
# source dir and its children dirs using the cppcheck program.
# This is probably a Linux only test and the target will not be generated if
# you don't have cppcheck installed.
# ---------------------------------------------------------------------------

FUNCTION( ADD_CPPCHECK_TEST name )
    IF ( "${cppcheck}" STREQUAL "")
        # Find the program cppcheck; ${cppcheck} will be either the exe path or "cppcheck-NOTFOUND"
        FIND_PROGRAM( cppcheck "cppcheck" )
        mark_as_advanced(cppcheck) # we don't need to see this

        IF ( "${cppcheck}" STREQUAL "cppcheck-NOTFOUND" )
            MESSAGE( STATUS "* WARNING: cppcheck NOT found, NOT generating ADD_CPPCHECK_TEST() tests" )
        ELSE()
            MESSAGE( STATUS "* cppcheck found, generating ADD_CPPCHECK_TEST() tests" )
        ENDIF()
    ENDIF()

    # Do not add the test if cppcheck is not found
    IF ( NOT "${cppcheck}" STREQUAL "cppcheck-NOTFOUND" )
        # Get the last part of the directory we're currently in
        STRING(REPLACE "${CMAKE_HOME_DIRECTORY}/" "" PROJ_DIR_TEMP ${CMAKE_CURRENT_SOURCE_DIR})

        LIST(FIND ADD_CPPCHECK_TEST_DIR_LIST ${PROJ_DIR_TEMP} PROJ_DIR_IDX_TEMP)

        IF ("${PROJ_DIR_IDX_TEMP}" GREATER "-1")
            MESSAGE(STATUS "* WARNING: Duplicate cppcheck tests for dir ${CMAKE_CURRENT_SOURCE_DIR} with names ${name}, you need only one ADD_CPPCHECK_TEST per directory.")
        ELSE()
            ADD_TEST( ${name} ${cppcheck} "--verbose" "--quiet" ${CMAKE_CURRENT_SOURCE_DIR} )
            SET_TESTS_PROPERTIES( ${name} PROPERTIES FAIL_REGULAR_EXPRESSION "(error)")

            SET(ADD_CPPCHECK_TEST_LIST          ${ADD_CPPCHECK_TEST_LIST}          "${name}" CACHE INTERNAL "")
            SET(ADD_CPPCHECK_TEST_DIR_LIST      ${ADD_CPPCHECK_TEST_DIR_LIST}      "${PROJ_DIR_TEMP}" CACHE INTERNAL "")
            SET(ADD_CPPCHECK_TEST_DIR_NAME_LIST ${ADD_CPPCHECK_TEST_DIR_NAME_LIST} "${PROJ_DIR_TEMP}/${name}" CACHE INTERNAL "")
        ENDIF()
    ENDIF()
ENDFUNCTION()

# ---------------------------------------------------------------------------
# Make ADD_LIBRARY() a little easier to read by putting together the
# necessary calls into one function.
# Usage:
#   ADD_LIBRARY_FULL( target_name [SHARED] [STATIC] [MODULE] [CUDA] [EXCLUDE_FROM_ALL] [ALLOW_MISSING_FILES]
#       HEADERS                                    # Can use wildcards like "*.h" or "" for no files
#           "include/foo/*.h" "include/foo/*.hpp"  # or specify the file names exactly.
#       SOURCES                                    # You must specify at least one valid source file.
#           "src/a.cpp" "src/b.cpp"                #
#      [PRECOMPILED_INCLUDE] foo/precomp.h         # The path to precomp header as used exactly in #include "foo/precomp.h"
#      [PRECOMPILED_HEADER]  include/foo/precomp.h # Path to the precomp header file.
#      [PRECOMPILED_SOURCE]  src/precomp.cpp       # Path to the precomp cpp file that only includes the header
#      [PRECOMPILED_SOURCES] src/a.cpp             # Full path to cpp files that include the precomp header.
#      [PRECOMPILED_FORCED]                        # Force including the PRECOMPILED_INCLUDE by the compiler.
#       LINK_LIBRARIES                             # Replaces TARGET_LINK_LIBRARIES(target_name lib1 lib2 ...)
#           gtest ${Boost_LIBRARIES}
#       PROPERTIES FOLDER "Foo/Bar"                # replaces SET_TARGET_PROPERTIES(target_name PROPERTIES ...)
#       PROPERTIES LINKER_LANGUAGE CXX ...         # use PROPERTIES XXX YYY as many times as you like
#       INSTALL "${ADD_LIBRARY_FULL_INSTALL_DEFAULT}" # INSTALL(TARGETS ${target_name} ${ADD_LIBRARY_FULL_INSTALL_DEFAULT})
#                                                  # is called with the parameters expanded into a list.
#                                                  # Create list as shown below or use the default flags as shown here.
#   )
#
# For the 'INSTALL' command create a variable like this to use:
#   SET( MY_INSTALL_FLAG_LIST
#        EXPORT \${target_name}_exports
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib )
#
# Internal cached variables will be created:
#   ${target_name}_HEADER_FILES - List of HEADERS given above
#   ${target_name}_SOURCE_FILES - List of SOURCES given above
#   ${target_name}_OBJECT_FILES - List of object files CMake will create compiling SOURCES
#
# ---------------------------------------------------------------------------

# These are flags for a default "normal" install for a ADD_LIBRARY_FULL() target.
set(ADD_LIBRARY_FULL_INSTALL_DEFAULT
    EXPORT \${target_name}_exports
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib )

function( ADD_LIBRARY_FULL target_name )
    set(_SHARED)                   # ADD_LIBRARY's SHARED directive
    set(_STATIC)                   # ADD_LIBRARY's STATIC directive
    set(_MODULE)                   # ADD_LIBRARY's MODULE directive
    set(_CUDA FALSE)               # Use CUDA_ADD_LIBRARY() not ADD_LIBRARY()
    set(_EXCLUDE_FROM_ALL)         # ADD_LIBRARY's EXCLUDE_FROM_ALL directive
    set(_ALLOW_MISSING_FILES)      # Don't warn on missing HEADERS or SOURCES files on disk
    set(_HEADERS)                  # ADD_LIBRARY's list of header files
    set(_SOURCES)                  # ADD_LIBRARY's list of source files
    set(_PRECOMPILED_INCLUDE "")   # Precompiled header #include -->"path/precomp.h"<== this path
    set(_PRECOMPILED_HEADER)       # Precompiled header file with path
    set(_PRECOMPILED_SOURCE)       # Precompiled source file that only includes the precomp header
                                   # This file must be part of SOURCES too.
    set(_PRECOMPILED_SOURCES)      # list of source file that use the precomp header, optional.
                                   # Defaults to SOURCES, use this if not ALL SOURCES have
                                   # #include "PRECOMPILED_INCLUDE" since MSVC will give an error for missing include.
    set(_PRECOMPILED_FORCED)       # Force including the PRECOMPILED_INCLUDE
    set(_LINK_LIBRARIES)           # TARGET_LINK_LIBRARIES's list of libraries to link to
    set(_PROPERTIES)               # SET_TARGET_PROPERTIES's list of PROPERTIES
    set(_INSTALL)                  # INSTALL's list of commands


    set(cmd)                       # Current directive we're processing
    set(HEADER_FILES)
    set(SOURCE_FILES)
    set(OBJECT_FILES)
    set(PRECOMPILED_SOURCE_FILES)

    FOREACH(arg ${ARGN})
        set(read_args FALSE) # TRUE if we didn't find a keyword

        if     ("${arg}" STREQUAL    "SHARED")
            set(_SHARED              "SHARED")
        elseif ("${arg}" STREQUAL    "STATIC")
            set(_STATIC              "STATIC")
        elseif ("${arg}" STREQUAL    "MODULE")
            set(_MODULE              "MODULE")
        elseif ("${arg}" STREQUAL    "CUDA")
            set(_CUDA                 TRUE)
        elseif ("${arg}" STREQUAL    "EXCLUDE_FROM_ALL")
            set(_EXCLUDE_FROM_ALL    "EXCLUDE_FROM_ALL")
        elseif ("${arg}" STREQUAL    "ALLOW_MISSING_FILES")
            set(_ALLOW_MISSING_FILES "ALLOW_MISSING")
        elseif ("${arg}" STREQUAL    "HEADERS")
            set(cmd                  "HEADERS")
        elseif ("${arg}" STREQUAL    "SOURCES")
            set(cmd                  "SOURCES")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_INCLUDE")
            set(cmd                  "PRECOMPILED_INCLUDE")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_HEADER")
            set(cmd                  "PRECOMPILED_HEADER")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_SOURCE")
            set(cmd                  "PRECOMPILED_SOURCE")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_SOURCES")
            set(cmd                  "PRECOMPILED_SOURCES")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_FORCED")
            set(_PRECOMPILED_FORCED  "FORCE_INCLUDE")
        elseif ("${arg}" STREQUAL    "LINK_LIBRARIES")
            set(cmd                  "LINK_LIBRARIES")
        elseif ("${arg}" STREQUAL    "PROPERTIES")
            set(cmd                  "PROPERTIES")
        elseif ("${arg}" STREQUAL    "INSTALL")
            set(cmd                  "INSTALL")
        else()
            set(read_args TRUE) # hopefully a file
        endif()

        if (read_args)
            if ("${cmd}" STREQUAL "HEADERS")
                set(_HEADERS ${_HEADERS} ${arg})
                ADD_FILE_TO_LIST(HEADER_FILES ${arg} ${_ALLOW_MISSING_FILES})
            elseif ("${cmd}" STREQUAL "SOURCES")
                set(_SOURCES ${_SOURCES} ${arg})
                ADD_FILE_TO_LIST(SOURCE_FILES ${arg} ${_ALLOW_MISSING_FILES})

            elseif ("${cmd}" STREQUAL "PRECOMPILED_INCLUDE")
                set(_PRECOMPILED_INCLUDE ${arg})
            elseif ("${cmd}" STREQUAL "PRECOMPILED_HEADER")
                ADD_FILE_TO_LIST(_PRECOMPILED_HEADER ${arg} ${_ALLOW_MISSING_FILES})
            elseif ("${cmd}" STREQUAL "PRECOMPILED_SOURCE")
                ADD_FILE_TO_LIST(_PRECOMPILED_SOURCE ${arg} ${_ALLOW_MISSING_FILES})
            elseif ("${cmd}" STREQUAL "PRECOMPILED_SOURCES")
                set(_PRECOMPILED_SOURCES ${_PRECOMPILED_SOURCES} ${arg})
                ADD_FILE_TO_LIST(PRECOMPILED_SOURCE_FILES ${arg} ${_ALLOW_MISSING_FILES})

            elseif ("${cmd}" STREQUAL "LINK_LIBRARIES")
                set(_LINK_LIBRARIES ${_LINK_LIBRARIES} ${arg})
            elseif ("${cmd}" STREQUAL "PROPERTIES")
                set(_PROPERTIES ${_PROPERTIES} ${arg})
            elseif ("${cmd}" STREQUAL "INSTALL")
                set(_INSTALL ${_INSTALL} ${arg})
            else()
                MESSAGE(FATAL_ERROR "Unknown directive '${arg}' or '${cmd}' in ADD_LIBRARY_FULL")
            endif()
        endif()
    ENDFOREACH(arg)

    SET("${target_name}_DIRECTORY" "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "")
    SET("${target_name}_SOURCE_DIR" "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "")
    SET("${target_name}_BINARY_DIR" "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "")

    SET("${target_name}_HEADER_FILES" ${HEADER_FILES} CACHE INTERNAL "")
    SET("${target_name}_SOURCE_FILES" ${SOURCE_FILES} CACHE INTERNAL "")

    GET_OBJECT_FILES(${target_name} OBJECT_FILES "${SOURCE_FILES}" )
    SET("${target_name}_OBJECT_FILES" ${OBJECT_FILES} CACHE INTERNAL "")

    # Make sure we have at least one file since the error message from ADD_LIBRARY is cryptic
    SET(ALL_FILES ${HEADER_FILES} ${SOURCE_FILES})
    LIST(LENGTH ALL_FILES ALL_FILES_COUNT)
    if (${ALL_FILES_COUNT} LESS "1")
        MESSAGE(WARNING "NO files to compile for ${target_name} in ${CMAKE_CURRENT_SOURCE_DIR}!\n Input headers: '${_HEADERS}' and sources: '${_SOURCES}'")
    endif()

    if (NOT "${_PRECOMPILED_INCLUDE}" STREQUAL "")
        if (CMAKE_COMPILER_IS_GNUCXX)
            include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/include") # find .gch file before .h file
        endif()
    endif()

    if (_CUDA)
        CUDA_ADD_LIBRARY(${target_name} ${_SHARED} ${_STATIC} ${_MODULE} ${_EXCLUDE_FROM_ALL} ${HEADER_FILES} ${SOURCE_FILES})
    else()
        ADD_LIBRARY(     ${target_name} ${_SHARED} ${_STATIC} ${_MODULE} ${_EXCLUDE_FROM_ALL} ${HEADER_FILES} ${SOURCE_FILES})
    endif()
    TARGET_LINK_LIBRARIES(${target_name} ${_LINK_LIBRARIES})

    if (DEFINED _PROPERTIES)
        SET_TARGET_PROPERTIES(${target_name} PROPERTIES ${_PROPERTIES})
    endif()

    if (NOT "${_PRECOMPILED_INCLUDE}" STREQUAL "")
        if ("${PRECOMPILED_SOURCE_FILES}" STREQUAL "")
            set(PRECOMP_SRCS ${SOURCE_FILES})
        else()
            set(PRECOMP_SRCS ${PRECOMPILED_SOURCE_FILES})
        endif()

        TARGET_PRECOMPILED_HEADER(${target_name} ${_PRECOMPILED_FORCED}
                                  PCH_INCLUDE "${_PRECOMPILED_INCLUDE}"
                                  PCH_HEADER  "${_PRECOMPILED_HEADER}"
                                  PCH_SOURCE  "${_PRECOMPILED_SOURCE}"
                                  SOURCES      ${PRECOMP_SRCS})
    endif()

    DO_GET_TARGET_FLAGS(${target_name})

    # Set the install includes to use same dir structure as the source directories
    #STRING(REPLACE "${CMAKE_HOME_DIRECTORY}/" "" PROJ_DIR_TEMP ${CMAKE_CURRENT_SOURCE_DIR})
    #INSTALL(FILES
    #    ${HEADER_FILES} DESTINATION "include/${PROJ_DIR_TEMP}")

    # INSTALL(... EXPORT ...) appears to be a little overealous.
    # If you have targetA and targetB depends on A you have to do this.
    # In targetA directory :
    #   INSTALL(TARGETS targetA EXPORT targetA_exports ...)
    #   INSTALL(TARGETS targetA EXPORT targetB_exports ...)
    #   INSTALL(EXPORT targetA_exports DESTINATION ...)
    # In targetB directory :
    #   INSTALL(TARGETS targetA EXPORT targetB_exports ...)
    #   INSTALL(EXPORT targetA_exports DESTINATION ...)
    # So this cannot be made to work when you chain together CMakeLists.txt
    # for targetA and targetB when targetA may not know about B.
    # Ideally, targetA and targetB simply install their own export files
    # so that targetC can import them both. There is no reason that
    # targetB absolutely must export targetA, but I don't know how to do that.

    if ("${_EXCLUDE_FROM_ALL}" STREQUAL "")
    if (NOT "${_INSTALL}" STREQUAL "")
        INSTALL(TARGETS ${target_name} ${_INSTALL})
                #EXPORT  "${target_name}_export"
                #RUNTIME DESTINATION bin
                #LIBRARY DESTINATION lib
                #ARCHIVE DESTINATION lib)
    endif()
    endif()

endfunction()

# ---------------------------------------------------------------------------
# Make ADD_EXECUTABLE() a little easier to read by putting together the
# necessary calls into one function.
# Usage:
#   ADD_EXECUTABLE_FULL( target_name [WIN32] [MACOSX_BUNDLE] [CUDA] [EXCLUDE_FROM_ALL] [ALLOW_MISSING_FILES]
#       HEADERS                                    # Can use wildcards like "*.h" or "" for no files
#           "include/foo/*.h" "include/foo/*.hpp"  # or specify the file names exactly.
#       SOURCES                                    # You must specify at least one valid source file.
#           "src/a.cpp" "src/b.cpp"                #
#      [PRECOMPILED_INCLUDE] foo/precomp.h         # The path to precomp header as used exactly in #include "foo/precomp.h"
#      [PRECOMPILED_HEADER]  include/foo/precomp.h # Path to the precomp header file.
#      [PRECOMPILED_SOURCE]  src/precomp.cpp       # Path to the precomp cpp file that only includes the header
#      [PRECOMPILED_SOURCES] src/a.cpp             # Full path to cpp files that include the precomp header.
#      [PRECOMPILED_FORCED]                        # Force including the PRECOMPILED_INCLUDE by the compiler.
#       LINK_LIBRARIES                             # Replaces TARGET_LINK_LIBRARIES(target_name lib1 lib2 ...)
#           gtest ${Boost_LIBRARIES}
#       PROPERTIES FOLDER "Foo/Bar"                # replaces SET_TARGET_PROPERTIES(target_name PROPERTIES ...)
#       PROPERTIES LINKER_LANGUAGE CXX ...         # use PROPERTIES XXX YYY as many times as you like
#       INSTALL "${ADD_EXECUTABLE_FULL_INSTALL_DEFAULT}" # INSTALL(TARGETS ${target_name} ${ADD_EXECUTABLE_FULL_INSTALL_DEFAULT})
#                                                  # is called with the parameters expanded into a list.
#                                                  # Create list as shown below or use the default flags as shown here.
#   )
#
# For the 'INSTALL' command create a variable like this to use: (see also ADD_EXECUTABLE_FULL_INSTALL_DEFAULT)
#   SET( MY_INSTALL_FLAG_LIST
#        DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#        BUNDLE  DESTINATION bin)
#
# Internal cached variables will be created:
#   ${target_name}_HEADER_FILES - List of HEADERS given above
#   ${target_name}_SOURCE_FILES - List of SOURCES given above
#   ${target_name}_OBJECT_FILES - List of object files CMake will create compiling SOURCES
#
# INSTALL() put the exe into CMAKE_INSTALL_PREFIX/bin.
# ---------------------------------------------------------------------------

# These are flags for a default "normal" install for a ADD_EXECUTABLE_FULL() target.
set(ADD_EXECUTABLE_FULL_INSTALL_DEFAULT
    DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    BUNDLE  DESTINATION bin)

function( ADD_EXECUTABLE_FULL target_name )

    set(_WIN32)                    # ADD_EXECUTABLE's WIN32 directive
    set(_MACOSX_BUNDLE)            # ADD_EXECUTABLE's MACOSX_BUNDLE directive
    set(_CUDA FALSE)               # Use CUDA_ADD_EXECUTABLE() not ADD_EXECUTABLE()
    set(_EXCLUDE_FROM_ALL)         # ADD_EXECUTABLE's EXCLUDE_FROM_ALL directive
    set(_ALLOW_MISSING_FILES)      # was ALLOW_MISSING_FILES directive used
    set(_HEADERS)                  # ADD_LIBRARY's list of header files
    set(_SOURCES)                  # ADD_LIBRARY's list of source files
    set(_PRECOMPILED_INCLUDE "")   # Precompiled header #include -->"path/precomp.h"<== this path
    set(_PRECOMPILED_HEADER)       # Precompiled header file with path
    set(_PRECOMPILED_SOURCE)       # Precompiled source file that only includes the precomp header
                                   # This file must be part of SOURCES too.
    set(_PRECOMPILED_SOURCES)      # list of source file that use the precomp header, optional.
                                   # Defaults to SOURCES, use this if not ALL SOURCES have
                                   # #include "PRECOMPILED_INCLUDE" since MSVC will give an error for missing include.
    set(_PRECOMPILED_FORCED)       # Force including the PRECOMPILED_INCLUDE
    set(_LINK_LIBRARIES)           # TARGET_LINK_LIBRARIES's list of libraries to link to
    set(_PROPERTIES)               # SET_TARGET_PROPERTIES's list of PROPERTIES
    set(_INSTALL)                  # INSTALL's list of commands

    set(cmd)                       # current directive we're processing
    set(HEADER_FILES)
    set(SOURCE_FILES)
    set(OBJECT_FILES)
    set(PRECOMPILED_SOURCE_FILES)

    FOREACH(arg ${ARGN})
        set(read_args FALSE) # TRUE if we didn't find a keyword

        if     ("${arg}" STREQUAL    "WIN32")
            set(_WIN32               "WIN32")
        elseif ("${arg}" STREQUAL    "MACOSX_BUNDLE")
            set(_MACOSX_BUNDLE       "MACOSX_BUNDLE")
        elseif ("${arg}" STREQUAL    "CUDA")
            set(_CUDA                 TRUE)
        elseif ("${arg}" STREQUAL    "EXCLUDE_FROM_ALL")
            set(_EXCLUDE_FROM_ALL    "EXCLUDE_FROM_ALL")
        elseif ("${arg}" STREQUAL    "ALLOW_MISSING_FILES")
            set(_ALLOW_MISSING_FILES "ALLOW_MISSING")
        elseif ("${arg}" STREQUAL    "HEADERS")
            set(cmd                  "HEADERS")
        elseif ("${arg}" STREQUAL    "SOURCES")
            set(cmd                  "SOURCES")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_INCLUDE")
            set(cmd                  "PRECOMPILED_INCLUDE")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_HEADER")
            set(cmd                  "PRECOMPILED_HEADER")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_SOURCE")
            set(cmd                  "PRECOMPILED_SOURCE")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_SOURCES")
            set(cmd                  "PRECOMPILED_SOURCES")
        elseif ("${arg}" STREQUAL    "PRECOMPILED_FORCED")
            set(_PRECOMPILED_FORCED  "FORCE_INCLUDE")
        elseif ("${arg}" STREQUAL    "LINK_LIBRARIES")
            set(cmd                  "LINK_LIBRARIES")
        elseif ("${arg}" STREQUAL    "PROPERTIES")
            set(cmd                  "PROPERTIES")
        elseif ("${arg}" STREQUAL    "INSTALL")
            set(cmd                  "INSTALL")
        else()
            set(read_args TRUE) # hopefully a file
        endif()

        if (read_args)
            if ("${cmd}" STREQUAL "HEADERS")
                set(_HEADERS ${_HEADERS} ${arg})
                ADD_FILE_TO_LIST(HEADER_FILES ${arg} ${_ALLOW_MISSING_FILES})
            elseif ("${cmd}" STREQUAL "SOURCES")
                set(_SOURCES ${_SOURCES} ${arg})
                ADD_FILE_TO_LIST(SOURCE_FILES ${arg} ${_ALLOW_MISSING_FILES})

            elseif ("${cmd}" STREQUAL "PRECOMPILED_INCLUDE")
                set(_PRECOMPILED_INCLUDE ${arg})
            elseif ("${cmd}" STREQUAL "PRECOMPILED_HEADER")
                ADD_FILE_TO_LIST(_PRECOMPILED_HEADER ${arg} ${_ALLOW_MISSING_FILES})
            elseif ("${cmd}" STREQUAL "PRECOMPILED_SOURCE")
                ADD_FILE_TO_LIST(_PRECOMPILED_SOURCE ${arg} ${_ALLOW_MISSING_FILES})
            elseif ("${cmd}" STREQUAL "PRECOMPILED_SOURCES")
                set(_PRECOMPILED_SOURCES ${_PRECOMPILED_SOURCES} ${arg})
                ADD_FILE_TO_LIST(PRECOMPILED_SOURCE_FILES ${arg} ${_ALLOW_MISSING_FILES})

            elseif ("${cmd}" STREQUAL "LINK_LIBRARIES")
                set(_LINK_LIBRARIES ${_LINK_LIBRARIES} ${arg})
            elseif ("${cmd}" STREQUAL "PROPERTIES")
                set(_PROPERTIES ${_PROPERTIES} ${arg})
            elseif ("${cmd}" STREQUAL "INSTALL")
                set(_INSTALL ${_INSTALL} ${arg})
            else()
                MESSAGE(FATAL_ERROR "Unknown directive '${arg}' or '${cmd}' in ADD_EXECUTABLE_FULL")
            endif()
        endif()
    ENDFOREACH(arg)

    SET("${target_name}_DIRECTORY" "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "")
    SET("${target_name}_SOURCE_DIR" "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "")
    SET("${target_name}_BINARY_DIR" "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "")

    SET("${target_name}_HEADER_FILES" ${HEADER_FILES} CACHE INTERNAL "")
    SET("${target_name}_SOURCE_FILES" ${SOURCE_FILES} CACHE INTERNAL "")

    GET_OBJECT_FILES(${target_name} OBJECT_FILES "${SOURCE_FILES}" )
    SET("${target_name}_OBJECT_FILES" ${OBJECT_FILES} CACHE INTERNAL "")

    # Make sure we have at least one file since the error message from ADD_EXECUTABLE is cryptic
    SET(ALL_FILES ${HEADER_FILES} ${SOURCE_FILES})
    LIST(LENGTH ALL_FILES ALL_FILES_COUNT)
    if (${ALL_FILES_COUNT} LESS "1")
        MESSAGE(WARNING "NO files to compile for ${target_name} in ${CMAKE_CURRENT_SOURCE_DIR}!\n Input headers: '${_HEADERS}' and sources: '${_SOURCES}'")
    endif()

    if (NOT "${_PRECOMPILED_INCLUDE}" STREQUAL "")
        if (CMAKE_COMPILER_IS_GNUCXX)
            include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/include") # find .gch file before .h file
        endif()
    endif()

    if (_CUDA)
        CUDA_ADD_EXECUTABLE(${target_name} ${_WIN32} ${_MACOSX_BUNDLE} ${_EXCLUDE_FROM_ALL} ${HEADER_FILES} ${SOURCE_FILES})
    else()
        ADD_EXECUTABLE(     ${target_name} ${_WIN32} ${_MACOSX_BUNDLE} ${_EXCLUDE_FROM_ALL} ${HEADER_FILES} ${SOURCE_FILES})
    endif()
    TARGET_LINK_LIBRARIES(${target_name} ${_LINK_LIBRARIES})

    if (DEFINED _PROPERTIES)
        SET_TARGET_PROPERTIES(${target_name} PROPERTIES ${_PROPERTIES} )
    endif()

    if (NOT "${_PRECOMPILED_INCLUDE}" STREQUAL "")
        if ("${PRECOMPILED_SOURCE_FILES}" STREQUAL "")
            set(PRECOMP_SRCS ${SOURCE_FILES})
        else()
            set(PRECOMP_SRCS ${PRECOMPILED_SOURCE_FILES})
        endif()

        TARGET_PRECOMPILED_HEADER(${target_name} ${_PRECOMPILED_FORCED}
                                  PCH_INCLUDE "${_PRECOMPILED_INCLUDE}"
                                  PCH_HEADER  "${_PRECOMPILED_HEADER}"
                                  PCH_SOURCE  "${_PRECOMPILED_SOURCE}"
                                  SOURCES      ${PRECOMP_SRCS})
    endif()

    if ("${_EXCLUDE_FROM_ALL}" STREQUAL "")
    if (NOT "${_INSTALL}" STREQUAL "")
    	INSTALL(TARGETS ${target_name} ${_INSTALL})
                #DESTINATION bin
                #LIBRARY DESTINATION lib
                #ARCHIVE DESTINATION lib
                #BUNDLE  DESTINATION bin)
    endif()
    endif()

endfunction()

# ---------------------------------------------------------------------------
# This function can be used to add include search paths to library targets.
#
# Usage:
#  ADD_TARGET_INCLUDE_PATHS(target_name path/to/a.h path/to/b.h)
#
# Parameters:
#  target_name   - The name of the target to modify,
#                  i.e. the one you created with ADD_LIBRARY(target_name...)
#  INCLUDE_PATHS - Relative paths from the CMakeLists.txt file caller to add
#                  to the compiler's command line to other proto files (its /I or -I option)
# Example:
#  ADD_TARGET_INCLUDE_PATHS(target_name "../Project2/include/" "../Project3/include/")
# ---------------------------------------------------------------------------

function(ADD_TARGET_INCLUDE_PATHS target_name)

    set(INCLUDE_PATHS_ARGS)

    foreach(INCLUDE_PATH ${ARGN})
        get_filename_component(ABS_INCLUDE_PATH ${INCLUDE_PATH} ABSOLUTE)

        IF (MSVC)
            set(INCLUDE_PATHS_ARGS "${INCLUDE_PATHS_ARGS} /I \"${ABS_INCLUDE_PATH}\"" )
        ELSE()
            set(INCLUDE_PATHS_ARGS "${INCLUDE_PATHS_ARGS} -I ${ABS_INCLUDE_PATH}" )
        ENDIF()

    endforeach()

    APPEND_TARGET_PROPERTIES(${target_name} PROPERTIES COMPILE_FLAGS ${INCLUDE_PATHS_ARGS})

endfunction()


# ---------------------------------------------------------------------------
# APPEND_TARGET_PROPERTIES() is the same as SET_TARGET_PROPERTIES() except that
# instead of overwriting the property value it appends to the current one.
# append_target_properties(target1 target2 ...
#                          PROPERTIES prop1 value1 prop2 value2 ...)
# ---------------------------------------------------------------------------
function(APPEND_TARGET_PROPERTIES)

    set(TARGETS)                        # list of targets to append props to
    set(PROPERTIES FALSE)
    set(PROP_NAME "")                   # last prop name

    FOREACH(arg ${ARGN})
        if (PROPERTIES)
            # We get the old prop value, append the new and apply it to each target

            if ("${PROP_NAME}" STREQUAL "")
                set(PROP_NAME ${arg})
            else()
                foreach(t ${TARGETS})
                    GET_TARGET_PROPERTY(old_value ${t} ${PROP_NAME})
                    # If no old value ("old_value-NOTFOUND") then set the variable to empty string
                    IF (NOT old_value)
                        SET(old_value "")
                    ENDIF()

                    SET_TARGET_PROPERTIES(${t} PROPERTIES ${PROP_NAME} "${old_value} ${arg}")
                endforeach()

                set(PROP_NAME "")
            endif()
        elseif ("${arg}" STREQUAL "PROPERTIES")
            set(PROPERTIES TRUE)
        else()
            set(TARGETS ${TARGETS} ${arg})
        endif()
    ENDFOREACH(arg)

    if (NOT PROPERTIES)
        message(FATAL_ERROR "Missing PROPERTIES directive in APPEND_TARGET_PROPERTY()")
    endif()
    if (NOT "${PROP_NAME}" STREQUAL "")
        message(FATAL_ERROR "Missing value in PROPERTIES directive in APPEND_TARGET_PROPERTY()")
    endif()
endfunction()

# ---------------------------------------------------------------------------
# APPEND_SOURCE_FILES_PROPERTIES() is the same as SET_SOURCE_FILES_PROPERTIES()
# except that instead of overwriting the property value it appends to the current one.
# set_source_files_properties([file1 [file2 [...]]]
#                              PROPERTIES prop1 value1
#                              [prop2 value2 [...]])
# ---------------------------------------------------------------------------
function(APPEND_SOURCE_FILES_PROPERTIES)

    set(FILES)                          # list of files to append props to
    set(PROPERTIES FALSE)
    set(PROP_NAME "")                   # name of last prop

    FOREACH(arg ${ARGN})
        if (PROPERTIES)
            # We get the old prop value, append the new and apply it to each target

            if ("${PROP_NAME}" STREQUAL "")
                set(PROP_NAME ${arg})
            else()
                foreach(f ${FILES})
                    get_source_file_property(old_value ${f} ${PROP_NAME})
                    # If no old value, it equals "NOTFOUND"
                    IF (NOT old_value)
                        SET(old_value "")
                    ENDIF()

                    SET_SOURCE_FILES_PROPERTIES(${f} PROPERTIES ${PROP_NAME} "${old_value} ${arg}")
                endforeach()

                set(PROP_NAME "")
            endif()
        elseif ("${arg}" STREQUAL "PROPERTIES")
            set(PROPERTIES TRUE)
        else()
            set(FILES ${FILES} ${arg})
        endif()
    ENDFOREACH(arg)

    if (NOT PROPERTIES)
        message(FATAL_ERROR "Missing PROPERTIES directive in APPEND_SOURCE_FILES_PROPERTIES()")
    endif()
    if (NOT "${PROP_NAME}" STREQUAL "")
        message(FATAL_ERROR "Missing value in PROPERTIES directive in APPEND_SOURCE_FILES_PROPERTIES()")
    endif()
endfunction()

# ---------------------------------------------------------------------------
# GET_OBJECT_FILES( target_name object_file_list source_files )
#
# Append to the object_file_list the object file(s) which would be generated by an
# ADD_EXECUTABLE() or ADD_LIBRARY() call.
# Usage :
#   GET_OBJECT_FILES(blah objlist "a.cpp;b.cpp") ...
#   objlist = "/path/to/.../blah.dir/.../a.obj; .../b.obj or *.o as appropriate.
# ---------------------------------------------------------------------------

FUNCTION( GET_OBJECT_FILES target_name object_file_list source_files )
    if (MSVC)
        foreach(f ${source_files})
            GET_FILENAME_COMPONENT(fname ${f} NAME)
            string(REPLACE ".cpp" ".obj" fname2 ${fname})
            string(REPLACE ".c"   ".obj" fname2 ${fname2})
            if (MSVC60)
                set(objects ${objects} "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${fname2}")
            else()
                set(objects ${objects} "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.dir/${CMAKE_CFG_INTDIR}/${fname2}")
            endif()
        endforeach()
    else()
        # Backwards compat to CMake <= 2.8.4
        if ("${CMAKE_CURRENT_LIST_DIR}" STREQUAL "")
            get_filename_component(CMAKE_CURRENT_LIST_DIR
                                  ${CMAKE_CURRENT_LIST_FILE} PATH ABSOLUTE)
        endif()

        foreach(f ${source_files})
            #GET_FILENAME_COMPONENT(fname ${f} NAME)
            string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" fname "${f}")
            if ("${CMAKE_GENERATOR}" MATCHES "MinGW Makefiles")
                set(objects ${objects} "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target_name}.dir/${fname}.obj")
            else()
                set(objects ${objects} "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target_name}.dir/${fname}.o")
            endif()
        endforeach()
    endif()

    set(${object_file_list} ${${object_file_list}} ${objects} PARENT_SCOPE)

ENDFUNCTION()

# ---------------------------------------------------------------------------
# DO_GET_TARGET_FLAGS( target_name )
#
# Create these variables as internal cache variables.
#   ${target_name}_CXX_FLAGS
#   ${target_name}_CXX_FLAGS_Release[ or _Debug, _RelWithDebInfo, _MinSizeRel]
#
# This function is automatically called by ADD_EXECUTABLE/LIBRARY_FULL()
# and you do not have to call it afterwards, unless you change the target.
#
# Usage :
#   GET_TARGET_INCLUDES(mytarget) ...
# ---------------------------------------------------------------------------

FUNCTION( DO_GET_TARGET_FLAGS target_name )

    set(_CONFIGS "" Release Debug RelWithDebInfo MinSizeRel)

    set(cxx_flags "")

    foreach(c ${_CONFIGS})
        string(TOUPPER ${c} c_UPPERCASE)
        set(cxx_flags_${c_UPPERCASE} "")
    endforeach()

    # Get DLL export define, CMake automatically defines ${target_name}_EXPORTS
    # unless it has been changed.
    get_target_property(target_type ${target_name} TYPE)
    if ("${target_type}" STREQUAL "SHARED_LIBRARY")
        set(cxx_flags ${cxx_flags} ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})

        get_target_property(target_define_symbol ${target_name} DEFINE_SYMBOL)
        if (target_define_symbol)
            list(APPEND cxx_flags "-D${target_define_symbol}")
        else()
            list(APPEND cxx_flags "-D${target_name}_EXPORTS")
        endif()
    endif()

    get_target_property(target_flags ${target_name} COMPILE_FLAGS)
    if (target_flags)
        set(cxx_flags ${cxx_flags} ${target_flags})
        set(target_flags "${target_flags};") # append ; for use later
    else()
        set(target_flags) # clear it for use later
    endif()

    get_target_property(target_defs ${target_name} COMPILE_DEFINITIONS)
    if (target_defs)
        foreach (i ${target_defs})
            list(APPEND cxx_flags "-D${i}")
        endforeach()
    endif()

    foreach(c ${_CONFIGS})
        string(TOUPPER ${c} c_UPPERCASE)

        get_target_property(target_defs ${target_name} COMPILE_DEFINITIONS_${c_UPPERCASE})

        if (target_defs)
            foreach (i ${target_defs})
                list(APPEND cxx_flags_${c_UPPERCASE} "-D${i}")
            endforeach()
        endif()
    endforeach()

    set(list_dir ${CMAKE_CURRENT_SOURCE_DIR})
    #while (EXISTS ${list_dir})

        get_directory_property( dir_defs DIRECTORY ${list_dir} COMPILE_DEFINITIONS)
        foreach (i ${dir_defs})
            list(APPEND cxx_flags "-D${i}")
        endforeach()

        foreach(c ${_CONFIGS})
            string(TOUPPER ${c} c_UPPERCASE)

            get_directory_property( dir_defs DIRECTORY ${list_dir} COMPILE_DEFINITIONS_${c_UPPERCASE})

            if (dir_defs)
                foreach (i ${dir_defs})
                    list(APPEND cxx_flags_${c_UPPERCASE} "-D${i}")
                endforeach()
            endif()
        endforeach()

        get_directory_property(dir_includes DIRECTORY ${list_dir} INCLUDE_DIRECTORIES)
        foreach (i ${dir_includes})
            list(APPEND cxx_flags "-I${i}")
        endforeach()

        #get_directory_property(list_dir DIRECTORY ${list_dir} PARENT_DIRECTORY)
    #endwhile()

    set(cxx_flags ${cxx_flags} ${CMAKE_CXX_FLAGS})
    list(REMOVE_DUPLICATES cxx_flags)

    set("${target_name}_CXX_FLAGS" ${cxx_flags} CACHE INTERNAL "")
    list(REMOVE_DUPLICATES "${target_name}_CXX_FLAGS")

    set(_CONFIGS "" Release Debug RelWithDebInfo MinSizeRel)
    foreach(c ${_CONFIGS})
        string(TOUPPER ${c} c_UPPERCASE)
        set("${target_name}_CXX_FLAGS_${c}" ${cxx_flags} ${cxx_flags_${c_UPPERCASE}} ${CMAKE_CXX_FLAGS_${c_UPPERCASE}} CACHE INTERNAL "")
        list(REMOVE_DUPLICATES "${target_name}_CXX_FLAGS_${c}")
    endforeach()

ENDFUNCTION()

# ---------------------------------------------------------------------------
# TARGET_PRECOMPILED_HEADER() function to add a precomp header to a target.
#
# Note that MSVC will error for SOURCES that do not have the line
#   #include "PCH_INCLUDE" (where PCH_INCLUDE is identical to what's passed to this function)
#   See FORCE_INCLUDE flag for a workaround that uses the compiler's
#     directive to #include a file before compilation.
#     This also works well for GCC and allows you to use precomp headers w/o
#     modifying any code at all.
#
# FORCE_INCLUDE flag may be used to tell the compiler to include the precomp header
#               in cases where it is not explicitly included in the code.
#               This can have great benefits in GCC and may be required in MSVC.
#
# Usage :
#  TARGET_PRECOMPILED_HEADER( myproj [FORCE_INCLUDE]
#      PCH_INCLUDE myproj/precomp.h         # The path to precomp header as used exactly in #include "foo/precomp.h"
#      PCH_HEADER  include/myproj/precomp.h # Path to the precomp header file on disk.
#      PCH_SOURCE  src/precomp.cpp          # Path to the precomp cpp file that only includes the header
#      SOURCES src/a.cpp src/b.cpp )        # Full path to cpp files that include the precomp header.
# ---------------------------------------------------------------------------

function( TARGET_PRECOMPILED_HEADER target_name )

    # Allow people to en/disable precomp headers to test if it actually works or not.
    # GCC is a little tricky to get right since you have to compile it exactly the same flags
    # and CMake bizarrely hides flags passed to add_definitions() not starting with -D.
    # There will be no sign that the precomp header is not used other than a longer compilation time.

    # How to measure compilation times:
    #   GCC use $time make;
    #   MSVC 2008 Tools->Options->Projects and Solutions->VC++ Project Settings->Build Timing [True]

    if ("${BUILD_USE_PRECOMPILED_HEADERS}" STREQUAL "")
        set(BUILD_USE_PRECOMPILED_HEADERS TRUE)

        if (NOT "${CMAKE_GENERATOR}" MATCHES "MinGW Makefiles")
            if (CMAKE_COMPILER_IS_GNUCXX)
                # ccache is much faster when not using our own precomp header so disable them
                set(BUILD_USE_PRECOMPILED_HEADERS FALSE)
            endif()
        endif()
    endif()
    set(BUILD_USE_PRECOMPILED_HEADERS ${BUILD_USE_PRECOMPILED_HEADERS} CACHE BOOL "Enable the use of precompiled headers? (disable if using gcc and ccache)" FORCE)

    IF (NOT BUILD_USE_PRECOMPILED_HEADERS)
        RETURN()
    ENDIF()

    # -----------------------------------------------------------------------

    set(_FORCE_INCLUDE)      # Tell the compiler to #include this file
    set(_PCH_INCLUDE)        # The #include "PCH_INCLUDE"
    set(_PCH_HEADER)         # Path to the PCH_INCLUDE
    set(_PCH_SOURCE)         # Source file that include PCH_INCLUDE
    set(_SOURCES)            # All source files that include PCH_INCLUDE

    set(cmd)                 # current directive we're processing
    set(SOURCE_FILES)

    FOREACH(arg ${ARGN})
        set(read_args FALSE) # TRUE if we didn't find a keyword

        if     ("${arg}" STREQUAL    "FORCE_INCLUDE")
            set(_FORCE_INCLUDE       "FORCE_INCLUDE")
        elseif ("${arg}" STREQUAL    "PCH_INCLUDE")
            set(cmd                  "PCH_INCLUDE")
        elseif ("${arg}" STREQUAL    "PCH_HEADER")
            set(cmd                  "PCH_HEADER")
        elseif ("${arg}" STREQUAL    "PCH_SOURCE")
            set(cmd                  "PCH_SOURCE")
        elseif ("${arg}" STREQUAL    "SOURCES")
            set(cmd                  "SOURCES")
        else()
            set(read_args TRUE) # hopefully a file
        endif()

        if (read_args)
            if ("${cmd}" STREQUAL "PCH_INCLUDE")
                set(_PCH_INCLUDE ${arg})
            elseif ("${cmd}" STREQUAL "PCH_HEADER")
                #set(_PCH_HEADER ${arg})
                ADD_FILE_TO_LIST(_PCH_HEADER ${arg})
            elseif ("${cmd}" STREQUAL "PCH_SOURCE")
                #set(_PCH_SOURCE ${arg})
                ADD_FILE_TO_LIST(_PCH_SOURCE ${arg})
            elseif ("${cmd}" STREQUAL "SOURCES")
                set(_SOURCES ${_SOURCES} ${arg})
                ADD_FILE_TO_LIST(SOURCE_FILES ${arg})
            else()
                MESSAGE(FATAL_ERROR "Unknown directive '${cmd}' in TARGET_PRECOMPILED_HEADER")
            endif()
        endif()
    ENDFOREACH(arg)


    # For precompiled headers in XCode we can set this property, ignored for other compilers.
    SET_TARGET_PROPERTIES(${target_name} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER YES)
    SET_TARGET_PROPERTIES(${target_name} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${_PCH_INCLUDE}")

    IF(MSVC)
        get_filename_component(PCH_BASENAME ${_PCH_HEADER} NAME_WE)
        #set(PCH_BINARY "${CMAKE_CURRENT_BINARY_DIR}/$(IntDir)/${PCH_BASENAME}.pch")
        set(PCH_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PCH_BASENAME}.pch")

        # need to delete this file for clean too, otherwise we can get errors
        # about mismatched pch and pdb files.
        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PCH_BINARY})

        file(TO_CMAKE_PATH "${_PCH_SOURCE}" native_pch_source)

        if (_FORCE_INCLUDE)
            set(FORCE_INCLUDE_FLAG "/FI\"${_PCH_INCLUDE}\"")
        endif()

        set(found_pch FALSE)

        foreach ( f ${SOURCE_FILES} )
            file(TO_CMAKE_PATH "${f}" native_f)

            if ("${native_pch_source}" STREQUAL "${native_f}")
                set(found_pch TRUE)

                APPEND_SOURCE_FILES_PROPERTIES(${_PCH_SOURCE} PROPERTIES
                                               COMPILE_FLAGS "/Yc\"${_PCH_INCLUDE}\" /Fp\"${PCH_BINARY}\""
                                               OBJECT_OUTPUTS "${PCH_BINARY}")
            else()
                APPEND_SOURCE_FILES_PROPERTIES(${f} PROPERTIES
                                               COMPILE_FLAGS "/Yu\"${_PCH_INCLUDE}\" /Fp\"${PCH_BINARY}\" ${FORCE_INCLUDE_FLAG}"
                                               OBJECT_DEPENDS "${PCH_BINARY}")
            endif()
        endforeach()

        if (NOT found_pch)
            message(WARNING "* Unable to find precompiled source file '${_PCH_SOURCE}' normalized to '${native_pch_source}' in list of files '${SOURCE_FILES}'")
        endif()
    ELSEIF (CMAKE_COMPILER_IS_GNUCXX)

        # We will try to create the pch in an include tree in the binary dir
        get_filename_component(PCH_INCLUDE_PATH "${_PCH_INCLUDE}" PATH)

        if ("${PCH_INCLUDE_PATH}" STREQUAL "")
            # Rename the header and put it into the cmake build dir and force include it.
            # The include guard will skip the original header
            set(_FORCE_INCLUDE TRUE)
            file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/${PCH_INCLUDE_PATH}")
            configure_file( "${_PCH_HEADER}" "${CMAKE_CURRENT_BINARY_DIR}/include/${target_name}_${_PCH_INCLUDE}" COPYONLY)
            set(_PCH_INCLUDE "${target_name}_${_PCH_INCLUDE}")
            set(PCH_BINARY "${CMAKE_CURRENT_BINARY_DIR}/include/${_PCH_INCLUDE}.gch")
            set(_PCH_INCLUDE "${CMAKE_CURRENT_BINARY_DIR}/include/${PCH_INCLUDE_PATH}/${_PCH_INCLUDE}")
        else()
            file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/${PCH_INCLUDE_PATH}")
            set(PCH_BINARY "${CMAKE_CURRENT_BINARY_DIR}/include/${_PCH_INCLUDE}.gch")
            #include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/include") # find .gch file before .h file
            configure_file( "${_PCH_HEADER}" "${CMAKE_CURRENT_BINARY_DIR}/include/${_PCH_INCLUDE}" COPYONLY)
            set(_PCH_INCLUDE "${CMAKE_CURRENT_BINARY_DIR}/include/${_PCH_INCLUDE}")
        endif()

        if (_FORCE_INCLUDE)
            APPEND_SOURCE_FILES_PROPERTIES(${SOURCE_FILES} PROPERTIES
                                           COMPILE_FLAGS "-include \"${_PCH_INCLUDE}\"")
        endif()

        # need to delete this file for clean too, otherwise we may be mismatched.
        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PCH_BINARY})

        # get the cxx flags if they haven't already
        if (NOT DEFINED ${target_name}_CXX_FLAGS)
            DO_GET_TARGET_FLAGS(${target_name})
        endif()

        # combine all the appropriate flags and remove duplicates
        set(cxx_flags ${${target_name}_CXX_FLAGS} ${${target_name}_CXX_FLAGS_${CMAKE_BUILD_TYPE}})
        list(REMOVE_DUPLICATES cxx_flags)
        list(APPEND cxx_flags -c ${_PCH_HEADER} -o ${PCH_BINARY})
        separate_arguments(cxx_flags)

        if (WIN32)
            add_custom_command(OUTPUT "${PCH_BINARY}"
                               COMMAND "${CMAKE_COMMAND}" -E remove "${PCH_BINARY}" # can't use 'del' here for some reason
                               COMMAND ${CMAKE_CXX_COMPILER} ${cxx_flags}
                               DEPENDS "${_PCH_HEADER}"
                               VERBATIM)
        else()
            add_custom_command(OUTPUT "${PCH_BINARY}"
                               COMMAND rm -f "${PCH_BINARY}"
                               COMMAND ${CMAKE_CXX_COMPILER} ${cxx_flags}
                               DEPENDS "${_PCH_HEADER}")
        endif()

        add_custom_target( "${target_name}_precomp" ALL DEPENDS "${PCH_BINARY}")

        add_dependencies(${target_name} ${target_name}_precomp)

        # Check for changed files included by the precomp header and warn them.
        append_target_properties( ${target_name} PROPERTIES COMPILE_FLAGS "-Winvalid-pch")
    ENDIF()
endfunction()

# ---------------------------------------------------------------------------
# ADD_DOXYGEN(target_name doxyfile_in doxyfile_out doxygen_working_dir)
# A function to make adding a doxygen target easy, the doxyfile_in will have
# @vars@ replaced by configure and copied to doxyfile_out and a target_name will
# made to call "doxygen ${doxyfile_out}" from the doxygen_working_dir.
# Usage : ADD_DOXYGEN( doc
#                      ${CMAKE_SOURCE_DIR}/build/Doxyfile.in
#                      ${CMAKE_BINARY_DIR}/Doxyfile.cfg
#                      ${CMAKE_BINARY_DIR} )
#
# These are some doxygen.cfg settings you might want to set to @VAR_NAME@
# then set their variables in CMake so that when configured the doxygen file
# will be setup correctly.
#
# set( DOXYGEN_PROJECT_NAME      "MyProgram" )
# set( DOXYGEN_PROJECT_NUMBER    "1.2.3" )
# set( DOXYGEN_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/doc" )
# set( DOXYGEN_STRIP_FROM_PATH   "${CMAKE_SOURCE_DIR}/include" )
# set( DOXYGEN_INPUT             "${CMAKE_SOURCE_DIR}/include/..." )
# set( DOXYGEN_FILE_PATTERNS     "*.h *.hpp" )
# ---------------------------------------------------------------------------

function( ADD_DOXYGEN target_name doxyfile_in doxyfile_out doxygen_working_dir)

    if (NOT DEFINED DOXYGEN_FOUND)
        find_package(Doxygen)
    endif()

    if (DOXYGEN_FOUND)
        configure_file( ${doxyfile_in} ${doxyfile_out} @ONLY )

        add_custom_target( ${target_name} ${DOXYGEN_EXECUTABLE} ${doxyfile_out}
                           WORKING_DIRECTORY ${doxygen_working_dir}
                           COMMENT "Generating API documentation with Doxygen" VERBATUM )

        message( STATUS "* Doxygen found, run $make ${target_name} to generate documentation in build folder." )
    else()
        message( STATUS "* WARNING: Doxygen NOT found, ${target_name} target will not be generated." )
    endif (DOXYGEN_FOUND)

endfunction()


# ---------------------------------------------------------------------------
# Dump all variables in CMake that we can possibly find
# It can be very confusing to users when something goes wrong and maybe this will help.
# ---------------------------------------------------------------------------

macro(GET_ALL_CMAKE_VARIABLES all_vars cache_vars)

    get_cmake_property(_props VARIABLES)
    foreach(_var ${_props})
        #message( STATUS "@ VARIABLES: ${_var} = \"${${_var}}\"" )
        set(${all_vars} ${${all_vars}} "${_var}")
    endforeach()

    unset(_var)
    unset(_props)

    get_cmake_property(_props CACHE_VARIABLES)
    foreach(_var ${_props})
        #message( STATUS "@ CACHE_VARIABLES: ${_var} = \"${${_var}}\"" )
        set(${all_vars}   ${${all_vars}}       "${_var}")
        set(${cache_vars} ${${cache_vars}} "${_var}")
    endforeach()

    unset(_var)
    unset(_props)

    # merge the VARIABLES and CACHE_VARIABLES since they're mostly the same
    list(SORT ${all_vars})
    list(REMOVE_DUPLICATES ${all_vars})
endmacro()


macro(PRINT_ALL_CMAKE_VARIABLES)
    set(CMAKE_ALL_VARIABLES_LIST)
    set(CMAKE_ALL_CACHE_VARIABLES_LIST)

    GET_ALL_CMAKE_VARIABLES(CMAKE_ALL_VARIABLES_LIST CMAKE_ALL_CACHE_VARIABLES_LIST)

    message( STATUS " ")
    message( STATUS "@----------------------------------------------------------------------------")
    message( STATUS "@- Begin printing internal CMake variables")
    message( STATUS "@----------------------------------------------------------------------------")

    foreach(_var ${CMAKE_ALL_VARIABLES_LIST})
        list(FIND CMAKE_ALL_CACHE_VARIABLES_LIST ${_var} _is_cached)
        if (_is_cached GREATER "-1")
            message( STATUS "@ CACHE VAR: ${_var} = \"${${_var}}\"" )
        else()
            message( STATUS "@ VARIABLE : ${_var} = \"${${_var}}\"" )
        endif()
    endforeach()

    get_cmake_property(_props COMMANDS)
    foreach(_var ${_props})
        message( STATUS "@ COMMANDS: ${_var} = \"${${_var}}\"" )
    endforeach()

    get_cmake_property(_props MACROS)
    foreach(_var ${_props})
        message( STATUS "@ MACROS: ${_var} = \"${${_var}}\"" )
    endforeach()

    get_cmake_property(_props COMPONENTS)
    foreach(_var ${_props})
        message( STATUS "@ COMPONENTS: ${_var} = \"${${_var}}\"" )
    endforeach()


    # Unset the variables for the PRINT_NEW_CMAKE_VARIABLES() macro
    unset(_var)
    unset(_props)
    unset(_is_cached)

    message( STATUS "@----------------------------------------------------------------------------")
    message( STATUS "@- End printing internal CMake variables")
    message( STATUS "@----------------------------------------------------------------------------")
    message( STATUS " ")
endmacro()

# ---------------------------------------------------------------------------
# This simply does a diff between the last call to PRINT_ALL_CMAKE_VARIABLES
# and this call, saving the newly founds vars for the next call to this.

macro( PRINT_NEW_CMAKE_VARIABLES )
    message( STATUS " ")
    message( STATUS "@----------------------------------------------------------------------------")
    message( STATUS "@- Begin printing NEW internal CMake variables")
    message( STATUS "@----------------------------------------------------------------------------")

    set(CMAKE_ALL_VARIABLES_LIST2)
    set(CMAKE_ALL_CACHE_VARIABLES_LIST2)

    GET_ALL_CMAKE_VARIABLES(CMAKE_ALL_VARIABLES_LIST2 CMAKE_ALL_CACHE_VARIABLES_LIST2)

    # merge the VARIABLES and CACHE_VARIABLES since they're mostly the same
    foreach(_var ${CMAKE_ALL_VARIABLES_LIST2})
        list(FIND CMAKE_ALL_VARIABLES_LIST ${_var} _is_new)
        if (_is_new EQUAL "-1")
            list(FIND CMAKE_ALL_CACHE_VARIABLES_LIST ${_var} _is_cached)
            if (_is_cached GREATER "-1")
                message( STATUS "@ CACHE VAR: ${_var} = \"${${_var}}\"" )
            else()
                message( STATUS "@ VARIABLE : ${_var} = \"${${_var}}\"" )
            endif()
        endif()
    endforeach()

    message( STATUS "@----------------------------------------------------------------------------")
    message( STATUS "@- End printing NEW internal CMake variables")
    message( STATUS "@----------------------------------------------------------------------------")
    message( STATUS " ")

    unset(_var)
    unset(_is_new)
    unset(_is_cached)

    set(CMAKE_ALL_VARIABLES_LIST       ${CMAKE_ALL_VARIABLES_LIST2})
    set(CMAKE_ALL_CACHE_VARIABLES_LIST ${CMAKE_ALL_CACHE_VARIABLES_LIST2})
endmacro()

# ---------------------------------------------------------------------------
# Allow users to see what CMake thinks of the system in gory detail.
# Most people have no idea how CMake works, this will at least give them
# a slight chance at figuring out why something doesn't work.

if ( NOT CMAKE_DEBUG_VARIABLES )
    set( CMAKE_DEBUG_VARIABLES FALSE )
endif()

set( CMAKE_DEBUG_VARIABLES ${CMAKE_DEBUG_VARIABLES} CACHE BOOL "Print all CMake internal variables" FORCE)

if ( CMAKE_DEBUG_VARIABLES )
    PRINT_ALL_CMAKE_VARIABLES()
endif()
