
LDLIBS = -lcurl -lboost_regex
CXXFLAGS = -Wall -g -std=c++0x

TEMP = dctp-dl.o ccurl/curl.o dctp-dl

.PHONY: all

all: dctp-dl

dctp-dl: ccurl/curl.o dctp-dl.o
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@


ccurl/curl.o: ccurl/curl.hh

dctp-dl.o: ccurl/curl.hh


.PHONY: clean

clean:
	rm -f $(TEMP)

.PHONY: update

update:
	git submodule init
	git submodule update

