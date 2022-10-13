nam(X).
nu(X).
cha(X, Y) :- nam(X).
me(X, Y) :- nu(X).
vo(X, Y) :- nu(X), nam(Y).
chong(X, Y) :- nam(X), nu(Y).
tuoi(X, Y)./*tuoi X lon hơn Y*/
anh(X, Y) :- nam(X),vo(K, Z),chong(Z, K), cha(Z, X), cha(Z, Y), me(K, X), me(K, Y), tuoi(X, Y), X <> Y.
chi(X, Y) :- gai(X),vo(K, Z),chong(Z, K), cha(Z, X), cha(Z, Y), me(K, X), me(K, Y), tuoi(X, Y), X <> Y.
emtrai(X, Y) :- nam(X),anh(Y,X);chi(Y,X). /* ; là phép toán hoặc */
emgai(X, Y) :- gai(X), anh(Y,X);chi(Y,X).
chu(X, Y) :- cha(Z, X), emtrai(X, Z). 
bac(X, Y) :- cha(Z, X), anh(X, Z). 
cau(X, Y) :- me(Z, X), emtrai(X, Z), chi(Z, Y). 
co(X, Y) :- cha(Z, Y), emgai(X, Y).
di(X, Y) :- me(Z, Y), emgai(X, Z).
mo(X, Y) :- cau(Z, Y), vo(X, Z).
thim(X, Y) :- chu(Z, Y), vo(X, Z).
bacgai(X, Y) :- bac(Z, Y), vo(X, Z).
ongnoi(X, Y) :- cha(Z, Y), cha(X, Z).
ongngoai(X, Y) :- me(Z, Y), me(X, Z).
banoi(X, Y) :- cha(Z, Y), me(X,Z).
bangoai(X, Y) :- me(Z, Y), me(X, Z).
chavo(X, Y) :- vo(Z, Y), cha(X, Z).
mevo(X, Y) :- vo(Z, Y), me(X, Z).
chachong(X, Y) :- chong(Z, Y), cha(X, Z).
mechong(X, Y) :- chong(Z, Y), me(X, Z).