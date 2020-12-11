include /opt/fpp/src/makefiles/common/setup.mk

all: libfpp-plugin-data2curl.so
debug: all

OBJECTS_fpp_data2curl_so += src/data2curlPlugin.o src/CURLItem.o
LIBS_fpp_data2curl_so += -L/opt/fpp/src -lfpp
CXXFLAGS_src/data2curlPlugin.o += -I/opt/fpp/src

%.o: %.cpp Makefile
	$(CCACHE) $(CC) $(CFLAGS) $(CXXFLAGS) $(CXXFLAGS_$@) -c $< -o $@

libfpp-plugin-data2curl.so: $(OBJECTS_fpp_data2curl_so) /opt/fpp/src/libfpp.so
	$(CCACHE) $(CC) -shared $(CFLAGS_$@) $(OBJECTS_fpp_data2curl_so) $(LIBS_fpp_data2curl_so) $(LDFLAGS) -o $@

clean:
	rm -f libfpp-plugin-data2curl.so $(OBJECTS_fpp_data2curl_so)
