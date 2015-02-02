
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

update: .git
	git submodule init
	git submodule update


.git:
	@echo '`make update` only works on a cloned git repository - cf. README.md'
	@false
