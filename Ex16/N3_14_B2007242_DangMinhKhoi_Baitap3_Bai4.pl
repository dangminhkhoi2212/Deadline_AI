tohop(0,N,1) :- !.
tohop(K,K,1) :- !.
tohop(K, N, KQ) :- K1 is K-1, N1 is N-1,tohop(K1,N1, KQ1),tohop(K, N1, KQ2), KQ is KQ1 + KQ2. 