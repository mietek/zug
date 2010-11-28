debug:
	gcc -Wall -std=c99 -lobjc -framework Carbon -framework Cocoa -o ZAgent ZCommon.c ZCarbon.c ZCocoa.m ZAgent.m
	cp ZAgent ZAgent.app/Contents/MacOS/
	killall ZAgent || true
	./ZAgent

final:
	gcc -Wall -O2 -std=c99 -lobjc -framework Carbon -framework Cocoa -arch ppc -arch i386 -arch x86_64 -DNS_BUILD_32_LIKE_64 -o ZAgent ZCommon.c ZCarbon.c ZCocoa.m ZAgent.m
	cp ZAgent ZAgent.app/Contents/MacOS/
	killall ZAgent || true
	./ZAgent
