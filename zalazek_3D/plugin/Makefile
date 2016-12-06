__start__: obj ../libs/Interp4Fly.so 
	@echo "  Tworzenie wtyczki zakonczone."
	@echo

obj:
	mkdir obj

CPPFLAGS=-Wall -fPIC -pedantic -std=c++11 -Iinc -I../inc
LDFLAGS=-Wall -shared


../libs/Interp4Fly.so: obj/Interp4Fly.o
	g++ ${LDFLAGS} -o ../libs/Interp4Fly.so obj/Interp4Fly.o
	@echo
	@echo "  Wtyczka dla polecenia 'Fly' zostala utworzona."
	@echo


obj/Interp4Fly.o: src/Interp4Fly.cpp ../inc/Interp4Command.hh\
                        inc/Interp4Fly.hh 
	g++ -c ${CPPFLAGS} -o obj/Interp4Fly.o src/Interp4Fly.cpp


clean:
	rm -f obj/*

cleanall: clean
	find . -name \*~ -print -exec rm {} \;
