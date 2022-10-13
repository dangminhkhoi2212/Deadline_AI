dichuyen(1,A,B,C) :- write(A),write(' -> '), write(C), nl,!.
dichuyen(N,A, B, C) :- N1 is N-1,dichuyen(N1, A, C, B), dichuyen(1, A, B, C), dichuyen(N1, B, A, C). 