CXX ?= g++

# from https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

CXXFLAGS += -Wno-psabi -O3 -std=c++11

ifeq ($(detected_OS),Cygwin)
    CXXFLAGS += -D__USE_MISC -D_DEFAULT_SOURCE -DNO_BLUETOOTH  
endif

SOURCES = goveebttemplogger.cpp

ifeq ($(detected_OS),Linux)
    LDLIBS = -lbluetooth
else
    SOURCES += bthelpers.c
endif

$(info OS: $(detected_OS))
$(info SOURCES: $(SOURCES))

GoveeBTTempLogger/usr/local/bin/goveebttemplogger: $(SOURCES)
	mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) $? -o$@ $(LDLIBS)

deb: GoveeBTTempLogger/usr/local/bin/goveebttemplogger GoveeBTTempLogger/DEBIAN/control GoveeBTTempLogger/usr/local/lib/systemd/system/goveebttemplogger.service
	# Set architecture for the resulting .deb to the actually built architecture
	sed -i "s/Architecture: .*/Architecture: $(shell dpkg --print-architecture)/" GoveeBTTempLogger/DEBIAN/control
	chmod a+x GoveeBTTempLogger/DEBIAN/postinst GoveeBTTempLogger/DEBIAN/postrm GoveeBTTempLogger/DEBIAN/prerm
	dpkg-deb --build GoveeBTTempLogger
	dpkg-name ./GoveeBTTempLogger.deb

install-deb: deb
	apt install ./GoveeBTTempLogger.deb

clean:
	-rm -rf GoveeBTTempLogger/usr/local/bin
	-rm -f GoveeBTTempLogger.deb
	git restore GoveeBTTempLogger/DEBIAN/control

.PHONY: clean deb install-deb
