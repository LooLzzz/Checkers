# MAKEFILE

srcdir = ./src
bindir = ./bin
outdir = ./output

objects = colors main state utils
objectsWithExt = $(foreach obj, ${objects}, ${obj}.o) # prepend `.o` to all objects
objectsWithPath = $(foreach obj, $(objectsWithExt), $(bindir)/$(obj)) # prepend `bindir` to all objects

gccFlags = -g -Wno-write-strings -Werror -lm
target = checkers.exe

INPUT = state.dat


build: mkdirs $(objectsWithExt)
	gcc $(gccFlags) -o $(outdir)/$(target) $(objectsWithPath) -ldl -lrt

mkdirs:
	mkdir -p $(bindir)
	mkdir -p $(outdir)

clean:
	rm -f $(outdir)/*.exe
	rm -f $(outdir)/*.dat
	rm -f $(bindir)/*.o

run: build
	$(outdir)/$(target) $(INPUT)


# each `*.o` file is made from its corresponding `*.c` and `*.h`
%.o: $(srcdir)/%.c $(srcdir)/%.h
	gcc $(gccFlags) -c $(srcdir)/$*.c -o $(bindir)/$*.o
