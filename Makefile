# Barebones, for now all this does is build XmAxy and then AxY FTP.

.PHONY: all clean cleanall

all: xmaxy src

xmaxy:
	@echo "Building XmAxY then AxY FTP."
	$(MAKE) -C xmaxy

src
	$(MAKE) -C src

clean:
	$(MAKE) -C xmaxy clean
	$(MAKE) -C src clean

doc:
	$(MAKE) -C doc
