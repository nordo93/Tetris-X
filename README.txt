X-tetris
X-Tetris is an advanced version of the original Tetris game


X-Tetris è una versione avanzata del gioco originale del Tetris. Per le regole del tetris puoi visitare la pagine
Wikipedia https://it.wikipedia.org/wiki/Tetris.

Autore Alessandro Cecchin
Progetto per il corso IaP dell’Università di Venezia
Features

*Single Player

A differenza del Tetris originale, in X-Tetris il giocatore ha inizialmente a disposizione 20 pezzi per ciascun tipo, detti tetramino, e una mossa consiste nello scegliere quale pezzo giocare,dove farlo cadere e con quale rotazione.
Il campo di gioco è largo 10 e alto 15 posizioni. Una volta posizionato un tetramino, se una o più righe orizzontali vengono riempite per intero, queste righe vengono cancellate come nel tetris originale.
La rimozione di una riga vale 1 punto, la rimozione di due righe con un solo pezzo vale 3 punti, tre righe 6
punti, quattro righe 12 punti.
Il gioco termina quando finiscono i pezzi o il giocatore non riesce a posizionare un tetramino nel campo di gioco rispettando il limite di altezza e larghezza. oppure supera i 100 punti.

*Multi Player

Il programma supporta una seconda modalità di gioco, selezionabile dal menu iniziale, in cui due giocatori giocano a turni alterni ciascuno nel proprio campo di gioco ma pescando dallo stesso insieme di tetramini.
In questa modalità si avranno il doppio di pezzi a disposizione. Nel caso in cui un giocatore cancelli una
o due linee simultaneamente, il gioco procede come per il caso single player. Nel caso il giocatore cancelli
3 o più linee con una singola mossa, il campo dell’avversario viene modificato invertendo il corrispondente
numero di linee nella parte più bassa del campo di gioco: una posizione vuota diventa piena e viceversa.
Un giocatore perde la partita se non posiziona correttamente un pezzo nel proprio campo di gioco. Se i pezzi
finiscono vince il giocatore con il punteggio più alto.
La modalità multi-player deve prevede la possibilità di giocare player vs. player

la costruzione del campo di gioco dei tetramini e di ogni cosa
ha solamente utilizzato la funzione printf e le librerie standard di C

*Logica di costruzione campo

Un puntatore punta ad una struttura composta da RIGHE x COLONNE riquadri.
ad ogni turno il puntatore viene usato per stampare l’intero campo di gioco
e per riempirlo in base alla colonna e al tetramino scelto.

[riquadro_t] - enumeratore che indica la singola casella del campo di gioco
[* campo _ di _ gioco] - putatore che punta alla struttura sopra
[I_90] - esempio di tetramino, ogni tetramino è un vettore di 4 elementi.


*Logica di costruzione del tetramino

tipo di pezzi del Tetris: I, J, L, O, S, T, Z

immagine tetramini posizione 0 : https://upload.wikimedia.org/wikipedia/commons/3/39/Tetrominoes_IJLO_STZ_Worlds.svg

cosa vogliono dire i numeri all’interno degli array dei tetramini:
1 = 1 bloccho occupato; 2 = 2 blocchi sovrapposti; 3 = 3 blocchi sovrapposti
4 = 1 blocco vuoto e sopra di esso uno pien0; 5 = due blocchi vuoti e uno sopra di essi pieno
6 = un blocco vuoto e sopra di esso due pieni; 7 = 4 blocchi sovrapposti uno all’altro

I_[4] = {1,1,1,1}
vuol dire che il tetramino è in posizione 0 e occupa un solo blocco in altezza per ogni colonna
X X X X
0 1 2 3

J_90[4]={3,5,0,0}
il tetramino è in poszione ruotata di 90 gradi e occupa con la prima casella 3 blocchi in altezza,
nella seconda casella sempre 3 blocchi i altezza ma 2 sono vuoti e 1 è pieno e occupato.
i restanti a 0 sono invece colonne libere
X X _ _
X _ _ _
X _ _ _
0 1 2 3