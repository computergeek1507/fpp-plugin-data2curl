include /opt/fpp/src/makefiles/common/setup.mk

all: libfpp-tasmota-plugin.so
debug: all

OBJECTS_fpp_tasmota_so += src/FPPTasmotaPlugin.o src/TasmotaBulb.o
LIBS_fpp_tasmota_so += -L/opt/fpp/src -lfpp
CXXFLAGS_src/FPPTasmotaPlugin.o += -I/opt/fpp/src

%.o: %.cpp Makefile
	$(CCACHE) $(CC) $(CFLAGS) $(CXXFLAGS) $(CXXFLAGS_$@) -c $< -o $@

libfpp-tasmota-plugin.so: $(OBJECTS_fpp_tasmota_so) /opt/fpp/src/libfpp.so
	$(CCACHE) $(CC) -shared $(CFLAGS_$@) $(OBJECTS_fpp_tasmota_so) $(LIBS_fpp_tasmota_so) $(LDFLAGS) -o $@

clean:
	rm -f libfpp-tasmota-plugin.so $(OBJECTS_fpp_tasmota_so)
