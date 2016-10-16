LIB := libhda-db

SOURCES := \
	connection.cpp \
	pool.cpp \
	settings.cpp \
	transaction.cpp \

LIBS := \
	pqxx \

TESTS := \
	tests/test_connection_pool.cpp \

TEST-LDFLAGS += \
	-lpqxx

FILES := \
	/usr/lib/$(LIB).so \
	/usr/include \

include  /usr/share/hda/bild/bild.mk
