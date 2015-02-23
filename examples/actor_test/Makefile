# name of your application
APPLICATION = actor_test

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Uncomment these lines if you want to use platform support from external
# repositories:
#RIOTCPU ?= $(CURDIR)/../../../thirdparty_cpu
#RIOTBOARD ?= $(CURDIR)/../../../thirdparty_boards

# Uncomment this to enable scheduler statistics for ps:
#CFLAGS += -DSCHEDSTATISTICS

# If you want to use native with valgrind, you should recompile native
# with the target all-valgrind instead of all:
# make -B clean all-valgrind

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP -Wno-deprecated

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1 

CAF_HOME = /home/noir/actor-framework/

INCLUDES += -I$(CAF_HOME)/libcaf_core/

# If you want to add some extra flags when compile c++ files, add these flags
# to CXXEXFLAGS variable
CXXEXFLAGS += -std=c++11 -Os -s -Wno-deprecated -I. -D__RIOTBUILD_FLAG -DCAF_NO_MEM_MANAGEMENT

CXXEXTRALIBS += -Wl,--whole-archive
CXXEXTRALIBS += $(CAF_HOME)/build/lib/libcaf_core_static.a
CXXEXTRALIBS += -Wl,--no-whole-archive

USEMODULE += vtimer
USEMODULE += timex
USEMODULE += ps

include $(RIOTBASE)/Makefile.include
