# CHECKERS MAKEFILE #


## CONFIG ##
srcdir = ./src
bindir = ./bin
outdir = ./output

gccFlags = -g -Wno-write-strings -Werror -lm

target = checkers.exe
objects = main state utils array
objectsExpended = $(foreach obj, ${objects}, $(bindir)/${obj}.o) # objects.map(obj => `./${bindir}/${obj}.o`)
extraDeps = objects.h

INPUT = $(outdir)/state.dat
DEBUG = 0


## RULES ##
.PHONY: run clean

# target rule
$(outdir)/$(target): $(objectsExpended)
	gcc $(gccFlags) -o $(outdir)/$(target) $(objectsExpended)

# each `*.o` file is made from its corresponding `*.c` and `*.h`
$(bindir)/%.o: $(srcdir)/%.c $(srcdir)/%.h $(srcdir)/$(extraDeps)
	gcc $(gccFlags) -c $(srcdir)/$*.c -o $(bindir)/$*.o

run: $(outdir)/$(target)
	$(outdir)/$(target) $(INPUT) $(DEBUG)

clean:
	rm -f $(outdir)/*.exe
	rm -f $(outdir)/*.dat
	rm -f $(bindir)/*.o
	rm -f *.stackdump
