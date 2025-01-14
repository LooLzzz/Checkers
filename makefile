# CHECKERS MAKEFILE #


## CONFIG ##
srcdir = ./src
bindir = ./bin
outdir = ./output

gccFlags = -g -Wno-write-strings -Werror -lm

target = checkers.exe
objects = main state utils array validation
headerDeps = objects

objectsExpanded = $(foreach obj, ${objects}, $(bindir)/${obj}.o) # objects.map(obj => `./${bindir}/${obj}.o`)
headerDepsExpanded = $(foreach header, ${headerDeps}, $(srcdir)/${header}.h)

INPUT = $(outdir)/state.dat
DEBUG = 0


## RULES ##
.PHONY: run clean

# target rule
# link all objects together
$(outdir)/$(target): $(objectsExpanded)
	gcc $(gccFlags) -o $(outdir)/$(target) $(objectsExpanded)

# each `*.o` file is made from its corresponding `*.c` and `*.h`
# -c means compile only (no linking)
$(bindir)/%.o: $(srcdir)/%.c $(srcdir)/%.h $(headerDepsExpanded)
	gcc $(gccFlags) -c $(srcdir)/$*.c -o $(bindir)/$*.o

run: $(outdir)/$(target)
	$(outdir)/$(target) $(INPUT) $(DEBUG)

clean:
	rm -f $(outdir)/*.exe
	rm -f $(outdir)/*.dat
	rm -f $(bindir)/*.o
	rm -f *.stackdump
