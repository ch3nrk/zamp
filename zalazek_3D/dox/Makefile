__start__:
	doxygen Doxyfile 2> doxy.log
	less -P " Aby zakonczyc, nacisnij klawisz Q " doxy.log

clean:
	rm -fr doc/html doxy.log

cleanall: clean
	find . -name \*~ -print -exec rm {} \;
