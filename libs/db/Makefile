LIB := libhda-db
INCLUDES := \
	/usr/include/pqxx \

SOURCES := \
	connection.cpp \
	pool.cpp \
	settings.cpp \
	transaction.cpp \

LIBS := \
	pqxx \

TESTS := \
	tests/test_connection_pool.cpp \

TEST-CXXFLAGS := \
	-I/usr/include/pqxx \

TEST-LDFLAGS += \
	-lpqxx

FILES := \
	/usr/lib/$(LIB).so \
	/usr/include \

include  /usr/share/hda/bild/bild.mk
