# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8")
  file(MAKE_DIRECTORY "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8")
endif()
file(MAKE_DIRECTORY
  "/Users/kinda/Developer/stm32-study/day-01/tmp/1"
  "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8"
  "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8/tmp"
  "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8/src/Project.Debug+STM32F103C8-stamp"
  "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8/src"
  "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8/src/Project.Debug+STM32F103C8-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8/src/Project.Debug+STM32F103C8-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/kinda/Developer/stm32-study/day-01/tmp/Project.Debug+STM32F103C8/src/Project.Debug+STM32F103C8-stamp${cfgdir}") # cfgdir has leading slash
endif()
