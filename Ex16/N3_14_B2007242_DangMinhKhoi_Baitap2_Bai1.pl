thucan(ga).
thucan(tao).
thucan(Y) :- an(X,Y), consong(X),!.
consong(bill).
an(bill, dauphong).
an(john,X) :- thucan(X).
an(sue,X) :- an(bill,X).