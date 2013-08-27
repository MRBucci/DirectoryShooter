all: create

create: derangedCat.c
	gcc derangedCat.c -o levelScanner
	./levelScanner

clean:
	rm levelScanner
