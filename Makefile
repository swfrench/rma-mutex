CC = mpicc
LD = mpicc

CFLAGS = -g -O0 -Iinclude

O = obj
S = src

TOBJ = $O/test.o $O/rma_mutex.o
TBIN = xtest

all: $(TBIN)

$O/%.o: $S/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TBIN): $O $(TOBJ)
	$(LD) $(LDFLAGS) $(TOBJ) -o $@

$O:
	mkdir -p $O

.PHONY: clean
clean:
	rm -rf $O

.PHONY: distclean
distclean: clean
	rm -f $(TBIN)
