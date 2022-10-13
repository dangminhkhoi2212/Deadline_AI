luythua(_, 0, 1) :- !.
luythua(X, Y, Z) :- Y1 is Y-1,luythua(X, Y1, Z1),Z is Z1 * X.