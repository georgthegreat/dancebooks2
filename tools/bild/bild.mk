_BILD_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifdef LIB
include $(_BILD_DIR)/lib.mk
endif

ifdef TESTS
include $(_BILD_DIR)/test.mk
endif

ifdef FILES
include $(_BILD_DIR)/install.mk
endif

