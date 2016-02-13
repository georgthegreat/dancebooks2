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
	#FIXME: this works improperly for interface files put into include/ folder
	$(CXX) -MM $(SOURCES) > $@

ifneq ($(SOURCES),)
include $(_DEPS_MK)
endif

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

# Install/Uninstall targets

_LOCAL_FILES := $(foreach _FILE,$(FILES),$(shell basename $(_FILE)))

install: $(_LOCAL_FILES)
#Even though uninstall doesn't change local files,
#it requires them to be built to uninstall folders properly
uninstall: $(_LOCAL_FILES)

install:
	for DST_PATH in $(FILES); \
	do \
		SRC_PATH="`basename $$DST_PATH`"; \
		DST_DIRNAME="`dirname $$DST_PATH`"; \
		DST_PATH="$$DST_PATH"; \
		\
		mkdir -p "$$DST_DIRNAME"; \
		cp -r "$$SRC_PATH" "$$DST_DIRNAME"; \
	done \


uninstall:
	for DST_PATH in $(FILES); \
	do \
		SRC_PATH="`basename $$DST_PATH`"; \
		DST_DIRNAME="`dirname $$DST_PATH`"; \
		DST_PATH="$$DST_PATH"; \
		if [ -d $$SRC_PATH ]; \
		then \
			for SINGLE_PATH in `find "$$SRC_PATH" -type f`; \
			do \
				rm -vf "$$DST_DIRNAME/$$SINGLE_PATH"; \
			done; \
		else \
			rm -vf "$$DST_PATH"; \
		fi; \
	done \

# Utility targets
$(_TMP_FILES):
	mkdir -p $@

# Cleaning targets
clean:
	rm -rf $(_TMP_FILES)

distclean: clean
	rm -f $(LIB).so $(LIB).a $(_TEST_MODULES)

