# CHECKERS MAKEFILE #


## CONFIG  ##
srcdir = ./src
bindir = ./bin
outdir = ./output

gccFlags = -g -Wno-write-strings -Werror -lm

target = checkers.exe
objects = main state utils
objectsExpended = $(foreach obj, ${objects}, $(bindir)/${obj}.o) # objects.map(obj => `./${bindir}/${obj}.o`)
extraDeps = objects.h

INPUT_FILE = state.dat


## RULES  ##
.PHONY: run clean

# target rule
$(outdir)/$(target): $(objectsExpended)
	gcc $(gccFlags) -o $(outdir)/$(target) $(objectsExpended) -ldl -lrt

# each `*.o` file is made from its corresponding `*.c` and `*.h`
$(bindir)/%.o: $(srcdir)/%.c $(srcdir)/%.h $(srcdir)/$(extraDeps)
	gcc $(gccFlags) -c $(srcdir)/$*.c -o $(bindir)/$*.o

run: $(outdir)/$(target)
	$(outdir)/$(target) $(INPUT_FILE)

clean:
	rm -f $(outdir)/*.exe
	rm -f $(outdir)/*.dat
	rm -f $(bindir)/*.o
