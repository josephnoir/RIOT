# name of your application
export PROJECT = test_std_thread

# for easy switching of boards
export BOARD ?= native

# this has to be the absolute path of the RIOT-base dir
export RIOTBASE = $(CURDIR)/../..

# switches for c++/c mixedmode
export CPPMIX = 1
export CPPMIXCC = g++ -g -static-libstdc++ -L/home/developer/Desktop/test/RIOT/tests/test_std_thread
#export CPPMIXEXTRALIBS
export CPPMIXCFLAGS = -std=c++11

## Modules to include.

USEMODULE += posix
USEMODULE += pthread

ifeq ($(BOARD),native)
    CFLAGS += -isystem $(RIOTBASE)/sys/posix/pthread/include
   # CXXFLAGS += -isystem $(RIOTBASE)/sys/posix/pthread/include
#else
    export INCLUDES += -I$(RIOTBASE)/sys/posix/pthread/include \
                       -I$(RIOTBASE)/sys/posix/include
endif

#CFLAGS += -D__STRICT_ANSI__

INCLUDES += -I$(CURDIR)/

include $(RIOTBASE)/Makefile.include
