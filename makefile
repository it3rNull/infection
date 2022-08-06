LDLIBS=-lpcap

all: infection

infection: main.o arphdr.o ethhdr.o mac.o ip.o pch.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f arp-spoof *.o
