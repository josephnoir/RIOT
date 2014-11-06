# name of your application
APPLICATION = wait_for

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP -Wno-deprecated

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# If you want to add some extra flags when compile c++ files, add these flags
# to CXXEXFLAGS variable
CXXEXFLAGS += --std=c++11 -g -O0 -D__RIOTBUILD_FLAG -DCAF_NO_MEM_MANAGEMENT

CAF_HOME = /home/localadmin/actor-framework/

INCLUDES += -I$(CAF_HOME)/libcaf_core/
INCLUDES += -I$(CAF_HOME)/libcaf_io/

CXXEXTRALIBS += -Wl,--whole-archive
CXXEXTRALIBS += $(CAF_HOME)/build/lib/libcaf_core_static.a
CXXEXTRALIBS += -Wl,--no-whole-archive

USEMODULE += vtimer
USEMODULE += timex

include $(RIOTBASE)/Makefile.include
