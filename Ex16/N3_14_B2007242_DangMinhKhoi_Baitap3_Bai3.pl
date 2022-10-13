fi(1,1) :- !.
fi(2,1) :- !.
fi(N, KQ) :- N1 is N-1, N2 is N-2, fi(N1, KQ1), fi(N2, KQ2), KQ is KQ1+KQ2.