__start__: obj xmlparser4scene
	./xmlparser4scene

obj:
	mkdir obj

CPPFLAGS=-Wall -g -pedantic -Iinc
LDFLAGS=-Wall

xmlparser4scene: obj/xmlparser4scene.o obj/main.o
	g++ ${LDFLAGS} -o xmlparser4scene  obj/xmlparser4scene.o obj/main.o -lxerces-c

obj/xmlparser4scene.o: src/xmlparser4scene.cpp inc/xmlparser4scene.hh
	g++ -c ${CPPFLAGS} -o obj/xmlparser4scene.o src/xmlparser4scene.cpp

obj/main.o: src/main.cpp inc/scene.hh
	g++ -c ${CPPFLAGS} -o obj/main.o src/main.cpp

clean:
	echo
	rm -f obj/* xmlparser4scene core; echo


cleanall: clean
	find . -name \*~ -print -exec rm {} \;