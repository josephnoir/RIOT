# name of your application
export PROJECT = test_easy_cpp_mix

# for easy switching of boards
export BOARD ?= native

# this has to be the absolute path of the RIOT-base dir
export RIOTBASE = $(CURDIR)/../..

# switches for c++/c mixedmode
export CPPMIX = 1
export CPPMIXCC = g++
#export CPPMIXEXTRALIBS
export CPPMIXCFLAGS = -std=c++11

## Modules to include.

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += posix
USEMODULE += pthread

INCLUDES += -I$(CURDIR)/
INCLUDES += -I$(RIOTBASE)/sys/posix/pthread/include/

include $(RIOTBASE)/Makefile.include
