TEST-CXX ?= $(CXX)
_TEST-CXXFLAGS := \
	-std=c++11 \
	-Wall -Werror \
	$(TEST-CXXFLAGS) \
	-Iinclude \
	$(addprefix -I,$(TEST-INCLUDES)) \

_TEST-LDFLAGS := \
	$(TEST-LDFLAGS) \
	-pthread \
	-L. \
	-l:$(LIB).so \
	-lboost_unit_test_framework \
	$(addprefix -l,$(TEST-LIBS)) \

# Test execution targets
_TEST_MODULES := $(TESTS:%.cpp=%)
_TEST_TARGETS := $(_TEST_MODULES:%=%-run)
_TEST_ENV := \
	LD_LIBRARY_PATH=. \

define _COMPILE_TEST_MODULE
$(1): $(1).cpp $$(LIB).so
	$$(TEST-CXX) \
		$$(_TEST-CXXFLAGS) -o $$@ $$< \
		$$(_TEST-LDFLAGS)
endef

define _RUN_TEST_MODULE
$(1)-run: $(1) $$(LIB).so
	$$(_TEST_ENV) $(1)
endef

$(foreach _TEST_MODULE,$(_TEST_MODULES),$(eval $(call _COMPILE_TEST_MODULE,$(_TEST_MODULE))))
$(foreach _TEST_MODULE,$(_TEST_MODULES),$(eval $(call _RUN_TEST_MODULE,$(_TEST_MODULE))))

test: $(_TEST_TARGETS);

distclean: distclean-test

distclean-test:
	rm -f $(_TEST_MODULES)
