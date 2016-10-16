.DEFAULT_GOAL ?= $(LIB)
$(LIB): $(LIB).so $(LIB).a

CXX ?= g++
_CXXFLAGS := \
	-std=c++11 -Wall -Werror -fPIC -O2 -g \
	$(CXXFLAGS) \
	$(addprefix -I,$(INCLUDES)) \
	-Iinclude

_LDFLAGS := \
	-fPIC \
	-pthread \
	$(LDFLAGS) \
	$(addprefix -l,$(LIBS)) \

#Service variables
_TMP_FILES := tmp_files

# Compilation targets
_MODULES := $(SOURCES:%.cpp=$(_TMP_FILES)/%.o)
_DEPS_MK := $(_TMP_FILES)/deps.mk

$(LIB).so: $(_MODULES)
	$(CXX) -shared $^ -o $@ $(_LDFLAGS)

$(LIB).a: $(_MODULES)
	ar rvs $@ $^

$(_TMP_FILES):
	mkdir -p $@

## Everything should be rebuilt upon Makefile change
$(_TMP_FILES)/%.o: %.cpp Makefile $(_TMP_FILES)
	$(CXX) $(_CXXFLAGS) -c $< -o $@

ifneq ($(SOURCES),)
$(_DEPS_MK): $(SOURCES) $(_TMP_FILES)
	#FIXME: this works improperly for interface files put into include/ folder
	$(CXX) -MM $(_CXXFLAGS) $(SOURCES) > $@

include $(_DEPS_MK)
endif

clean: clean-lib
distclean: distclean-lib

clean-lib:
	rm -rf $(_TMP_FILES)

distclean-lib:
	rm -f $(LIB).so $(LIB).a
