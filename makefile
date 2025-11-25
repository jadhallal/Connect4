CXX = gcc
CPPFLAGS = -Wall -g
PROGS = game

all: $(PROGS)

game: connect4.o easyBot.o mediumBot.o hardbot.o functions.o winCheck.o
	$(CXX) $(CPPFLAGS) -o $@ $^

connect4.o: connect4.c functions.h easyBot.h mediumBot.h hardbot.h winCheck.h
	$(CXX) $(CPPFLAGS) -c $<

easyBot.o: easyBot.c easyBot.h
	$(CXX) $(CPPFLAGS) -c $<

mediumBot.o: mediumBot.c mediumBot.h
	$(CXX) $(CPPFLAGS) -c $<

hardbot.o: hardbot.c hardbot.h
	$(CXX) $(CPPFLAGS) -c $<

functions.o: functions.c functions.h
	$(CXX) $(CPPFLAGS) -c $<

winCheck.o: winCheck.c winCheck.h
	$(CXX) $(CPPFLAGS) -c $<

clean:
	rm -rf *.o *~ *.dSYM $(PROGS)
