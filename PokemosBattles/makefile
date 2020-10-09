PokemonsBattles:MainSecond.o BattleByCategory.o Pokemon.o MaxHeap.o LinkedList.o
	gcc MainSecond.c BattleByCategory.c Pokemon.c MaxHeap.c LinkedList.c -o PokemonsBattles

MainSecond.o:MainSecond.c Pokemon.h BattleByCategory.h
	gcc -c MainSecond.c

BattleByCategory.o:BattleByCategory.c BattleByCategory.h MaxHeap.h LinkedList.h Defs.h
	gcc -c BattleByCategory.c

Pokemon.o:Pokemon.h Pokemon.c Defs.h
	gcc -c Pokemon.c
MaxHeap.o: Defs.h MaxHeap.h MaxHeap.c
	gcc -c MaxHeap.c

LinkedList.o:LinkedList.h LinkedList.c Defs.h
	gcc -c LinkedList.c
