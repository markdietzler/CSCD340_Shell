lab8:	cscd340Lab8.c ./tokenize/makeArgs.o ./utils/myUtils.o ./pipes/pipes.o ./comlist/comlist.o ./aliaslist/aliaslist.o
	gcc -g cscd340Lab8.c ./tokenize/makeArgs.o ./utils/myUtils.o ./pipes/pipes.o ./comlist/comlist.o ./aliaslist/aliaslist.o -o mssh

makeArgs.o:	./tokenize/makeArgs.c ./tokenize/makeArgs.h
	gcc -g -c ./tokenize/makeArgs.c

myUtils.o:	./utils/myUtils.o ./utils/myUtils.h
	gcc -g -c ./utils/myUtils.c

pipes.o:	./pipes/pipes.c ./pipes/pipes.h
	gcc -g -c ./pipes/pipes.c

comlist.o:	./comlist/comlist.c ./comlist/comlist.h
	gcc -g -c ./comlist/comlist.c

alist.o:	./aliaslist/aliaslist.c ./aliaslist/aliaslist.h
	gcc -g -c ./aliaslist/aliaslist.c

clean:
	rm ./pipes/pipes.o	
	rm ./utils/myUtils.o
	rm ./aliaslist/aliaslist.o
	rm ./tokenize/makeArgs.o
	rm ./comlist/comlist.o
	rm mssh
