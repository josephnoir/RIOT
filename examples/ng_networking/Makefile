# name of your application
APPLICATION = ng_6lo_udp

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Specify the modules that are used
USEMODULE += ng_netif
USEMODULE += ng_sixlowpan_frag
USEMODULE += ng_ipv6
USEMODULE += ng_udp
USEMODULE += ng_pktdump
USEMODULE += uart0
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
