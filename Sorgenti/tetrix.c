/**
 * @file tetrix.c
 * @author Alessandro Cecchin (859869@stud.unive.it)
 * @brief Include il main e tutte le funzioni necessarie allo svolgimento della partita.
 * @version 0.9.1
 * @date 2022-08-19
 * 
 * @copyright Copyright © 2007 Free Software Foundation, Inc. <https://fsf.org/>
 * */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/** @include tetramini.h @see tetramini.h*/
#include "tetramini.h"

/** @brief Definisco delle costanti per il numero di righe e colonne*/
const int RIGHE = row;
const int COLONNE = column;

/** @brief Definisco un enumeratore per il numero di giocatori e modalità di gioco*/
typedef enum giocatori
{  
  SINGLE_PLAYER = 1,  /**< Quando la modalità selezionata è giocatore singolo*/
  MULTI_PLAYER = 2,   /**< Quando la modalità selezionata è multigiocatore*/
  NOT_SELECTED = -99,  /**< Quando la modalità non è stata selezionata*/
  CPU_PLAYER = 3       /**< Quando la modalità è CPU vs PLAYER*/
} giocatori_t;

/** @brief  Definisco un enumeratore per sapere lo stato della casella del piano di gioco*/
typedef enum riquadro
{ 
  VUOTO,    /**< se la casella non contiene un pezzo di tetramino*/
  OCCUPATO  /**< quando la casella contiene un pezzo del tetramino*/
} riquadro_t;

/** @brief uso questo enum per supportare il tipo bool anche su standard C89*/
typedef enum bool_e
{
  FALSE = 0,  /**< FALSE = 0*/
  TRUE = 1    /**< TRUE = 1*/
} bool_t;

/** @brief campo_di_gioco e' un puntatore che puntera poi al campo di gioco*/
typedef riquadro_t *campo_di_gioco;

/** @brief Definisco variabile globale per verificare uscita dal campo (sia da righe che colonne)
*   Uguale a TRUE quando un giocatore esce dal campo e quindi dovrà perdere la partita. */
bool_t Perdita_uscita_campo = FALSE;

/** @brief Definisco una variabile globale per il bonus da 3 punti o piu' per il multiplayer
*   Uguale a TRUE quando uno dei due giocatori cancella 3 righe o piu' contemporanamente.
*   Inverto quindi il campo dell'avversario @see inverti_campo_di_gioco*/
bool_t inverti_campo = FALSE;

/**
 * @brief Inizializza il campo da gioco a tutto vuoto @see riquadro
 * 
 * @param piano viene passato il puntatore al piano di gioco, quello del giocatore 1 o 2
 * @param riga viene passato il numero di righe del campo
 * @param colonna viene passato il numero di colonne del campo
 */
void init(campo_di_gioco piano, int riga, int colonna) {
  int r,c;
  for (r=0; r<riga; r++){
    for (c=0; c<colonna; c++){
      piano[r*colonna + c] = VUOTO; /*il campo da gioco è grande 10 * 15 caselle = 149 spazi*/ 
    }
  }
}

/**
 * @brief Funzione che uso per visualizzare un ateprima del tetramino scelto dal giocatore 
 * @param tetramino punta al primo elemento dell'array del tetramino che ha selezionato il giocatore
 * @param colonna_scelta_dal_giocatore la colonna selezionata dal giocatore viene passata alla funzione
 */
void stampa_anteprima(int colonna_scelta_dal_giocatore, int * tetramino){
  printf("-ANTEPRIMA scelta 1-\n");

  campo_di_gioco campo_tetramino = (campo_di_gioco) malloc(size*size*sizeof(riquadro_t));
  int fondo_anteprima = (size*size) - (size); /*sono 16 caselle in tutto, un quadrato 4 x 4 , il fondo della prima colonna è il 12 quadrato quindi 4 x 4 - 4*/
  init(campo_tetramino, 4, 4);
  int i,r,c;
  /*Ciclo for che analizza l'array del tetramino passato dalla funzione
  * Imposta a OCCUPATO o VOUTO il campo_tetramino in base al numero passato. guardare README.md per le regole di costruzione*/
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
        
  /*Ciclo for per stampare a video l'antetprima, simile alla funzione stampa*/
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
  /** @brief stampo un anteprima delle colonne su cui scende il tetramino esempio:\n
   * OUTPUT:\n
   *  _  _  _  _ \n
   *  X  _  _  _ \n
   *  X  X  X  _ \n
   *  6  7  8  9\n*/
  for(i = colonna_scelta_dal_giocatore; i < colonna_scelta_dal_giocatore + size; i++)
    printf(" %d ", i);
  printf("\n");
}

/**
 * @brief verifica che il tetramino inserito non superi il limite di colonne  e righe massime
 * 
 * @param p punta al primo elemento dell'array del tetramino che ha selezionato il giocatore
 * @param righe_rimanenti il numero di righe rimaste vuote, quindi un tetramino I in verticale non ci starà se le righe rimaneti sono solo 3
 * @param colonna questa indica la colonna scelta dal giocatore e aumenta di valore in base a quanto occupa il tetrmaino in termini di larghezza
 * @param contatto questo è dove il tetramino è appogiato, lo uso come valore di entrata e uscita per verificare gli errori, se il valore cambia allora il tetramino è uscito dallo spazio di gioco
 * @return ritrona di nuovo contatto se non ci sono errori, sennò ritorna -1
 * @attention guardare note sotto, aggiungo questa condizione perchè in caso di tetramini come l a 90 gradi il primo valore del vettore è più alto del secondo
 */
int verifica_uscita(campo_di_gioco piano,int *p, int righe_rimanenti, int colonna, int contatto){
  int i;
  int sottrazione_riga = 0;
  /*Ciclo for che analizza l'array del tetramino passato dalla funzione
  * Verifico se il tetramino ha un valore diverso da 0, quindi se occupa una colonna
  */
  if(contatto <= 0)
    printf("");
  for(i=0; i < size; i++){

    if(*p != 0)
      colonna++;  /*se il puntatore del tetramino non è zero allora vuol dire che occupera anche la prossima colonna*/

  /** @see @param Perdita_uscita_campo: quando è a TRUE indica l'uscita dal campo e il giocatore perde*/
    if(colonna > COLONNE || colonna < 0){
      Perdita_uscita_campo = TRUE;
      return -1;
	  }
    
	/**
	 * @brief la variabile contatto indica dove il tetramino comincierà ad essere disegnato, quindi se quella casella è occupata:
	 * vuol dire che il tetramino è appoggiato ad un altro e quindi il numero di righe che occupa è minore. esempio:
	 * OUTPUT:\n
	 * X  X  _  TETRAMINO NON APPOGIATO AD UN ALTRO \n
     * _  X  _ \n
     * _  X  _ \n
	 * X  X  X \n
	 * */
    if(*p == 1 && sottrazione_riga == 0) /** @note if(*p == 1 && sottrazione_riga == 0)*/
	  sottrazione_riga = 1;
	  else if(( *p == 2 && *(p - 1) == 4 && *(p - 2) != 4 ) || ( *p == 4 && *(p - 1) == 2 ) && piano[contatto + 2] == OCCUPATO ) /*Per il caso di t_180 appoggiato , *(p - 2) != 4 aggiunto per l_180 su superfice piana*/
	    sottrazione_riga = 1;
	    else if(( *p == 3 && *(p - 1) == 4 ) && piano[contatto] == OCCUPATO ) /*Per il caso di t_270 appoggiato*/
	      sottrazione_riga = 2;
	      else if(( *p == 4 && *(p - 1) == 3 ) && piano[contatto + 1] == OCCUPATO ) /*Per il caso di t_90 appoggiato*/
	        sottrazione_riga = 2;
	        else if(( *p == 4 && *(p - 1) == 3 ) && piano[contatto] != OCCUPATO ) /*Per il caso di t_270*/
	          sottrazione_riga = 3;
            else if(((*p == 2 || *p == 4) && *(p - 1) != 6 ) && piano[contatto] != OCCUPATO ) /*&& *(p - 1) == 6  per s verticale che occcupa 3 spazi*/
	            sottrazione_riga = 2;
		            else if(( *p == 5 && *(p - 1) == 3 ) && piano[contatto - COLONNE + 1] == OCCUPATO ) /*Per J_180 appoggiato e j_90*/
		              sottrazione_riga = 1;
				          else if((*p == 6 && *(p - 1) == 2) && piano[contatto + 1] == OCCUPATO) /*aggiunto per z_90 verticale che poggia su tetramino*/
				            sottrazione_riga = 2;
		                else if((*p == 3 || *p == 5 || *p == 6) && piano[contatto - COLONNE] != OCCUPATO )
 		                  sottrazione_riga = 3;
		                  else if(*p == 7)
		                    sottrazione_riga = 4;
			                  else if((*p == 4 || *p == 5) && piano[contatto - COLONNE] != VUOTO )
			                    sottrazione_riga = 1;
			                    else if(*p == 6 && piano[contatto] != VUOTO)
			                      sottrazione_riga = 2;
				                    else if(*p == 4 && piano[contatto] != VUOTO) /*per t_180 a sx*/
				                      sottrazione_riga = 1;
							                else if((*p == 2 && *(p - 1) == 6 ) && piano[contatto] == OCCUPATO)
 							                  sottrazione_riga = 2;
				  
    p++;
	}

  righe_rimanenti = righe_rimanenti - sottrazione_riga; /*lo faccio alla fine del ciclo perchè un tetramino non occupa sempre 4 righe ma in base a quanto è alto*/

  if(righe_rimanenti > RIGHE || righe_rimanenti < 0){
    Perdita_uscita_campo = TRUE;
    return -1;
	}

  return contatto;
}


/**
 * @brief Verifica se il tetramino inserito non vada a scrivere su una casella già occupata
 * @attention Questa funzione non viene più usata nel codice, perchè la logica porterà sempre il tetramino su una casella non occupata.
 * @param piano punta al campo di gioco del giocatore 1 o 2 in base al turno
 * @param scelta
 * @param p punta al primo elemento dell'array del tetramino che ha selezionato il giocatore
 * @return bool_t TRUE se la casella è occupata , FALSE se il ciclo finisce e quindi nessuna casella è occupata
 */
bool_t verifica_occupata (campo_di_gioco piano, int scelta, int * p){
  
  int i;
  bool_t occupata = FALSE;
  for(i = 0; i<size; i++){
    if(*p == 6){
	  if(piano[scelta - COLONNE] == OCCUPATO || piano[scelta - (COLONNE * 2)] == OCCUPATO) occupata = TRUE;
	  }
	  else if(*p == 2){
	    if(piano[scelta - COLONNE] == OCCUPATO) occupata = TRUE;
		}
		else if(*p == 0){;}
		  else if(*p == 3){
			if(piano[scelta - COLONNE] == OCCUPATO ||
			   piano[scelta - (COLONNE * 2)] == OCCUPATO) 
         occupata = TRUE;
			   }
			  else if(*p == 4){
			    if(piano[scelta - COLONNE] == OCCUPATO) occupata = TRUE;
				}
				  else if(*p == 5){
				    if(piano[scelta - (COLONNE * 2)] == OCCUPATO) occupata = TRUE;
					}
					else if(*p == 7){
				      if(piano[scelta] == OCCUPATO ||
				        piano[scelta - COLONNE] == OCCUPATO ||
				        piano[scelta - (COLONNE * 2)] == OCCUPATO ||
						    piano[scelta - (COLONNE * 3)] == OCCUPATO) occupata = TRUE;
					      }
					        else if(*p == 1){
					          if(piano[scelta] == OCCUPATO) occupata = TRUE;
						        }
			scelta++;
			p++;
            }
    return occupata;
}

/**
 * @brief Questa funzione parte ad analizzare tutta la colonna che il giocatore ha scelto,
 * più le 3 colonne successive a quella scelta, questo perchè un tetramino occupa fino a 4 colonne per esempio @see I_
 * Quando viene trovato una casella occupata e il valore di *p è diverso da zero allora viene restituito dove dovrà poggiare il tetramino.
 * @param piano punta al campo di gioco del giocatore 1 o 2 in base al turno
 * @param scelta_colonna  indica la colonna scelta dal giocatore all'interno del main
 * @param p punta al primo elemento dell'array del tetramino che ha selezionato il giocatore
 * @return int contatto indica dove il tetramino è caduto, in base alla colonna scelta dal giocatore
 */
int contatto (campo_di_gioco piano, int scelta_colonna, int *p){
	
  int * inizio = p; /*salvo l'inizio del mio tetramino*/
  int c, i;
  int riga = 0; /*tengo conto della riga a cui siamo arrvati*/
  int contatto = (RIGHE*COLONNE) - (COLONNE- scelta_colonna) + size; /*aggiungo size perchè se presente un sigolo blocco occupato a fine corsa non lo vede*/
  bool_t found = FALSE;
  bool_t contatto_minore = FALSE; /*Questa variabile mi serve per il caso in cui il tetramino si appoggi ma il prossimo blocco invece no*/
  /*contatto è uguale all'ultimo numero della colonna, quindi se colonna 2 allora sara 142 il numero contatto*/
  for (c = scelta_colonna; c < contatto; ){ /*ciao*/
    for(i = 0; i < size ; i++){
      if(piano[c + i] == OCCUPATO && *p > 0 && contatto_minore == FALSE){
        printf("trovato contatto a : %d\n", c + i);  /*printf inutile, lo tengo per eventuali errori nel codice*/
        if(*p == 4 || *p == 6){ /*Verifico se sotto il tetramino c'è qualcosa su cui appoggiare*/
          contatto = c;
		      found = TRUE;
          }
          else if(*p == 5 && c >= 140){  /*Nel caso di J_90 che arriva fino in fondo*/
			      contatto = c;
			      found = TRUE;
			      }
      	    else if (*p == 5 && found == FALSE ){
		          contatto = c + COLONNE;
              found = TRUE;
              }
              else if(*inizio == 7){
			          contatto = c - COLONNE;
			          found = TRUE;
			          }
        	      else if (*p == 1 || *p == 2 || *p == 3){
                  contatto = c - COLONNE;
				          contatto_minore = TRUE;
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
  contatto = verifica_uscita(piano,p,riga,scelta_colonna, contatto);
  p = inizio; /*ritorno all'inizio del mio tetramino*/
  if(found == FALSE)
    return contatto - size; /*tolgo size perchè ora non mi serve più per verificare se cella occupata*/
	else
	  return contatto; /*se ho trovato un contatto uso il valore calcolato*/
}

/**
 * @brief Modifica il campo di gioco in base a dove è caduto il tetramino a cominciare dalla posizione passata dalla funzione @see contatto
 * @param piano punta al campo di gioco del giocatore 1 o 2 in base al turno
 * @param p punta al primo elemento dell'array del tetramino che ha selezionato il giocatore
 * @param scelta_colonna 
 * @return bool_t FALSE se il giocatore è uscito da righe o colonne, TRUE se è andato tutto bene
 */
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
  if(scelta == -1){
    printf("!!! ATTENZIONE !!!:\tHai perso la partita non hai posizionato correttamente un pezzo nel tuo campo di gioco.\n\n");
    return FALSE;
    }
    if(scelta < 0){
      printf("!!! ATTENZIONE !!!:\tHai perso la partita non hai posizionato correttamente un pezzo nel tuo campo di gioco.\n\n");
      return FALSE;
      }

  occupata = verifica_occupata(piano, scelta, p);

  if(occupata == FALSE){
    is_ok == TRUE;
    for(i = 0; i<size; i++){
      /*******************************************************
      *  scelta rappresenta la selezione attuale nel piano   *
      *  riquadro_1 si trova sopra riquadro nel piano        *
      *  riquadro_2 si trova sopra riquadro_1 nel piano      *
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

/**
 * @brief Funzione chiamata dalla funzione scelta per scegliere la rotazione del tetramino
 * @param code input da tastiera scelto dal main e passato dalla funzione scelta, tetramino scelta tra quelli disponibili
 * @param colonna_scelta_dal_giocatore input da tastiera, colonna scielta dal giocatore
 * @return int* passa un puntatore che punta al primo elemento dell'array del tetramino che ha selezionato il giocatore
 */
int * rotazione(char code, int colonna_scelta_dal_giocatore){
  int *p;
  int chose = 0;
  bool_t is_ok = FALSE;

  /*Funzione molto luga ma solo solo una serie di if che fanno scegliere al gicatore la rotazione e assegna a p il primo elemento di uno degli array*/
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
		case 2: p = I_90;
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

/** @brief Funzione che restituisce quanti tetramini sono rimasti disponibili*/
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

/**
 * @brief funzione chiamata dal main per scegliere quale tetramino usare tra i seguenti i, j, o, s, l, t o z
 * 
 * @param colonna_scelta_dal_giocatore questo parametro viene passato solo perchè dopo voglio stampare l'anteprima del tetramino scelto @see stampa_anteprima
 * @return int* passa un puntatore che punta al primo elemento dell'array del tetramino che ha selezionato il giocatore, in base alla funzione @see rotazione
 */
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
	
/**
 * @brief Funzione chiamata dal main per avviare tutte le funzioni relative alla selezione del tetramino,
 * alla sua rotazione e al suo posizionamento.
 * 
 * @param piano punta al campo di gioco del giocatore 1 o 2 in base al turno
 * @param RIGHE il numero di righe del campo da gioco
 * @param COLONNE il numero di colonne del campo da gioco
 * @param turno indica chi sta giocando se il giocatore 1 o 2
 * @return è presente un return in caso di errore. in quel caso la variabile globale sarà TRUE @see Perdita_uscita_campo
 */
void seleziona_tetramino(campo_di_gioco piano, int RIGHE, int COLONNE, int turno){
  bool_t is_ok = FALSE;
  int scelta_colonna = -1;
	int tetramino[size] = {1,1,1,1};

	/*inserisco un ciclo while per selezionare la colonna e il tetramino corretto e utilizzabile*/
	while (is_ok == FALSE){

	  printf("Seleziona la colonna dove inserire il tetramini:\n");
	  printf("Scelta: ");
    scanf(" %d", &scelta_colonna);
		while (getchar() != '\n') /*salta alla fine della riga*/
		  ;
    printf("\n");

	  if( turno == 1 && scelta_colonna < 10 && scelta_colonna >= 0)
	    is_ok = TRUE;   /*se il turno è del giocatore uno deve selezionare una colonna da 0 a 9*/
	    else if( turno == 2 && scelta_colonna >= 10 && scelta_colonna < 20 )
	      is_ok = TRUE;  /*se il turno è del giocatore due deve selezionare una colonna da 10 a 19*/
	      else{
	      	printf("!!! ATTENZIONE !!!:\tScelta sbagliata. Hai selezionato una colonna sbagliata.\n");
				  }
	        
    /*Se seleziono colonna corretta allora avanzo*/
	  if ( is_ok == TRUE ){
	    int *p;
      int i;
      /*scelgo a quale tetramino punterà il mio puntatore*/ 
	    p = scelta(scelta_colonna);
        
		/*salva tetramino oltre a modificare il campo di gioco in base alle mie scelte restituisce anche errori evenutali is_ok in questo caso sarà false*/
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

/**
 * @brief viene chiamata quando una riga è completamente, in quel caso la elimina
 * 
 * @param piano punta al campo di gioco del giocatore 1 o 2 in base al turno
 * @param riga_punto la riga che è completamente occupata questo lo estraggo dalla funzione @see calcola_punti ricorsivamente controlla tutto il campo da gioco e quale riga siamo arrivati
 * @param colonna la colonna a cui siamo arrivati, non la utilizzo ma è stata inserita per sviluppi futuri
 */
void rimuovi_riga(campo_di_gioco piano, int riga_punto, int colonna){
  int r= 1, c = 0, i;
	/*vado a rimuovere la riga completata*/
  for (c=0; c<COLONNE; c++) {
    riquadro_t riquadro = VUOTO;
    piano[riga_punto*COLONNE + c] = riquadro;
    }
  
  /*considero colonna per colonna e sposto l'array indietro di una posizione*/
	for (c=0; c<COLONNE; c++) {
    for (r=riga_punto-1; r>-2; r--) {
      riquadro_t riquadro_sotto = piano[r*COLONNE + c]; /*Questo passaggio mi permette di memorizzare il valore della riquadro sopra*/
		  if(r == - 1) /*Quando l'ultima riga presenta dei riquadri occupati, per spostarli nella riga sottostanze e azzerare l'ultima metto questa condizione*/
		    riquadro_sotto = VUOTO;
      piano[(r+1)*COLONNE + c] = riquadro_sotto;          
		  }
	  }
}

/**
 * @brief controlla tutto il campo da gioco finche non trova una riga completa da eliminare e quindi generare punteggio
 * @note assegga un bonus in caso di righe completate contemporaneamente e assegna quindi più punti
 * @param piano punta al campo di gioco del giocatore 1 o 2 in base al turno
 * @param RIGHE numero di righe del campo da gioco
 * @param COLONNE numero di colonne del campo da gioco
 * @return int Ritorna il numero di punti, 1 se il valore di bonus è 1 quindi ho eliminato una colonna, 3 se bonus = 2 , 6 se ne elimino 3 insieme e 4 in tutti gli altri casi
 */
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

/**
 * @brief stampa il campo di gioco , assegno alla variabile riquadro il valore del piano nella posizione r e c.
 * 
 * @param piano punta al campo di gioco del giocatore 1
 * @param piano2 punta al campo di gioco del giocatore 2 
 * @param RIGHE numero di righe del campo da gioco, i campi da gioco sono gradi uguali
 * @param COLONNE numero di righe del campo da gioco, i campi da gioco sono gradi uguali
 * @param giocatori passo il numero di giocatori single player o multi player, perchè stampo o 1 o 2 campi da gioco.
 */
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
    else{
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

/** @brief Usata nel Multiplayer In questa modalità si avranno il doppio di pezzi a disposizione.*/
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

/**
 * @brief Nel caso il giocatore cancelli 3 o più linee con una singola mossa, la variabile inverti_campo diventa TRUE
 * e il campo dell’avversario viene modificato invertendo il corrispondente numero di linee nella parte più bassa del campo di gioco: una posizione vuota diventa piena e viceversa.  
 * @param campo_giocatore punta al campo di gioco del giocatore 1 o 2 in base al turno
 * @param RIGHE numero di righe del campo di gioco
 * @param COLONNE numero di colonne del campo di gioco
 * @param righe_da_invertire numero di righe da invertire nella parte più bassa del campo di gioco
 * @note il massimo numero di righe cancellabili è 4 con un tetramino I verticale
 */
void inverti_campo_di_gioco(campo_di_gioco campo_giocatore, int RIGHE, int COLONNE, int righe_da_invertire){
  int r,c;
	for (r=RIGHE - righe_da_invertire; r<RIGHE; r++) {
    for (c=0; c<COLONNE; c++) {
      riquadro_t riquadro = campo_giocatore[r*COLONNE + c];
			if (riquadro==VUOTO)
        campo_giocatore[r*COLONNE + c] = OCCUPATO;
        else if (riquadro==OCCUPATO)
          campo_giocatore[r*COLONNE + c] = VUOTO;
		  }
	  }
}

/**
 * @brief funzione che seleziona un tetramino casuale in base alla colonna scelta.
 * @note un tetramino I_ orrizontale non può starci nella colonna 8 perchè và fuori dal campo.
 * @param colonna_random colonna passata dalla funzione, da 0 a 7
 * @return int* ritorna un puntatore ad un tetramino casuale 
 */
int * tetramino_random(int colonna_random){
  int tetramino_random = rand() % 7; /*genera un numero compreso tra 0 e 6*/
	int rotazione_random = 0;
	
	/*logica scelta del tetramino: I = 0, J = 1, L = 2, O = 3, S = 4, T = 5, Z = 6*/
	if(tetramino_random == 0 || colonna_random == 9){
	  I_free--; /*tolgo un tetramino disponibile*/
	  rotazione_random = rand() % 1; /*genera un numero compreso tra 0 a 1*/
	  if(rotazione_random == 0 && colonna_random < 7)
	    return I_;
		  else
		    return I_90;
	  }
	  else if(tetramino_random == 1 && colonna_random < 8){
		  J_free--;
	    rotazione_random = rand() % 4; /*numero casuale tra 0 e 3*/
		  if(rotazione_random == 0)
		    return J_;
		    else if(rotazione_random == 1 && colonna_random < 9)
		      return J_90;
			    else if(rotazione_random == 2 && colonna_random < 8)
			      return J_180;
			      else
			        return J_270;
		}
	    else{
		    Z_free--;
		    rotazione_random = rand() % 2;
		    if(rotazione_random == 0)
		      return Z_;
			    else
			    return Z_90;
		  }
}

/**
 * @brief Non utilizzata
 * @param campo_cpu 
 * @param colonna 
 * @param riga 
 * @return bool_t 
 */
bool_t controllo_verticale(campo_di_gioco campo_cpu, int colonna, int riga){
	int r,c,i, spazio_libero;
	int is_empty = 0;
	for (c=colonna; c<colonna+1; c++) {
    for (r=riga; r<riga +1; r++) {
      for(i = 0; i< size; i++){
		    spazio_libero = r*COLONNE + c;
        riquadro_t riquadro = campo_cpu[spazio_libero];
		    if(riquadro == VUOTO && campo_cpu[spazio_libero + 1] == OCCUPATO)
		      is_empty++;
		    if(is_empty == 4){
          salva_tetramino(campo_cpu,I_,c);
		    	return TRUE;
		      }
		    }
	    }
	  }
	return FALSE;
}

/** @brief Sequenza di mosse predefinita della cpu. */
int * mosse_tetramino[5] = {I_,O_,O_,I_,O_};
/** @brief Sequenza di scelta colonna predefinita della cpu. */
int mosse_colonna [5] = {0,4,6,0,8};
/** @brief indica a quale mossa predefinita è arrivata la CPU */
int mossa_numero = 0;

/**
 * @brief Funzione casuale che restituisce 0 o 1 con uguale probabilità
 * @return bool_t 0 o 1 al 50% 
 * @note La funzione rand() genererà un numero dispari o pari con uguale probabilità.
 */
bool_t rand50(){
  return rand() & 1;
}

/**
 * @brief Funzione casuale che restituisce 1 con 87,5% di probabilità e 0 con 12% di probabilità utilizzando
 * @return bool_t restituisce 0 12 volte su 100 e il restante 1
 * @note la funzione rand50 genera 1 o 0 con uguale probabilità quindi 0.5*0.5*0.5 = 0.125
 */
bool_t rand87(){
  return rand50() | rand50() || rand50();
}

/**
 * @brief Funzione usata dalla cpu per inserire il tetramino, molte volte inserisce i tetramini secondo questo ordine @see mosse_tetramino[5] in determinate colonne @see mosse_colonna [5] .
 * Alcune volte (il 12 per cento delle volte) inserisce i tetramini casualmente.
 * 
 * @param campo_cpu punta al campo di gioco della CPU.
 * @param RIGHE numero di righe del campo di gioco
 * @param COLONNE numero di colonne del campo di gioco
 * @attention la strategia è molto semplice, consiste in una serie di mosse (5) determinate, l'unica differenza e che a volte viene inserito casualmente un tetramino diverso.
 */
void Cpu_colonna(campo_di_gioco campo_cpu, int RIGHE, int COLONNE){
  int r, c, spazio_libero;
  bool_t trovato = FALSE;
  int * tetramino = 0;
  int colonna_random;
  
  if(rand87() == 1 ){ /*uguale a 1 l' 87.5% delle volte*/
    salva_tetramino(campo_cpu,mosse_tetramino[mossa_numero],mosse_colonna[mossa_numero]);
    /*Diminuisco i tetramini disponibili*/
    if(mossa_numero == 0 || mossa_numero == 3)
      I_free--;
      else if(mossa_numero == 1 || mossa_numero == 2 || mossa_numero == 4)
        O_free--;
    mossa_numero++;
    if(mossa_numero == 5)
      mossa_numero = 0;
	  }
	  else{
	    colonna_random = rand() % 10; /*genera un numero compreso tra 0 e 10*/
	  /** @note Alcune persone non piace questa formula perché utilizza i bit di ordine inferiore del numero dato da rand(),
       e nelle implementazioni più vecchie di numeri pseudo-casuali software questi erano spesso meno casuali
        rispetto ai bit di ordine elevato, ma su qualsiasi sistema moderno questo metodo dovrebbe andare bene.*/
	    tetramino = tetramino_random(colonna_random);
	    salva_tetramino(campo_cpu,tetramino,colonna_random);
	    }
}

/**
 * @brief funzione che simula un caricamento e chiama funzione di inserimento tetramino dalla cpu
 * 
 * @param campo_cpu punta al campo di gioco della CPU.
 * @param RIGHE numero di righe del campo di gioco
 * @param COLONNE numero di colonne del campo di gioco
 * @note il tempo di caricamento dipende dalla macchina in cui si esegue il programma.
 */
void seleziona_CPU(campo_di_gioco campo_cpu, int RIGHE, int COLONNE)
{
  bool_t is_ok = FALSE;
	int scelta_colonna = -1;
	int * tetramino;

	/*inserisco un ciclo while per selezionare la colonna e il tetramino corretto e utilizzabile*/
	while (is_ok == FALSE){
    /*Schermata di caricamento molto semplice*/
	  printf("Sto pensando");
	  int i,j,a;
    for (i = 0; i < 10; i++){
		  for(j=0; j < 100000000; j++)
		    a = j;
        printf(".");
      }
    printf("\n");
    Cpu_colonna(campo_cpu, RIGHE, COLONNE);

    return;
    }
}

/**
* @brief Funzione usata a scopo di test, inserisce 3 righe piene fino alla colonna 7.
* @note usata per testare l'inversione del campo avversario
* @param campo_giocatore campo giocatore 1
* @param RIGHE numero righe campo giocatore 1, portate a -3 per riempire 3 righe
* @param COLONNE numero colonne giocatore 1 portate a -2 per riempore 8 colonne
* */
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

/**
 * @brief Riceve i punteggi di uno o due giocatori e verifica se uno dei due ha vinto. 
 * controlla anche i tetramini disponibili perchè è una condizione di vittoria.
 * @param punteggio_1 il punteggio del giocatore 1
 * @param punteggio_2 il punteggio del giocatore 2, in caso di giocatore singolo questo sarà -1
 * @param turno il turno indica quale giocatore sta giocando al momento 1 = giocatore 1 2 = giocatore 2
 * @return bool_t ritorna il valore TRUE se uno dei giocatori vince, FALSE se non c'è una vittoria
 */
bool_t Controlla_vittoria(int punteggio_1, int punteggio_2, int turno){

  if(punteggio_1 >= Punteggio_massimo){
    if(punteggio_2 == -1) /*Nel caso di giocatore singolo*/
      printf("Complimenti hai raggiunto %d punti, arrivederci!\n", Punteggio_massimo);
      else
        printf("Complimenti Giocatore 1 hai raggiunto %d punti, HAI VINTO! arrivederci!\n", Punteggio_massimo);
	  return TRUE;
    }
	  else if(punteggio_2 >= Punteggio_massimo){
		  printf("Complimenti Giocatore 2 hai raggiunto %d punti, HAI VINTO! arrivederci!\n", Punteggio_massimo);
		  return TRUE;
		  }
	
  /*Perdita per uscita dal campo in gicatore singolo*/
  if(Perdita_uscita_campo != FALSE && punteggio_2 == -1){
    printf("%40s%2s%40s\n"," ","**"," ");
    printf("HAI PERSO! Sei uscito dal campo, arrivederci!\n");
	  printf("%40s%2s%40s\n"," ","**"," ");
	  return TRUE;
  }

	if(Perdita_uscita_campo != FALSE && turno == 2){
	  printf("%40s%2s%40s\n"," ","**"," ");
    printf("Complimenti Giocatore 2 il Giocatore 1 è uscito dal campo, HAI VINTO! arrivederci!\n");
	  printf("%40s%2s%40s\n"," ","**"," ");
	  return TRUE;
    }
	if(Perdita_uscita_campo != FALSE && turno == 1){
	  printf("%40s%2s%40s\n"," ","**"," ");
    printf("Complimenti Giocatore 1 il Giocatore 2 è uscito dal campo, HAI VINTO! arrivederci!\n");
	  printf("%40s%2s%40s\n"," ","**"," ");
	  return TRUE;
    }
	/*Perdita per fine dei tetramini*/
	if(I_free <= 0 && J_free <= 0 && L_free <= 0 && O_free <= 0 
	&& S_free <= 0 && T_free <= 0 && Z_free <= 0){
    /*In caso di giocatore singolo*/
    if(punteggio_2 == -1){
      printf("I tetramini sono finiti, HAI PERSO! Non hai raggiunto il punteggio per vincere.\n");
      return TRUE;
      }
	  printf("I tetramini sono finiti!, vince il giocatore con il punteggio più alto : ");
	  if(punteggio_1 > punteggio_2)
	    printf("GIOCATORE 1, Complimenti\n");
	    else if(punteggio_2 < punteggio_1)
		    printf("GIOCATORE 2, Complimenti\n");
		    else
		      printf("PAREGGIO, Complimenti a tutti e due\n");
	  return TRUE;
	  }

  return FALSE;
}

/**
 * @brief funzione che uso a scopo di test, stampa il campo da gioco e il numero di ogni casella
 * OUTPUT:\n
 ---TEST----\n
\n
   0   1   2   3   4   5   6   7   8   9\n
  10  11  12  13  14  15  16  17  18  19\n
  20  21  22  23  24  25  26  27  28  29\n
  30  31  32  33  34  35  36  37  38  39\n
  40  41  42  43  44  45  46  47  48  49\n
  50  51  52  53  54  55  56  57  58  59\n
  60  61  62  63  64  65  66  67  68  69\n
  70  71  72  73  74  75  76  77  78  79\n
  80  81  82  83  84  85  86  87  88  89\n
  90  91  92  93  94  95  96  97  98  99\n
 100 101 102 103 104 105 106 107 108 109\n
 110 111 112 113 114 115 116 117 118 119\n
 120 121 122 123 124 125 126 127 128 129\n
 130 131 132 133 134 135 136 137 138 139\n
 140 141 142 143 144 145 146 147 148 149\n
 * @param piano punta al campo di gioco del giocatore 1 o 2 in base al turno
 * @param RIGHE numero di righe del campo di gioco
 * @param COLONNE numero di colonne del campo di gioco
 * @return void 
 */
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

/*************************************************//**
* main:	chiede all'utente di immettere un codice, poi           
*	chiama una funzione per eseguire l'azione                   
*   richiesta. Continua fino a quando la variabile              
*   fine_gioco non diventa positiva. stampa messaggio           
*	di errore quando il codice e' sbagliato.                    
*****************************************************/

int main(){

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
* @param bonus Usato per calcolare il numero di righe da invertire
* @param vittoria indica che uno dei due giocatori ha vinto
*/
  int   punteggio_1 = 0;
  int   punteggio_2 = 0;
  int bonus = 0;
  bool_t vittoria = FALSE;

/** creo i campi da gioco del giocatore 1 e 2, predispongo memoria per tutto il campo 10*15 = 149 caselle 
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
  /*test(campo_giocatore_1,RIGHE,COLONNE);*/ /*Funzione usata a scopo di test durate la programmazione*/
do
{
  if(giocatori == NOT_SELECTED){
    printf("Ciao :), In quanti giocate?\n\n");
    printf("1. Single Player - Un solo giocatore\n");
    printf("2. Multiplayer - due giocatori\n");
	  printf("3. CPU vs Player\n");
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
              /*TEST_INVERTI(campo_giocatore_2, RIGHE, COLONNE);*/ /*Inserire solo per scopo di test*/
              raddoppia_tetramini(); /*Raddopia il numero di tetramini per il multiplayer*/
              break;
		  case 3: printf("Ti piace la sfida!\n");
              raddoppia_tetramini(); /*Raddopia il numero di tetramini per il multiplayer*/
              break;
      default: printf("!!! ATTENZIONE !!!:\tScelta sbagliata. Selezionane un altra.\n");
               giocatori = NOT_SELECTED;
               break; 
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
      case 2: Visualizza_pezzi_disponibili();
              break;
      case 3: printf("Arrivederci\n");
	            fine_gioco = TRUE; 
              break;
      default: printf("!!! ATTENZIONE !!!:\tScelta sbagliata. Selezionane un altra.\n"); /*Cambio turno in modo da continuare il ciclo sul giocatore attuale*/   
      }
    
    /**Controllo Vittoria dopo i 50 punti e Perdita per uscita dal campo In giocatore singolo punteggio_2 = -1*/
	  vittoria = Controlla_vittoria(punteggio_1,-1,turno);
    
	  if(vittoria != FALSE)
	    fine_gioco = TRUE;
  }

  if(giocatori == MULTI_PLAYER){ 
    if(turno == 1){
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
	
    switch (select){
      case 1: if (turno == 1){
                seleziona_tetramino(campo_giocatore_1, RIGHE, COLONNE, turno);
                bonus = calcola_punti(campo_giocatore_1,RIGHE, COLONNE);
                punteggio_1 = punteggio_1 + bonus;
                turno = 2;
                }
                else if(turno == 2){
                  seleziona_tetramino(campo_giocatore_2, RIGHE, COLONNE, turno);
					        bonus = calcola_punti(campo_giocatore_2,RIGHE, COLONNE);
                  punteggio_2 = punteggio_2 + bonus;
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
/**Controllo Vittoria dopo i 50 punti e Perdita per uscita dal campo*/
	vittoria = Controlla_vittoria(punteggio_1,punteggio_2,turno);

/*la variabile inverti_campo diventa TRUE nella funzione calcola punti e quindi chiamo funzione inverti campo da gioco*/
	if(inverti_campo != FALSE && turno == 1 && vittoria != TRUE){
	  inverti_campo = FALSE;
		printf("%40s%2s%40s\n"," ","**"," ");
    printf("Il Giocatore 2 ha cancellato 3 o più righe, Campo giocatore 1 sarà invertito!\n");
		printf("%40s%2s%40s\n"," ","**"," ");
	  if(bonus == 12)  /*vuol dire che ho cacellato 4 righe*/
		  inverti_campo_di_gioco(campo_giocatore_1, RIGHE, COLONNE, 4);
		  else /*vuol dire che ho cancellato 3 righe*/
		    inverti_campo_di_gioco(campo_giocatore_1, RIGHE, COLONNE, 3);
    }
	if(inverti_campo != FALSE && turno == 2 && vittoria != TRUE){
	  inverti_campo = FALSE;
		printf("%40s%2s%40s\n"," ","**"," ");
    printf("Il Giocatore 1 ha cancellato 3 o più righe, Campo giocatore 2 sarà invertito!\n");
		printf("%40s%2s%40s\n"," ","**"," ");
		if(bonus == 12)  /*vuol dire che ho cacellato 4 righe*/
		  inverti_campo_di_gioco(campo_giocatore_2, RIGHE, COLONNE, 4);
		  else /*vuol dire che ho cancellato 3 righe*/
		    inverti_campo_di_gioco(campo_giocatore_2, RIGHE, COLONNE, 3);
    }

	if(vittoria != FALSE)
	  fine_gioco = TRUE;  
}  

if(giocatori == CPU_PLAYER)   
{ 
  	if(turno == 1)
	{
	  stampa(campo_giocatore_1,campo_giocatore_2, RIGHE, COLONNE, CPU_PLAYER);
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
      }
	  else{
		stampa(campo_giocatore_1,campo_giocatore_2, RIGHE, COLONNE, CPU_PLAYER);
      	printf("Turno del giocatore 2.\nPunteggio: %d.\n\n", punteggio_2);
		select = 1;
	    }

      switch (select)
      {
        case 1: if (turno == 1){
                  seleziona_tetramino(campo_giocatore_1, RIGHE, COLONNE, turno);
				  bonus = calcola_punti(campo_giocatore_1,RIGHE, COLONNE);
                  punteggio_1 = punteggio_1 + bonus;
                  turno = 2;
                  }
                  else if(turno == 2){
                    seleziona_CPU(campo_giocatore_2, RIGHE, COLONNE);
					bonus = calcola_punti(campo_giocatore_2,RIGHE, COLONNE);
                    punteggio_2 = punteggio_2 + bonus;
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
/**Controllo Vittoria dopo i 50 punti e Perdita per uscita dal campo*/
	     vittoria = Controlla_vittoria(punteggio_1,punteggio_2,turno);

/*la variabile inverti_campo diventa TRUE nella funzione calcola punti e quindi chiamo funzione inverti campo da gioco*/
	  if(inverti_campo != FALSE && turno == 1 && vittoria != TRUE){
	     inverti_campo = FALSE;
		 printf("%40s%2s%40s\n"," ","**"," ");
         printf("Il Giocatore 2 ha cancellato 3 o più righe, Campo giocatore 2 sarà invertito!\n");
		 printf("%40s%2s%40s\n"," ","**"," ");
		 if(bonus == 12)  /*vuol dire che ho cacellato 4 righe*/
		   inverti_campo_di_gioco(campo_giocatore_1, RIGHE, COLONNE, 4);
		   else /*vuol dire che ho cancellato 3 righe*/
		     inverti_campo_di_gioco(campo_giocatore_1, RIGHE, COLONNE, 3);
         }
	  if(inverti_campo != FALSE && turno == 2 && vittoria != TRUE){
	     inverti_campo = FALSE;
		 printf("%40s%2s%40s\n"," ","**"," ");
         printf("Il Giocatore 1 ha cancellato 3 o più righe, Campo giocatore 2 sarà invertito!\n");
		 printf("%40s%2s%40s\n"," ","**"," ");
		 if(bonus == 12)  /*vuol dire che ho cacellato 4 righe*/
		   inverti_campo_di_gioco(campo_giocatore_2, RIGHE, COLONNE, 4);
		   else /*vuol dire che ho cancellato 3 righe*/
		     inverti_campo_di_gioco(campo_giocatore_2, RIGHE, COLONNE, 3);
         }


	  if(vittoria != FALSE)
	     fine_gioco = TRUE;
      
}  

} while (!fine_gioco);

while (getchar() != '\n') /*premi invio per uscire dal gioco*/
  ;
  return 0;
}
