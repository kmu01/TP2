CFLAGS = -g -Wall -Wconversion -Wtype-limits -pedantic
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

# BORRAR

REMOVE = zyxcba.o strutil/strutil.o hash/hash.o heap/heap.o cola/cola.o lista/lista.o abb/abb.o abb/pila.o zyxcba_lib.o


# COMPILAR OBJETOS

zyxcba.o: zyxcba.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

strutil/strutil.o: strutil/strutil.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

hash/hash.o: hash/hash.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

heap/heap.o: heap/heap.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

cola/cola.o: cola/cola.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

lista/lista.o: lista/lista.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/abb.o: abb/abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@
	
abb/pila.o: abb/pila.c
	@- $(CC) -c $(CFLAGS) $^ -o $@	

zyxcba_lib.o: zyxcba_lib.c
	@- $(CC) -c $(CFLAGS) $^ -o $@


# COMPILAR EJECUTABLES

zyxcba: zyxcba.o strutil/strutil.o hash/hash.o heap/heap.o cola/cola.o lista/lista.o abb/abb.o abb/pila.o zyxcba_lib.o
	@- $(CC) $(CFLAGS) $^ -o $@
	
# LIMPIAR

clean:
	@- rm -r $(REMOVE)
