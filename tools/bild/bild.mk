# Some adequate defaults for user-site variables
CXX ?= g++
CXXFLAGS += -std=c++11 -Wall -Werror -fPIC -O2 -g -Iinclude
LDFLAGS += -fPIC

TEST-CXXFLAGS += -std=c++11 -Wall -Werror -Iinclude
TEST-LDFLAGS += -l:$(LIB).so -lboost_unit_test_framework

#Service variables
_TMP_FILES := tmp_files

default: $(LIB);

$(LIB): $(LIB).so $(LIB).a test;

# Compilation targets
_MODULES := $(SOURCES:%.cpp=$(_TMP_FILES)/%.o)
_DEPS_MK := $(_TMP_FILES)/deps.mk

$(LIB).so: $(_MODULES)
	$(CXX) $(LDFLAGS) -shared $^ -o $@

$(LIB).a: $(_MODULES)
	ar rvs $@ $^

## Everything should be rebuilt upon Makefile change
$(_TMP_FILES)/%.o: %.cpp Makefile $(_TMP_FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(_DEPS_MK): $(SOURCES) $(_TMP_FILES)
	$(CXX) -MM $(SOURCES) > $@

include $(_DEPS_MK)

# Test execution targets
_TEST_MODULES := $(TESTS:%.cpp=%)
_TEST_TARGETS := $(_TEST_MODULES:%=%-run)
_TEST_ENV := \
	LD_LIBRARY_PATH=. \

test: $(_TEST_TARGETS);

define _COMPILE_TEST_MODULE
$(1): $(1).cpp $$(LIB).so
	$$(CXX) \
		$$(TEST-CXXFLAGS) -o $$@ $$< \
		$$(TEST-LDFLAGS)
endef

define _RUN_TEST_MODULE
$(1)-run: $(1) $$(LIB).so
	$$(_TEST_ENV) $(1)
endef

$(foreach _TEST_MODULE,$(_TEST_MODULES),$(eval $(call _COMPILE_TEST_MODULE,$(_TEST_MODULE))))
$(foreach _TEST_MODULE,$(_TEST_MODULES),$(eval $(call _RUN_TEST_MODULE,$(_TEST_MODULE))))

# Utility targets
$(_TMP_FILES):
	mkdir -p $@

# Cleaning targets
clean:
	rm -rf $(_TMP_FILES)

distclean: clean
	rm -f $(LIB).so $(LIB).a $(_TEST_MODULES)

