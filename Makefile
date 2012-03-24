
LDLIBS = -lcurl -lboost_regex
CXXFLAGS = -Wall -g -std=c++0x


.PHONY: all

all: dctp-dl

dctp-dl: curl.o dctp-dl.o
	$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@


curl.o: curl.hh

dctp-dl.o: curl.hh


