/**
 * @file tetrix.c
 * @author Alessandro Cecchin (859869@stud.unive.it)
 * @brief 
 * @version 0.9.1
 * @date 2022-08-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief definisco il numero di righe e colonne
 * @def row è il numero di righe
 * @def column è il numero di colonne
 * @def size è la grandezza del vettore tetramino
 */

/** @def row è il numero di righe*/
#define row 15
#define column 10
#define size 4 

/*I pezzi di Tetris: I, J, L, O, S, T, Z
* Logica del gioco
* immagine tetramini posizione 0 : https://upload.wikimedia.org/wikipedia/commons/3/39/Tetrominoes_IJLO_STZ_Worlds.svg
*/ 

/* 
 * 0 = nessun blocco presente
 *                  #
 * # 1 = 1 bloccho  # 2 = 2 blocchi sovrapposti
 *
 * #            
 * #                             #
 * # 3 = 3 blocchi sovrapposti   0  4 = un posto vuoto con blocco sopra                         
 *
 * #                                                   #
 * 0                                                   #
 * 0 5 =due posti vuoti sovrapposti con blocco sopra   0 6 = un posto vuoto con due blocchi sopra
 * 
 * #
 * #
 * #
 * # 7 = 4 blocchi sovrapposti
 */

/* Differenti versioni del tetramino I */
int I_free = 20; /*Indico quanti pezzi sono disponibili di I*/
int I_[size]={1,1,1,1};
int I_180[size]={7,0,0,0};

/* Differenti versioni del tetramino J */
int J_free = 20;
int J_[size]={2,1,1,0};
int J_90[size]={3,5,0,0};
int J_180[size]={4,4,2,0};
int J_270[size]={1,3,0,0};

/* Differenti versioni del tetramino L*/
int L_free = 20;
int L_[size]={1,1,2,0};
int L_90[size]={3,1,0,0};
int L_180[size]={2,4,4,0};
int L_270[size]={1,3,0,0};

/* Differenti versioni del tetramino O */
int O_free = 20;
int O_[size]={2,2,0,0};

/* Differenti versioni del tetramino S */
int S_free = 20;
int S_[size]={1,2,4,0};
int S_90[size]={6,2,0,0};

/* Differenti versioni del tetramino T */
int T_free = 20;
int T_[size]={1,2,1,0};
int T_90[size]={3,4,0,0};
int T_180[size]={4,2,4,0};
int T_270[size]={4,3,0,0};

/* Differenti versioni del tetramino Z */
int Z_free = 20;
int Z_[size]={4,2,1,0};
int Z_90[size]={2,6,0,0};



/*Definisco una variabile per il numero di giocatori*/
typedef enum giocatori
{
    SINGLE_PLAYER = 1,
    MULTI_PLAYER = 2,
    NOT_SELECTED = -99
} giocatori_t;

/* riquadro del piano di gioco */
typedef enum riquadro
{ 
    VUOTO,
    OCCUPATO
} riquadro_t;

/* Piano di gioco */
/* nota: campo_di_gioco e' un puntatore */
typedef riquadro_t *campo_di_gioco;

typedef enum bool_e
{
    FALSE = 0,
    TRUE = 1
} bool_t;

/** @brief Definisco delle costanti per il numero di righe e colonne*/
const int RIGHE = row;
const int COLONNE = column;

/** @brief Definisco variabili globali per l'errore di uscire dalle righe colonne*/

bool_t Perdita_uscita_campo = FALSE;

/** @brief Definisco una variabile globale per il bonus da 3 punti o + per il multiplayer */

bool_t inverti_campo = FALSE;

/**
 * @brief Inizializzatore: inizializza il campo da gioco a VUOTO
 */
void init(campo_di_gioco piano, int riga, int colonna) {
    int r,c;
    for (r=0; r<riga; r++) {
        for (c=0; c<colonna; c++) {
            piano[r*colonna + c] = VUOTO;
        }
    }
}

/**
 * @brief Funzione che uso per visualizzare un ateprima del tetramino scelto dal giocatore
 * 
 * @param tetramino 
 */
void stampa_anteprima(int colonna_scelta_dal_giocatore, int * tetramino){
	
	printf("-ANTEPRIMA scelta 1-\n");

	campo_di_gioco campo_tetramino = (campo_di_gioco) malloc(size*size*sizeof(riquadro_t));
	int fondo_anteprima = (size*size) - (size); /*sono 16 caselle in tutto, un quadrato 4 x 4 , il fondo della prima colonna è il 12 quadrato quindi 4 x 4 - 4*/
	init(campo_tetramino, 4, 4);
	int i,r,c;
	
    for (i=0; i<size; i++) {
            if(*tetramino == 6){
            		campo_tetramino[fondo_anteprima + i] = VUOTO;
            		campo_tetramino[fondo_anteprima + i - size] = OCCUPATO;
					campo_tetramino[fondo_anteprima + i - (size * 2)] = OCCUPATO;
				}
				  else if(*tetramino == 2){
					campo_tetramino[fondo_anteprima + i] = OCCUPATO;
            		campo_tetramino[fondo_anteprima + i - size] = OCCUPATO;
				  }
				    else if(*tetramino == 0){
					}
					  else if(*tetramino == 3){
					  campo_tetramino[fondo_anteprima + i] = OCCUPATO;
				      campo_tetramino[fondo_anteprima + i - size] = OCCUPATO;
				      campo_tetramino[fondo_anteprima + i - (size * 2)] = OCCUPATO;
					  }
					    else if(*tetramino == 4){
				        campo_tetramino[fondo_anteprima + i - size] = OCCUPATO;
					    }
					      else if(*tetramino == 5){
				          campo_tetramino[fondo_anteprima + i - (size * 2)] = OCCUPATO;
					      }
						    else if(*tetramino == 7){
				            campo_tetramino[fondo_anteprima + i] = OCCUPATO;
				            campo_tetramino[fondo_anteprima + i - size] = OCCUPATO;
				            campo_tetramino[fondo_anteprima + i - (size * 2)] = OCCUPATO;
							campo_tetramino[fondo_anteprima + i - (size * 3)] = OCCUPATO;
					        }
					        else if(*tetramino == 1){
					          campo_tetramino[fondo_anteprima + i] = OCCUPATO;
							}
		tetramino++;
        }

		for (r=0; r<size; r++) {
          for (c=0; c<size; c++) {
            riquadro_t riquadro = campo_tetramino[r*size + c];
            if (riquadro==VUOTO)
                printf(" _ ");
            else if (riquadro==OCCUPATO)
                printf(" X ");
            else
                printf(" E "); /*E= error Segno eventuali valori diversi da OCCUPATO e VUOTO*/
           }
        /*vado a capo per creare la griglia*/
        printf("\n");
        }
        for(i = colonna_scelta_dal_giocatore; i < colonna_scelta_dal_giocatore + size; i++)
         printf(" %d ", i);
        printf("\n");
}

int verifica_uscita(int *p, int riga, int colonna, int contatto){
  int i;
  int sottrazione_riga = 0;
  for(i=0; i < size; i++){
    if(*p != 0)
      colonna++;  /*se il puntatore del tetramino non è zero allora vuol dire che occupera anche la prossima colonna*/
    
	/**Se le colonne sono maggiori delle colonne massime allora perdita uscita campo sarà true*/
    if(colonna > COLONNE || colonna < 0){
	  Perdita_uscita_campo = TRUE;
      return -1;
	  }
      /*
      if(*p == 2 || *p == 4 && sottrazione_riga == 0)
        sottrazione_riga = 1;
        else if(*p == 6 || *p == 5 || *p == 3)
          sottrazione_riga = 2;
          else if(*p == 7)
            sottrazione_riga = 3;*/
	if(*p == 1 && sottrazione_riga == 0)
	  sottrazione_riga = 1;
      else if(*p == 2 || *p == 4 )
	    sottrazione_riga = 2;
		else if(*p == 3 || *p == 5 || *p == 6)
		  sottrazione_riga = 3;
		  else if(*p == 7)
		    sottrazione_riga = 4;
    p++;
	}

    riga = riga - sottrazione_riga; /*lo faccio alla fine del ciclo perchè un tetramino non occupa sempre 4 righe ma in base a quanto è alto*/

    if(riga > RIGHE || riga < 0){
	  Perdita_uscita_campo = TRUE;
      return -1;
	}

    return contatto;
}



bool_t verifica_occupata (campo_di_gioco piano, int scelta, int * p){
  
  int i;
  
  for(i = 0; i<size; i++){
    if(*p == 6){
	  if(piano[scelta - COLONNE] == OCCUPATO || piano[scelta - (COLONNE * 2)] == OCCUPATO) return TRUE;
	  }
	  else if(*p == 2){
	    if(piano[scelta - COLONNE] == OCCUPATO) return TRUE;
		}
		else if(*p == 0){;}
		  else if(*p == 3){
			if(piano[scelta - COLONNE] == OCCUPATO ||
			   piano[scelta - (COLONNE * 2)] == OCCUPATO) 
               return TRUE;
			   }
			  else if(*p == 4){
			    if(piano[scelta - COLONNE] == OCCUPATO) return TRUE;
				}
				  else if(*p == 5){
				    if(piano[scelta - (COLONNE * 2)] == OCCUPATO) return TRUE;
					}
					else if(*p == 7){
				      if(piano[scelta] == OCCUPATO ||
				        piano[scelta - COLONNE] == OCCUPATO ||
				        piano[scelta - (COLONNE * 2)] == OCCUPATO ||
						piano[scelta - (COLONNE * 3)] == OCCUPATO) return TRUE;
					      }
					      else if(*p == 1){
					      if(piano[scelta] == OCCUPATO) return TRUE;
						    }
			scelta++;
			p++;
            }
    return FALSE;
}

int contatto (campo_di_gioco piano, int scelta_colonna, int *p){
	
	int * inizio = p; /*salvo l'inizio del mio tetramino*/
	int c, i;
    int riga = 0; /*tengo conto della riga a cui siamo arrvati*/
	int contatto = (RIGHE*COLONNE) - (COLONNE- scelta_colonna) + size; /*aggiungo size perchè se presente un sigolo blocco occupato a fine corsa non lo vede*/
	bool_t found = FALSE;
    /*contatto è uguale all'ultimo numero della colonna, quindi se colonna 2 allora sara 142 il numero contatto*/
	for (c = scelta_colonna; c < contatto; ){
		for(i = 0; i < size ; i++){
		  if(piano[c + i] == OCCUPATO && *p > 0){
		    printf("trovato contatto a : %d\n", c + i);
        	  if(*p == 4 || *p == 6){
                contatto = c;
                found = TRUE;
                }
      	  	    else if (*p == 5 && piano[c + i - (COLONNE * 2)] == OCCUPATO ){ /* Inserisco anche la verifica che sotto il tetramino ci sia un pezzo,  
																				  dove deve appoggiarsi. dovrò fare così anche con P = 4?*/
                  contatto = c - COLONNE;
                  found = TRUE;
                  }
                  else if(*inizio == 7){
				    contatto = c - COLONNE;
				    found = TRUE;
					}
        	        else if (*p == 1 || *p == 2 || *p == 3){
                      contatto = c - COLONNE;
                      found = TRUE;
                      }   
					  else
					    i++; /*ottimizzo il ciclo perchè se c'è uno zero lo sarà anche il prossimo*/
		}
		
		  p++;
		}
		p = inizio; /*ritorno all'inizio del mio tetramino*/
		c = c + COLONNE;
        if(found == FALSE)
          riga++;
	}
    contatto = verifica_uscita(p,riga,scelta_colonna, contatto);
	p = inizio; /*ritorno all'inizio del mio tetramino*/
	if(found == FALSE)
	return contatto - size; /*tolgo size perchè ora non mi serve più per verificare se cella occupata*/
	  else
	    return contatto; /*se ho trovato un contatto uso il valore calcolato*/

}

bool_t salva_tetramino (campo_di_gioco piano, int *p, int scelta_colonna){
	int r, c, i;
	bool_t is_ok = FALSE, occupata = FALSE;
	r = RIGHE;
	c = scelta_colonna;
	int scelta = contatto(piano, scelta_colonna, p);
    int * inizio = p; /*salvo l'inizio del mio tetramino*/

			/******************************************************
			*  scelta rappresenta la selezione attuale nel piano  *
			*  aggiungendo e sottraendo colum mi verifico         *
			*  le diverse posizioni del tetramino.                *
			*******************************************************/

    /*Se la scelta è uguale a -1 vuol dire che il giocatore ha sforato in altezza il campo da gioco*/
	if (scelta == -1){
	  printf("!!! ATTENZIONE !!!:\tHai perso la partita non hai posizionato correttamente un pezzo nel tuo campo di gioco.\n\n");
	  return FALSE;
      }
    if(scelta < 0){
		printf("!!! ATTENZIONE !!!:\tHai perso la partita non hai posizionato correttamente un pezzo nel tuo campo di gioco. seleziona tutto da capo\n\n");
					return FALSE;
				} 		  
    occupata = verifica_occupata(piano, scelta, p);

	if(occupata == FALSE)
	{
	     is_ok == TRUE;

          for(i = 0; i<size; i++){
			/******************************************************
			*  scelta rappresenta la selezione attuale nel piano *
			*  riquadro_1 si trova sopra riquadro nel piano         *
			*  riquadro_2 si trova sopra riquadro_1 nel piano       *
			*******************************************************/
            	if(*inizio == 6){
            		piano[scelta - COLONNE] = OCCUPATO;
            		piano[scelta - (COLONNE * 2)] = OCCUPATO;
				}
				  else if(*inizio == 2){
					piano[scelta] = OCCUPATO;
					piano[scelta - COLONNE] = OCCUPATO;
				  }
				    else if(*inizio == 0){
					}
					  else if(*inizio == 3){
					  piano[scelta] = OCCUPATO;
				      piano[scelta - COLONNE] = OCCUPATO;
				      piano[scelta - (COLONNE * 2)] = OCCUPATO;
					  }
					    else if(*inizio == 4){
				        piano[scelta - COLONNE] = OCCUPATO;
					    }
					      else if(*inizio == 5){
				          piano[scelta - (COLONNE * 2)] = OCCUPATO;
					      }
						    else if(*inizio == 7){
				            piano[scelta] = OCCUPATO;
				            piano[scelta - COLONNE] = OCCUPATO;
				            piano[scelta - (COLONNE * 2)] = OCCUPATO;
							piano[scelta - (COLONNE * 3)] = OCCUPATO;
					        }
					        else if(*inizio == 1){
					          piano[scelta] = OCCUPATO;
							}
            scelta++;
			inizio++;
            }
	   }
	     else{
	  	   printf("\n\n-----ERRORE riquadro OCCUPATE------\n\n");
	  	   return FALSE;
	     }
   return TRUE;        

}

/*scelta rotazione*/

int * rotazione(char code, int colonna_scelta_dal_giocatore){
  int *p;
  int chose = 0;
  bool_t is_ok = FALSE;

  printf("Scelgli la rotazione per questo tetramino %c:\n", code);
  
  if( code == 'j'){
    while( is_ok == FALSE ){
      printf("1. nessuna rotazione\n");
      printf("2. ruotare di 90 gradi\n");
      printf("3. ruotare di 180 gradi\n");
	  printf("4. ruotare di 270 gradi\n");
	  stampa_anteprima(colonna_scelta_dal_giocatore,J_);
	  printf("Scelta: ");
      scanf(" %d", &chose);
		  while (getchar() != '\n') /*salta alla fine della riga*/
		    ;
      printf("\n");
	 
	  switch(chose){
	    case 1: p = J_;
				is_ok = TRUE;
				break;
		case 2: p = J_90;
				is_ok = TRUE;
				break;
		case 3: p = J_180;
				is_ok = TRUE;
				break;
	    case 4: p = J_270;
				is_ok = TRUE;
				break;
			
		default: printf("Codice errato\n");
		}
	}
  }
  else if( code == 'i'){
    while( is_ok == FALSE ){
      printf("1. i orrizzontale\n");
	  printf("2. i verticale\n");
	  stampa_anteprima(colonna_scelta_dal_giocatore,I_);
	  printf("Scelta: ");
      scanf(" %d", &chose);
		  while (getchar() != '\n') /*salta alla fine della riga*/
		    ;
      printf("\n");

	  switch(chose){
	    case 1: p = I_;
				is_ok = TRUE;
				break;
		case 2: p = I_180;
				is_ok = TRUE;
				break;
			
		default: printf("Codice errato\n");
		}
		printf("\n");
	    }
    }
    else if( code == 'o'){
      p = O_;
	  printf("\n");
	  }
      else if( code == 'l'){
        while( is_ok == FALSE ){
          printf("1. nessuna rotazione\n");
          printf("2. ruotare di 90 gradi\n");
          printf("3. ruotare di 180 gradi\n");
	      printf("4. ruotare di 270 gradi\n");
		  stampa_anteprima(colonna_scelta_dal_giocatore,L_);
          printf("Scelta: ");
          scanf(" %d", &chose);
		    while (getchar() != '\n') /*salta alla fine della riga*/
		      ;
          printf("\n");
	      
          switch(chose){
	        case 1: p = L_;
				    is_ok = TRUE;
				    break;
		    case 2: p = L_90;
				    is_ok = TRUE;
				    break;
		    case 3: p = L_180;
				    is_ok = TRUE;
				    break;
	        case 4: p = L_270;
				    is_ok = TRUE;
				    break;
		    default: printf("Codice errato\n");
		   }
        printf("\n");
	    }
      }
      else if( code == 's'){
        while( is_ok == FALSE ){
          printf("1. i orrizzontale\n");
	      printf("2. i verticale\n");
		  stampa_anteprima(colonna_scelta_dal_giocatore,S_);
	      printf("Scelta: ");
          scanf(" %d", &chose);
		    while (getchar() != '\n') /*salta alla fine della riga*/
		      ;
          printf("\n");

	      switch(chose){
	        case 1: p = S_;
				    is_ok = TRUE;
				    break;
		    case 2: p = S_90;
				    is_ok = TRUE;
				    break;
		    default: printf("Codice errato\n");
		    }
		printf("\n");
	    }
      }
      else if( code == 'z'){
        while( is_ok == FALSE ){;
          printf("1. i orrizzontale\n");
	      printf("2. i verticale\n");
		  stampa_anteprima(colonna_scelta_dal_giocatore,Z_);
	      printf("Scelta: ");
          scanf(" %d", &chose);
		    while (getchar() != '\n') /*salta alla fine della riga*/
		      ;
          printf("\n");

	      switch(chose){
	        case 1: p = Z_;
				    is_ok = TRUE;
				    break;
		    case 2: p = Z_90;
				    is_ok = TRUE;
				    break;
		    default: printf("Codice errato\n");
		    }
		printf("\n");
	    }
      }
      else if( code = 't'){
        while( is_ok == FALSE ){
          printf("1. nessuna rotazione\n");
          printf("2. ruotare di 90 gradi\n");
          printf("3. ruotare di 180 gradi\n");
	      printf("4. ruotare di 270 gradi\n");
		  stampa_anteprima(colonna_scelta_dal_giocatore,T_);
          printf("Scelta: ");
          scanf(" %d", &chose);
		    while (getchar() != '\n') /*salta alla fine della riga*/
		      ;
          printf("\n");
	      
          switch(chose){
	        case 1: p = T_;
				    is_ok = TRUE;
				    break;
		    case 2: p = T_90;
				    is_ok = TRUE;
				    break;
		    case 3: p = T_180;
				    is_ok = TRUE;
				    break;
	        case 4: p = T_270;
				    is_ok = TRUE;
				    break;
		    default: printf("Codice errato\n");
		   }
        printf("\n");
	    }
      }
    
	
	return p;
}

void Visualizza_pezzi_disponibili(){
  printf("Sono rimasti i seguenti tetramini:\n");
  printf("I : %d\n", I_free);
  printf("J : %d\n", J_free);
  printf("L : %d\n", L_free);
  printf("O : %d\n", O_free);
  printf("S : %d\n", S_free);
  printf("T : %d\n", T_free);
  printf("Z : %d\n", Z_free);
  printf("Per continuare premi INVIO.\n");
  getchar();
  printf("\n\n");

}

int * scelta (int colonna_scelta_dal_giocatore){
	char code;
	int *p;

	bool_t is_ok = FALSE;
	
	while( is_ok == FALSE){
		printf("Scegli un tetramino tra i seguenti i, j, o, s, l, t o z:\n");

	    printf("Scelta: ");
        scanf(" %c", &code);
		  while (getchar() != '\n') /*salta alla fine della riga*/
		    ;
        printf("\n");
		
		switch(code){
			case 'i': if(I_free <= 0){
						printf("il pezzo %c è finito!\n", code);
                        Visualizza_pezzi_disponibili();
						break;
					  }
					  I_free--;
			          p = rotazione(code, colonna_scelta_dal_giocatore);
					  is_ok = TRUE;
					  break;
			case 'j': if(J_free <= 0){
						printf("il pezzo %c è finito!\n", code);
						Visualizza_pezzi_disponibili();
						break;
					  }
					  J_free--;
					  p = rotazione(code, colonna_scelta_dal_giocatore);
					  is_ok = TRUE;
			          break;
			case 'l': if(L_free <= 0){
						printf("il pezzo %c è finito!\n", code);
						Visualizza_pezzi_disponibili();
						break;
					  }
					  L_free--;
					  p = rotazione(code, colonna_scelta_dal_giocatore);
                      is_ok = TRUE;
			          break;
            case 'o': if(O_free <= 0){
						printf("il pezzo %c è finito!\n", code);
						Visualizza_pezzi_disponibili();
						break;
					  }
					  O_free--;
					  p = rotazione(code, colonna_scelta_dal_giocatore);
                      is_ok = TRUE;
			          break;
            case 's': if(S_free <= 0){
						printf("il pezzo %c è finito!\n", code);
						Visualizza_pezzi_disponibili();
						break;
					  }
					  S_free--;
					  p = rotazione(code, colonna_scelta_dal_giocatore);
                      is_ok = TRUE;
			          break;
            case 't': if(T_free <= 0){
						printf("il pezzo %c è finito!\n", code);
						Visualizza_pezzi_disponibili();
						break;
					  }
					  T_free--;
					  p = rotazione(code, colonna_scelta_dal_giocatore);
                      is_ok = TRUE;
			          break;
            case 'z': if(Z_free <= 0){
						printf("il pezzo %c è finito!\n", code);
						Visualizza_pezzi_disponibili();
						break;
					  }
					  Z_free--;
					  p = rotazione(code, colonna_scelta_dal_giocatore);
                      is_ok = TRUE;
			          break;
			default: printf("!!! ATTENZIONE !!!:\tScelta sbagliata. Selezionane un altra.\n");
		}
		printf("\n");	
	}
	return p;
}

	

void seleziona_tetramino(campo_di_gioco piano, int RIGHE, int COLONNE, int turno)
{
	bool_t is_ok = FALSE;
	int scelta_colonna = -1;
	int tetramino[size] = {1,1,1,1};
	
	while (is_ok == FALSE){

	  printf("Seleziona la colonna dove inserire il tetramini:\n");
	  printf("Scelta: ");
      scanf(" %d", &scelta_colonna);
		while (getchar() != '\n') /*salta alla fine della riga*/
		  ;
      printf("\n");

	  if( turno == 1 && scelta_colonna < 10 && scelta_colonna >= 0)
	    is_ok = TRUE;
	    else if( turno == 2 && scelta_colonna >= 10 && scelta_colonna < 20 )
	      is_ok = TRUE;
	      else{
	      	printf("!!! ATTENZIONE !!!:\tScelta sbagliata. Hai selezionato una colonna sbagliata.\n");
				  }
	        
	  

	  if ( is_ok == TRUE ){
    
	    int *p;
        int i;
        
	    p = scelta(scelta_colonna);
        
		if(turno == 1)
	      is_ok = salva_tetramino(piano, p, scelta_colonna);
        
        /* Nel caso del giocatore 2 diminusco le colonne di 10 perchè la selezione nel campo2 va da 10 a 19 */
        
		if(turno == 2)
		  is_ok = salva_tetramino(piano, p, scelta_colonna - 10);
		
		if(Perdita_uscita_campo != FALSE && is_ok == FALSE)
		  return;


	  }
	}
}

void rimuovi_riga(campo_di_gioco piano, int riga_punto, int colonna){

    int r= 1, c = 0, i;
	/*vado a rimuovere la riga completata*/
    for (c=0; c<COLONNE; c++) {
    riquadro_t riquadro = VUOTO;
    piano[riga_punto*COLONNE + c] = riquadro;
    }

    /*considero colonna per colonna e sposto l'array indietro di una posizione*/
    
	for (c=0; c<COLONNE; c++) {
      for (r=riga_punto-1; r>-1; r--) {
        riquadro_t riquadro_sotto = piano[r*COLONNE + c]; /*Questo passaggio mi permette di memorizzare il valore della riquadro sopra*/
        piano[(r+1)*COLONNE + c] = riquadro_sotto;
                
		}
	 }
}


int calcola_punti(campo_di_gioco piano, int RIGHE, int COLONNE) {
    int r, c;
	int punteggio = 0;
	int bonus = 0;
      for (r=0; r<RIGHE; r++) {
        for (c=0; c<COLONNE; c++) {
          riquadro_t riquadro = piano[r*COLONNE + c];
		  if (riquadro==OCCUPATO){
		    punteggio++;
			}
          if (punteggio >= COLONNE){
			bonus++;
            rimuovi_riga(piano, r, c);
			punteggio = 0;
			}
		}
	punteggio = 0;
    }
	
    if (bonus == 1){
	  return 1;			/*la rimozione di una riga vale un punto*/
	  }
      else if(bonus == 2){
	    return 3; 			/*la rimozione di due righe con un pezzo vale 3 punti*/
		}    
		else if(bonus == 3){
		  inverti_campo = TRUE; /*Quando un giocatore cancella 3 o + righe in multyplayer questa variabile permette di richiamare la funzione inverti*/
		  return 6; 		/*la rimozione di tre righe con un pezzo vale 6 punti*/
		  }
		  else if(bonus >= 4){
			inverti_campo = TRUE;  /*Quando un giocatore cancella 3 o + righe in multyplayer questa variabile permette di richiamare la funzione inverti*/
			return 12; 		/*la rimozione di 4 righe con un pezzo vale 12 punti*/
	        }
			else
              return 0;
}



/*Funzione che uso a scopo di test per visualizzare il numero di caselle nella griglia di gioco*/
void test(campo_di_gioco piano, int RIGHE, int COLONNE){
	printf("\n\n---TEST----\n\n");
	int r, c, count = 0;
    for (r=0; r<RIGHE; r++) {
        for (c=0; c<COLONNE; c++) {
            printf("%4d", count);
            count++;
        }
        printf("\n"); 
    }
}


/****************************************************************
* stampa:	stampa il campo di gioco           				    *
*		assegno alla variabile riquadro il valore del piano     *
*   nella posizione r e c.										*
****************************************************************/

void stampa(campo_di_gioco piano,campo_di_gioco piano2, int RIGHE, int COLONNE, giocatori_t giocatori) {
    int r, c;

    if(giocatori == SINGLE_PLAYER){
        for (r=0; r<RIGHE; r++) {
        for (c=0; c<COLONNE; c++) {
            riquadro_t riquadro = piano[r*COLONNE + c];
            if (riquadro==VUOTO)
                printf(" _ ");
            else if (riquadro==OCCUPATO)
                printf(" X ");
            else
                printf(" E "); /*E= error Segno eventuali valori diversi da OCCUPATO e VUOTO*/
           }
        /*vado a capo per creare la griglia*/
        printf("\n");
        /*stampo il numero della colonna al di sotto del piano di gioco*/
        }
        for (c=0; c<COLONNE; c++)
        printf(" %d ", c);
        printf("\n");
        
    }
      else
      {
        for (r=0; r<RIGHE; r++) {
          for (c=0; c<COLONNE; c++) {
            riquadro_t riquadro = piano[r*COLONNE + c];
            if (riquadro==VUOTO)
                printf(" _ ");
            else if (riquadro==OCCUPATO)
                printf(" X ");
            else
                printf(" E "); /*E= error Segno eventuali valori diversi da OCCUPATO e VUOTO*/
            }
        printf("  ");
        for (c=0; c<COLONNE; c++) {
            riquadro_t riquadro2 = piano2[r*COLONNE + c];
            if (riquadro2==VUOTO)
                printf(" _ ");
            else if (riquadro2==OCCUPATO)
                printf(" X ");
            else
                printf(" E "); /*E= error Segno eventuali valori diversi da OCCUPATO e VUOTO*/
            }
        printf("\n");
    }
    /*stampo il numero della colonna al di sotto del piano di gioco*/
    for (c=0; c<COLONNE; c++)
        printf(" %d ", c);
    printf("  ");
    for (c=COLONNE; c<(COLONNE*2); c++)
        printf(" %d", c);
    printf("\n");
  }
}

/*Usata nel Multiplayer In questa modalità si avranno il doppio di pezzi a disposizione.*/
void raddoppia_tetramini(){
  I_free = I_free * 2;
  J_free = J_free * 2;
  L_free = L_free * 2;
  O_free *= 2;
  S_free *= 2;
  T_free *= 2;
  Z_free *= 2;
  }

 /**************************************************************************************************************	 
  * Nel caso il giocatore cancelli 3 o più linee con una singola mossa, la variabile inverti_campo diventa TRUE *
  * e il campo dell’avversario viene modificato invertendo il corrispondente                                    *
  * numero di linee nella parte più bassa del campo di gioco: una posizione vuota diventa piena e viceversa.    *
  ***************************************************************************************************************/

void inverti_campo_di_gioco(campo_di_gioco campo_giocatore, int RIGHE, int COLONNE){
	int r,c;
	for (r=RIGHE - 3; r<RIGHE; r++) {
        for (c=0; c<COLONNE; c++) {
            riquadro_t riquadro = campo_giocatore[r*COLONNE + c];
			if (riquadro==VUOTO)
                campo_giocatore[r*COLONNE + c] = OCCUPATO;
            else if (riquadro==OCCUPATO)
                campo_giocatore[r*COLONNE + c] = VUOTO;
			}
	}
}

void TEST_INVERTI(campo_di_gioco campo_giocatore, int RIGHE, int COLONNE){
	int r,c;
	for (r=RIGHE - 3; r<RIGHE; r++) {
        for (c=0; c<COLONNE - 2; c++) {
            riquadro_t riquadro = campo_giocatore[r*COLONNE + c];
			if (riquadro==VUOTO)
                campo_giocatore[r*COLONNE + c] = OCCUPATO;
			}
	}
}

/*************************************************//**
* main:	chiede all'utente di immettere un codice, poi           
*	chiama una funzione per eseguire l'azione                   
*   richiesta. Continua fino a quando la variabile              
*   fine_gioco non diventa positiva. stampa messaggio           
*	di errore quando il codice e' sbagliato.                    
*****************************************************/

int main()
{
/** Variabili per utilizzo del menu: 
* @param fine_gioco indica quando il main chiudo il ciclo e finisce il gioco
* @param select indica che voce del menu ho selezionato
* @param turno indica il turno del giocatore attuale
* @param giocatori indica la modalità di gioco multi o single player
*/
  bool_t fine_gioco = FALSE;
  int select = 0; 
  int turno = 1;
  giocatori_t giocatori = NOT_SELECTED;

/** Variabili per il punteggio dei giocatori: 
* @param punteggio_1 indica il punteggio del giocatore 1
* @param punteggio_2 indica il punteggio del giocatore 2
* @param vittoria indica che uno dei due giocatori ha vinto
*/
  int   punteggio_1 = 0;
  int   punteggio_2 = 0;
  bool_t vittoria = FALSE;

/** creo i campi da gioco del giocatore 1 e 2 
 * @param campo_giocatore_1 puntatore al campo del giocatore 1
 * @param campo_giocatore_2 puntatore al campo del giocatore 2
 */
	campo_di_gioco campo_giocatore_1 = (campo_di_gioco) malloc(RIGHE*COLONNE*sizeof(riquadro_t));
	campo_di_gioco campo_giocatore_2 = (campo_di_gioco) malloc(RIGHE*COLONNE*sizeof(riquadro_t));
    
/**
 * @brief  inizializzare i campi da gioco 
 * @see init(campo_di_gioco piano, int riga, int colonna)
 */
  init(campo_giocatore_1, RIGHE, COLONNE);
  init(campo_giocatore_2, RIGHE, COLONNE);
 
  
  /** @brief messaggio di benvenuto */
  printf("\n                                 BENVENUTI IN\n\n");
  printf(" /////////////////// XXXXX  XXXXX  XXXXX  XXXX    X    	X   X  ///////////////////\n");
  printf(" ///////////////////   X    X        X    X   X   X    	 X X   ///////////////////\n");
  printf(" ///////////////////   X    XXXX     X    XXXX    X  XX   X    ///////////////////\n");
  printf(" ///////////////////   X    X        X    X  X    X   	 X X   ///////////////////\n");
  printf(" ///////////////////   X    XXXXX    X    X   X   X  	X   X  ///////////////////\n\n");
  printf("ISTRUZIONI:\n - X-Tetris e' una versione avanzata del gioco originale del Tetris.\n");
  printf(" - Il campo di gioco e\' largo 10 e alto 15 posizioni.\n");
  printf(" - La rimozione di una riga vale 1 punto, la rimozione di due righe con un solo pezzo vale 3 punti,\n tre righe 6 punti, quattro righe 12 punti..\n\n");
  printf("Per iniziare a giocare premi INVIO.\n");
  getchar();
  printf("\n\n");

do
{
  if(giocatori != MULTI_PLAYER && giocatori != SINGLE_PLAYER){
    printf("Ciao :), In quanti giocate?\n\n");
    printf("1. Single Player - Un solo giocatore\n");
    printf("2. Multiplayer - due giocatori\n");
    printf("Scelta: ");
    scanf(" %d", &giocatori);
	  while (getchar() != '\n') /*salta alla fine della riga*/
	    ;
    printf("\n");
	switch (giocatori)
      {
        case 1: printf("Giochi da solo, bene!\n");
              break;
        case 2: printf("Giochi in compagnia, bene!\n");
                TEST_INVERTI(campo_giocatore_1, RIGHE, COLONNE);
                raddoppia_tetramini(); /*Raddopia il numero di tetramini per il multiplayer*/
              break;
        default: printf("!!! ATTENZIONE !!!:\tScelta sbagliata. Selezionane un altra.\n");
		      
      }
    }

if(giocatori == SINGLE_PLAYER)
  {
    punteggio_1 = punteggio_1 + calcola_punti(campo_giocatore_1,RIGHE, COLONNE);
	stampa(campo_giocatore_1,campo_giocatore_2, RIGHE, COLONNE, SINGLE_PLAYER);
  	printf("Turno del giocatore 1.\nPunteggio: %d.\n\n", punteggio_1);

    printf("Menu\n\n");
    printf("1. Seleziona tetramino\n");
    printf("2. Pezzi disponibili\n");
    printf("3. Exit\n");
    
    printf("Scelta: ");
    scanf(" %d", &select);
		while (getchar() != '\n') /*salta alla fine della riga*/
		  ;
    printf("\n");
	
      switch (select)
      {
        case 1: seleziona_tetramino(campo_giocatore_1, RIGHE, COLONNE, 1);
              break;
        case 2: /*altra operazione*/
              break;
        case 3: printf("Arrivederci\n");
	          fine_gioco = TRUE; 
              break;
        default: printf("!!! ATTENZIONE !!!:\tScelta sbagliata. Selezionane un altra.\n"); /*Cambio turno in modo da continuare il ciclo sul giocatore attuale*/
		      
      }
    if(punteggio_1 >= 50){
	   vittoria = TRUE;
       printf("Complimenti hai raggiunto 50 punti, arrivederci!\n");
    }
	/*Perdita per uscita dal campo*/
	if(Perdita_uscita_campo != FALSE){
      vittoria = TRUE;
      printf("HAI PERSO! Sei uscito dal campo arrivederci!\n"); 
      }
	
	if(vittoria != FALSE)
	  fine_gioco = TRUE;
  }

if(giocatori == MULTI_PLAYER)   
{ 
  	if(turno == 1)
	{
	  stampa(campo_giocatore_1,campo_giocatore_2, RIGHE, COLONNE, MULTI_PLAYER);
  	  printf("Turno del giocatore 1.\nPunteggio: %d.\n\n", punteggio_1);
    }
	  else{
		stampa(campo_giocatore_1,campo_giocatore_2, RIGHE, COLONNE, MULTI_PLAYER);
      	printf("Turno del giocatore 2.\nPunteggio: %d.\n\n", punteggio_2);
	    }
    printf("Menu\n\n");
    printf("1. Seleziona tetramino\n");
    printf("2. Pezzi disponibili\n");
    printf("3. Exit\n");

    printf("Scelta: ");
    scanf(" %d", &select);
		while (getchar() != '\n') /*salta alla fine della riga*/
		  ;
    printf("\n");
	
      switch (select)
      {
        case 1: if (turno == 1){
                  seleziona_tetramino(campo_giocatore_1, RIGHE, COLONNE, turno);
                  punteggio_1 = punteggio_1 + calcola_punti(campo_giocatore_1,RIGHE, COLONNE);
                  turno = 2;
                  }
                  else if(turno == 2){
                    seleziona_tetramino(campo_giocatore_2, RIGHE, COLONNE, turno);
                    punteggio_2 = punteggio_2 + calcola_punti(campo_giocatore_2,RIGHE, COLONNE);
   		            turno = 1;
                    }
              break;
        case 2: Visualizza_pezzi_disponibili();
              break;
        case 3: printf("Arrivederci\n");
	          fine_gioco = TRUE; 
              break;

        default: printf("!!! ATTENZIONE !!!:\tScelta sbagliata. Salti il turno!.\n"); /*Cambio turno per scelta sbagliata*/
		         if(turno == 1)
                   turno = 2;
		           else
   		             turno = 1;
              break;
      }
/**Vittoria dopo i 50 punti*/
	  if(punteggio_1 >= 50){
	     vittoria = TRUE;
         printf("Complimenti Giocatore 1 hai raggiunto 50 punti, HAI VINTO! arrivederci!\n");
         }
	     else if(punteggio_2 >= 50){
			vittoria = TRUE;
			printf("Complimenti Giocatore 2 hai raggiunto 50 punti, HAI VINTO! arrivederci!\n");
		 }
	  
/*Perdita per uscita dal campo*/
	  if(Perdita_uscita_campo != FALSE && turno == 2){
	     vittoria = TRUE;
		 printf("%40s%2s%40s\n"," ","**"," ");
         printf("Complimenti Giocatore 2 il Giocatore 1 è uscito dal campo, HAI VINTO! arrivederci!\n");
		 printf("%40s%2s%40s\n"," ","**"," ");
         }
	  if(Perdita_uscita_campo != FALSE && turno == 1){
	     vittoria = TRUE;
		 printf("%40s%2s%40s\n"," ","**"," ");
         printf("Complimenti Giocatore 1 il Giocatore 2 è uscito dal campo, HAI VINTO! arrivederci!\n");
		 printf("%40s%2s%40s\n"," ","**"," ");
         }
/*la variabile inverti_campo diventa TRUE nella funzione calcola punti e quindi chiamo funzione inverti campo da gioco*/
	  if(inverti_campo != FALSE && turno == 1){
	     inverti_campo = FALSE;
		 printf("%40s%2s%40s\n"," ","**"," ");
         printf("Il Giocatore 2 ha cancellato 3 o più righe, Campo giocatore 2 sarà invertito!\n");
		 printf("%40s%2s%40s\n"," ","**"," ");
		 inverti_campo_di_gioco(campo_giocatore_1, RIGHE, COLONNE);
         }
	  if(inverti_campo != FALSE && turno == 2){
	     inverti_campo = FALSE;
		 printf("%40s%2s%40s\n"," ","**"," ");
         printf("Il Giocatore 1 ha cancellato 3 o più righe, Campo giocatore 2 sarà invertito!\n");
		 printf("%40s%2s%40s\n"," ","**"," ");
		 inverti_campo_di_gioco(campo_giocatore_2, RIGHE, COLONNE);
         }

	/*Perdita per fine dei tetramini*/
	  if(I_free <= 0 && J_free <= 0 && L_free <= 0 && O_free <= 0 && S_free <= 0 && T_free <= 0 && Z_free <= 0){
		printf("I tetramini sono finiti!, vince il giocatore con il punteggio più alto : ");
		if(punteggio_1 > punteggio_2)
		  printf("GIOCATORE 1, Complimenti\n");
		  else if(punteggio_2 < punteggio_1)
		    printf("GIOCATORE 2, Complimenti\n");
			else
		      printf("PAREGGIO, Complimenti a tutti e due\n");
		vittoria = TRUE;
	  }

	  if(vittoria != FALSE)
	     fine_gioco = TRUE;
      
}  

} while (!fine_gioco);
  return 0;
}
