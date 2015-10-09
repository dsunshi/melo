all: melo_full.c

melo_full.c: melo_mako.c
	python ..\airy\airy.py -i melo_mako.c -o melo_full.c

melo_mako.c: melo.c
	python -m cogapp -d -o melo_mako.c melo.c

clean:
	rm melo_full.c melo_mako.c