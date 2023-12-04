# TallerLaboratorio2EstructuraDatos
Solution for the second task of the data structure course. Its a simple implementation of minimax algorithm for connect 4 game.

Time it takes to make the best decision:
Difficulty: Easy
Movement without alpha-beta pruning:
1 = 3ms
2= 8ms
3= 0ms
4= 0ms
5= 8ms
6= 0ms
Movement with alpha-beta pruning:
1= 15ms
2= 5ms
3= 0ms
4= 0ms
5= 0ms
6= 7ms

Difficulty: Difficult
Movement without alpha-beta pruning:
1= 69692 ms
2= 70676 ms
3= 68688 ms
4= 70112 ms
5= 69174 ms
6= 47292 ms
Movement with alpha-beta pruning:
1= 105ms
2= 88ms
3= 140ms
4= 101ms
5= 52ms
6= 26ms

By using alpha-beta pruning, obtaining the best movement is much faster but this causes the first best option found to be chosen, ignoring the fact that there could be a much better solution to improve the speed of response.
