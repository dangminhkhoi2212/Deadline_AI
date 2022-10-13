nguoi(marcus).
pompeian(marcus).
lama(X):- pompeian(X).
lanhchua(ceasar).
amsat(X, Y) :- lama(X),lanhchua(Y),not(trungthanh(X,Y)).
not(trungthanh(marcus, ceasar)).