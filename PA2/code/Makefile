MAIN = Pwrong

.PHONY: all run clean

all: exe

exe: $(MAIN).o
		g++ -std=c++11 $(MAIN).o -o exe

run:
		./exe $(input) $(output)

clean:
		rm -f *.o
		rm -f exe

$(MAIN).o: $(MAIN).cpp 
		g++ -std=c++11 -c $(MAIN).cpp 


