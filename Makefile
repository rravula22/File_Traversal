CC = gcc
CFLAGS = -Wall -Werror -std=c11

.PHONY: all clean

all: search

search: search.c
	$(CC) $(CFLAGS) $< -o $@

run: search
	./search -S

run_Fflag: search
	./search -f jpg 2

run_eflag: search
	./search -s 200 -e "ls -l" 

run_Eflag: search
	./search -f jpg 2 -E "tar cvf jpg.tar"

clean:
	rm -f search