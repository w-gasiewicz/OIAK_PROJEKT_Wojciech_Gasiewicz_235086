default: libtest
clear:
	rm -f libtest
arthmeticlib: arithmeticlib.c calculation.s
	gcc -Wall -O1  -std=c99 arthmeticlib.c calculation.s -o arthmeticlib
libtest: libarthmeticlib.a libtest.c
	gcc -Wall -O1 -std=c99 -g libtest.c calculation.s -L. -larthmeticlib -lcunit -o libtest
libarthmeticlib.a: arthmeticlib.o calculation.s
	ar -rcsv libarthmeticlib.a calculation.s arthmeticlib.o
