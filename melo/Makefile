all: melo_full.c

melo_full.c: melo_mako.c
	python ..\airy\airy.py -i melo_mako.c -o melo_full.c -s states.yml

melo_mako.c: melo.c
	python -m cogapp -d -U -o melo_mako.c melo.c

clean:
	rm melo_full.c melo_mako.c