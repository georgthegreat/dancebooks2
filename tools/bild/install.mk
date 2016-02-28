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

