CC=g++
CFLAGS=-Isha1
SHA1=sha1.o

ODIR=obj
_OBJ=$(SHA1)
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

EXE=Generate B F

all: $(EXE)

$(EXE): %: $(ODIR)/%.o $(SHA1) $(OBJ)
	$(CC) $< $(OBJ) -o $@

$(SHA1):
	cd sha1 && $(MAKE) $@
	cp sha1/$@ $(ODIR)

$(ODIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(EXE)
	rm -f $(ODIR)/*.o
	cd sha1 && $(MAKE) clean