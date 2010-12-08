debug:
	gcc -Wall -std=c99 -lobjc -framework Carbon -framework Cocoa -o bin/ZAgent src/ZCommon.c src/ZCocoa.m src/ZAgent.m
	killall ZAgent || true
	./bin/ZAgent

release:
	gcc -Wall -O2 -std=c99 -lobjc -framework Carbon -framework Cocoa -arch ppc -arch i386 -arch x86_64 -DNS_BUILD_32_LIKE_64 -o bin/ZAgent src/ZCommon.c src/ZCocoa.m src/ZAgent.m
	cp bin/ZAgent ZAgent/ZAgent.app/Contents/MacOS/
	rm -f ZAgent.zip
	zip -r ZAgent.zip ZAgent
	scp ZAgent.zip varsztat:public_html
	killall ZAgent || true
	open ZAgent/ZAgent.app
