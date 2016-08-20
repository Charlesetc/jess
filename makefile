# makefile

CC=clang

run: jess regenerate_readme
	./jess text.txt

jess: jess.o

regenerate_readme:
	@cloc . | grep '\bC\b' | sed 's/Header//' | awk '{ total += $$5 } END { print total }' | xargs python ./libread/regenerate.py
