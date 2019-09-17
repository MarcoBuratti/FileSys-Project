#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define H 256
#define L 1024
#define STR 70000
#define DIM 2048



typedef struct _nodo{
    int larghezza;
    int file;           //** in File di 0 dimensione dinamica del vettore
    bool sporco;
    char *nome;
    char *cont;
    char *nperc;
    int ultimo;
    int indirizzo;
    struct _nodo *figli;
    struct _nodo *fratellodx;
    struct _nodo *fratellosx;

}nodo;

typedef struct nodo2{
	char *parola;
	struct nodo2 *next;
} elem_t;


int hashfunc(char *, unsigned int);
void incoda(nodo *);
nodo * inserisci(nodo *, int, char *, char *, nodo *);
nodo * inseriscif(nodo *, int, char *, char *, nodo *);
nodo * rehash(nodo*, nodo *);
nodo * fratelli(nodo *, int);
void taglia (nodo *, int );
void reset(nodo *, int );
void tagliaric(nodo *);
elem_t *in_coda(elem_t *, char *);
elem_t * search(char *, nodo *, elem_t *);

void create_dir(char *, nodo *);
void create(char *, nodo *);
void write(char *, char *, nodo *);
void read(char *, nodo *);
void delet(char *, nodo *);
void delet_r(char *, nodo *);
void find(char *, nodo *);


int main (){


    char input[STR], *token, temp[STR];
    char comando[H], percorso[STR], contenuto[H];
    struct _nodo *testa=NULL;

    testa = (nodo*)calloc(2048, sizeof(nodo));
    (testa+0)->larghezza = 0;
    (testa+0)->indirizzo = 0;
    (testa+0)->file = 2048;

    do{
    //fgets(input, STR, stdin);
    gets(input);
    strcpy(temp, input);
    token=strtok(temp, "\n");
    token=strtok(temp, " ");
    strcpy(comando, token);

    if(strcmp(comando,"create")==0){
        token=strtok(NULL, " ");
        strcpy(percorso, token);
        create(percorso, testa);
    }
    else if(strcmp(comando,"create_dir")==0){
        token=strtok(NULL, " ");
        strcpy(percorso, token);
        create_dir(percorso, testa);
    }
    else if(strcmp(comando,"write")==0){
        token = strtok(NULL, " ");
        strcpy(percorso, token);
        token = strtok(NULL, "\"");
        strcpy(contenuto, token);
        write(percorso, contenuto, testa);
    }
    else if(strcmp(comando,"read")==0){
        token=strtok(NULL, " ");
        strcpy(percorso, token);
        read(percorso, testa);
    }
    else if(strcmp(comando,"find")==0){
        token=strtok(NULL, " ");
        strcpy(percorso, token);
        find(percorso, testa);
    }
    else if(strcmp(comando,"delete")==0){
        token=strtok(NULL, " ");
        strcpy(percorso, token);
        delet(percorso, testa);
    }
    else if(strcmp(comando,"delete_r")==0){
        token=strtok(NULL, " ");
        strcpy(percorso, token);
        delet_r(percorso, testa);
    }
    else{
        break;
    }
    } while (1);                  //** necessario solo al do while non esegue alcun tipo di controllo

    return 0;
}

void create_dir(char *percorso, nodo *testa){

    int  i, pos, lenght, slash, save;
    char *token, *perc;
    int ctrl;
    struct _nodo *padre;
    char *name=NULL;

    slash = 0;
    lenght = strlen(percorso);
    for(i=0; i<lenght; i++){
        if (percorso[i] == '/')
            slash++;
    }
    if (slash > 255){
        printf("no\n");
        return;
    }
    perc = (char*)malloc(lenght+1);
    strcpy(perc, percorso);
    token = strtok(percorso, "/");
    while(token !=NULL){
        name = (char*)calloc((2*strlen(token)+1),sizeof(char));
        if(slash == 1){
            pos = hashfunc(token, (testa+0)->file);
            save = pos;
            while( (testa+pos)->sporco == 1 ){
                    if ( (testa+pos)->nome != NULL)
                        strcpy (name, (testa+pos)->nome);
                    else
                        strcpy (name, "\0");

                if( (strcmp( name, token) == 0) || (testa+0)->larghezza >= 1024 ){
                    printf("no\n");
                    return;
                }if( strlen( name) == 0 ) {  save = pos ;}
                pos ++;
                if(pos == (testa+0)->file) pos = 1;
            }
            save = pos;
            testa = inserisci(testa, save, perc, token, padre);
            printf("ok\n");
            return;
        }
        if(slash > 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while(ctrl == 0 && (testa+pos)->sporco == 1){
                    if ( (testa+pos)->nome != NULL )
                        strcpy (name, (testa+pos)->nome);
                    else
                        strcpy (name, "\0");
                if( strcmp( name, token ) == 0){
                    if( (testa+pos)->file == 1 ){
                        printf("no\n");
                        return;
                    }else{
                        padre = (testa+pos);
                        testa = (testa+pos)->figli;
                        slash--;
                        ctrl = 1;
                    }
                }else{
                    pos ++;
                    if(pos == (testa+0)->file) pos = 1;
                    }
            }
        }if(ctrl == 0){
            printf("no\n");
            return;
        }
        free(name);
        name=NULL;
        token = strtok(NULL, "/");
        }
}

void create(char *percorso, nodo *testa){

    int  i, pos, lenght, slash, save;
    char *token, *perc;
    int ctrl;
    struct _nodo *padre;
    char *name=NULL;

    slash = 0;
    lenght = strlen(percorso);
    for(i=0; i<lenght; i++){
        if (percorso[i] == '/')
            slash++;
    }
    if (slash > 255){
        printf("no\n");
        return;
    }
    perc = (char*)malloc(lenght+1);
    strcpy(perc, percorso);
    token = strtok(percorso, "/");
    while(token !=NULL){
        name = (char*)calloc((2*strlen(token)+1),sizeof(char));
        if(slash == 1){
            pos = hashfunc(token, (testa+0)->file);
            save = pos;
            while( (testa+pos)->sporco == 1 ){
                    if ( (testa+pos)->nome != NULL)
                        strcpy (name, (testa+pos)->nome);
                    else
                        strcpy (name, "\0");

                if( (strcmp( name, token) == 0) || (testa+0)->larghezza >= 1024 ){
                    printf("no\n");
                    return;
                }if( strlen( name) == 0 ) {  save = pos ;}
                pos ++;
                if(pos == (testa+0)->file) pos = 1;
            }
            save = pos;
            testa = inseriscif(testa, save, perc, token, padre);
            printf("ok\n");
            return;
        }
        if(slash > 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while(ctrl == 0 && (testa+pos)->sporco == 1){
                    if ( (testa+pos)->nome != NULL )
                        strcpy (name, (testa+pos)->nome);
                    else
                        strcpy (name, "\0");
                if( strcmp( name, token ) == 0){
                    if( (testa+pos)->file == 1 ){
                        printf("no\n");
                        return;
                    }else{
                        padre = (testa+pos);
                        testa = (testa+pos)->figli;
                        slash--;
                        ctrl = 1;
                    }
                }else{
                    pos ++;
                    if(pos == (testa+0)->file) pos = 1;
                    }
            }
        }if(ctrl == 0){
            printf("no\n");
            return;
        }
        free(name);
        name=NULL;
        token = strtok(NULL, "/");
        }
}

void write(char *percorso, char * contenuto, nodo *testa){

    int  i, pos, lenght, save, slash, newl;
    char *token, *perc;
    int ctrl;
    char *name=NULL;

    slash = 0;
    lenght = strlen(percorso);
    for(i=0; i<lenght; i++){
        if (percorso[i] == '/')
            slash++;
    }

    token = strtok(percorso, "/");
    while(token !=NULL){
        name = (char*)calloc((2*strlen(token)+1),sizeof(char));
        if(slash == 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while( (testa+pos)->sporco == 1 ){
                if ( (testa+pos)->nome != NULL)
                    strcpy ( name, (testa+pos)->nome );
                else
                    strcpy ( name, "\0" );

                if( (strcmp( name, token) == 0) && (testa+pos)->file == 1){
                        newl = strlen(contenuto);
                        (testa+pos)->cont = (char*)malloc(newl+1);
                        strcpy( (testa+pos)->cont, contenuto);
                        slash = 0;
                        ctrl = 1;
                        printf("ok %d\n", newl);
                        return;
                }
                pos ++;
                if(pos == (testa+0)->file) pos = 1;
            }if(ctrl == 0){
            printf("no\n");
            return;
            }
        }
        if(slash > 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while(ctrl == 0 && (testa+pos)->sporco == 1){
                    if ( (testa+pos)->nome != NULL )
                        strcpy (name, (testa+pos)->nome);
                    else
                        strcpy (name, "\0");
                if( strcmp( name, token ) == 0){
                    if( (testa+pos)->file == 1 ){
                        printf("no\n");
                        return;
                    }else{
                        testa = (testa+pos)->figli;
                        slash--;
                        ctrl = 1;
                    }
                }else{
                    pos ++;
                    if(pos == (testa+0)->file) pos = 1;
                    }
            }
        }if(ctrl == 0){
            printf("no\n");
            return;
        }
        free(name);
        name=NULL;
        token = strtok(NULL, "/");
        }
}

void read(char *percorso, nodo *testa){

    int  i, pos, lenght, save, slash, newl;
    char *token, *perc;
    int ctrl;
    char *name=NULL;

    slash = 0;
    lenght = strlen(percorso);
    for(i=0; i<lenght; i++){
        if (percorso[i] == '/')
            slash++;
    }

    token = strtok(percorso, "/");
    while(token !=NULL){
        name = (char*)calloc((2*strlen(token)+1),sizeof(char));
        if(slash == 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while( (testa+pos)->sporco == 1 ){
                if ( (testa+pos)->nome != NULL)
                    strcpy ( name, (testa+pos)->nome );
                else
                    strcpy ( name, "\0" );
                if( (strcmp( name, token) == 0) ){
                    if( (testa+pos)->file == 1 ){
                        if((testa+pos)->cont == NULL){
                                printf("contenuto\n");
                                return;
                            }
                            slash = 0;
                            ctrl = 1;
                            printf("contenuto %s\n", (testa+pos)->cont);
                            return;
                        }
                }
                pos ++;
                if(pos == (testa+0)->file) pos = 1;
            }
            if(ctrl == 0){
            printf("no\n");
            return;
        }
        }

        if(slash > 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while(ctrl == 0 && (testa+pos)->sporco == 1){
                    if ( (testa+pos)->nome != NULL )
                        strcpy (name, (testa+pos)->nome);
                    else
                        strcpy (name, "\0");
                if( strcmp( name, token ) == 0){
                    if( (testa+pos)->file == 1 ){
                        printf("no\n");
                        return;
                    }else{
                        testa = (testa+pos)->figli;
                        slash--;
                        ctrl = 1;
                    }
                }else{
                    pos ++;
                    if(pos == (testa+0)->file) pos = 1;
                    }
            }
        }if(ctrl == 0){
            printf("no\n");
            return;
        }
        free(name);
        name=NULL;
        token = strtok(NULL, "/");
        }
}

void delet(char *percorso, nodo *testa){

    int  i, pos, lenght, save, slash, newl;
    char *token, *perc;
    int ctrl;
    char *name=NULL;
    struct _nodo *appoggio, *padre;

    lenght = strlen(percorso);
    slash = 0;
    for(i=0; i<lenght; i++){
        if (percorso[i] == '/')
            slash++;
    }

    token = strtok(percorso, "/");
    while(token !=NULL){
        name = (char*)calloc((2*strlen(token)+1),sizeof(char));
        if(slash == 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while( (testa+pos)->sporco == 1 ){
                if ( (testa+pos)->nome != NULL)
                    strcpy ( name, (testa+pos)->nome );
                else
                    strcpy ( name, "\0" );
                if( (strcmp( name, token) == 0) ){
                    if( (testa+pos)->file == 1 ){
                        taglia(testa, pos);
                        reset(testa, pos);
                        ctrl = 1;
                        printf("ok\n");
                        return;
                    }
                    appoggio = (testa+pos)->figli;
                    padre = (testa+pos);
                    if( (appoggio+0)->larghezza == 0 ){
                        taglia(testa, pos);
                        reset(testa, pos);
                        free( padre->figli );
                        ctrl = 1;
                        printf("ok\n");
                        return;

                    }
                }
                pos ++;
                if(pos == (testa+0)->file) pos = 1;
            }
            if(ctrl == 0){
            printf("no\n");
            return;
            }
        }

        if(slash > 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while(ctrl == 0 && (testa+pos)->sporco == 1){
                if ( (testa+pos)->nome != NULL )
                    strcpy (name, (testa+pos)->nome);
                else
                    strcpy (name, "\0");
                if( strcmp( name, token ) == 0){
                    if( (testa+pos)->file == 1 ){
                        printf("no\n");
                        return;
                    }else{
                        testa = (testa+pos)->figli;
                        slash--;
                        ctrl = 1;
                    }
                }else{
                    pos ++;
                    if(pos == (testa+0)->file) pos = 1;
                    }
            }
        }if(ctrl == 0){
            printf("no\n");
            return;
        }
        free(name);
        name=NULL;
        token = strtok(NULL, "/");
        }
}

void delet_r(char *percorso, nodo *testa){


    int  i, pos, lenght, save, slash, newl;
    char *token, *perc;
    int ctrl;
    char *name=NULL;
    struct _nodo *appoggio, *padre;

    lenght = strlen(percorso);
    slash = 0;
    for(i=0; i<lenght; i++){
        if (percorso[i] == '/')
            slash++;
    }

    token = strtok(percorso, "/");
    while(token !=NULL){
        name = (char*)calloc((2*strlen(token)+1),sizeof(char));
        if(slash == 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while( (testa+pos)->sporco == 1 ){
                if ( (testa+pos)->nome != NULL)
                    strcpy ( name, (testa+pos)->nome );
                else
                    strcpy ( name, "\0" );
                if( (strcmp( name, token) == 0) ){
                    if( (testa+pos)->file == 1 ){
                        taglia(testa, pos);
                        reset(testa, pos);
                        ctrl = 1;
                        printf("ok\n");
                        return;
                    }else{
                        appoggio = (testa+pos)->figli;
                        padre = (testa+pos);
                        if( (appoggio+0)->larghezza == 0 ){
                            taglia(testa, pos);
                            reset(testa, pos);
                            free( padre->figli );
                            ctrl = 1;
                            printf("ok\n");
                            return;

                        }else{
                            taglia(testa, pos);
                            reset(testa, pos);
                            tagliaric( (padre->figli) );
                            (padre->figli) = NULL;
                            ctrl = 1;
                            printf("ok\n");
                            return;
                        }

                    }
                }
                pos ++;
                if(pos == (testa+0)->file) pos = 1;
            }
            if(ctrl == 0){
            printf("no\n");
            return;
            }
        }

       if(slash > 1){
            pos = hashfunc(token, (testa+0)->file);
            ctrl = 0;
            while(ctrl == 0 && (testa+pos)->sporco == 1){
                if ( (testa+pos)->nome != NULL )
                    strcpy (name, (testa+pos)->nome);
                else
                    strcpy (name, "\0");
                if( strcmp( name, token ) == 0){
                    if( (testa+pos)->file == 1 ){
                        printf("no\n");
                        return;
                    }else{
                        testa = (testa+pos)->figli;
                        slash--;
                        ctrl = 1;
                    }
                }else{
                    pos ++;
                    if(pos == (testa+0)->file) pos = 1;
                    }
            }
        }if(ctrl == 0){
            printf("no\n");
            return;
        }
        free(name);
        name=NULL;
        token = strtok(NULL, "/");
        }
}

void find(char *percorso, nodo * testa){

    struct nodo2 *t=NULL, *tmp;
    int c;
    c = 0;
    t = search(percorso, testa, t);

    for(tmp=t; tmp != NULL; tmp=tmp->next){
        c++;
        if(tmp->parola != NULL) printf("ok %s\n", tmp->parola);
    }
    if(c == 0){
        printf("no\n");
        return;
    }
    return;
}

int hashfunc(char *dir, unsigned int Size){
      unsigned int count, Address =0;
      int pos;
      for (count =0; dir[count]!='\0'; count++){
        Address = Address*dir[count] + dir[count] + count;
      }
      pos = (Address%Size);
      if( pos == 0) {pos = pos+1;}
      return pos;
}

void incoda(nodo *p){

    struct _nodo *nuovo;
    if(nuovo = (nodo *)calloc(16,sizeof(nodo))){
        p->figli = nuovo;
        (nuovo+0)->larghezza = 0;
        (nuovo+0)->indirizzo = 0;
        (nuovo+0)->file = 16;
    }else
        printf("Err Mem\n");

    return;
}

nodo * inserisci(nodo *testa, int pos, char *perc, char *token, nodo *padre){

    int len;
    int percentuale, i;

        len = strlen(token);
        (testa+pos)->nome = (char*)malloc(len+1);
        strcpy( (testa+pos)->nome, token );
        len = strlen(perc);
        (testa+pos)->nperc = (char*)malloc(len+1);
        strcpy( (testa+pos)->nperc, perc);
        (testa+pos)->sporco = 1;                                //** setto le variabili per file e torno al main
        (testa+pos)->file = 0;
        (testa+pos)->ultimo = pos;
        testa = fratelli( testa, pos);
        (testa+0)->larghezza++;
        incoda( testa+pos );

        i = (testa+0)->larghezza * 100;
        percentuale = i / (testa+0)-> file;
        if(percentuale >= 60){
            testa = rehash(testa, padre);
        }
   return testa;
}

nodo * inseriscif(nodo *testa, int pos, char *perc, char *token, nodo *padre){

    int len;
    int percentuale, i;

        len = strlen(token);
        (testa+pos)->nome = (char*)malloc(len+1);
        strcpy( (testa+pos)->nome, token );
        len = strlen(perc);
        (testa+pos)->nperc = (char*)malloc(len+1);
        strcpy( (testa+pos)->nperc, perc);
        (testa+pos)->sporco = 1;                                //** setto le variabili per file e torno al main
        (testa+pos)->file = 1;
        (testa+pos)->ultimo = pos;
        testa = fratelli(testa, pos);
        (testa+0)->larghezza++;
        incoda( testa+pos );

        i = (testa+0)->larghezza * 100;
        percentuale = i / (testa+0)-> file;
        if(percentuale >= 60){
            testa = rehash(testa, padre);
        }
   return testa;
}

nodo * rehash(nodo *testa, nodo *padre){

    struct _nodo *nuovo, *appoggio, *scambia;
    int dim, pos, len;

    dim = (testa+0)->file * 2;
    nuovo = calloc( dim, sizeof(nodo) );
    (nuovo+0)->larghezza = 0;                           //inizializzo fratelli in larghezza e dimensione della nuova tabella in file
    (nuovo+0)->file = dim;
    for(appoggio = testa->fratellodx; appoggio != NULL; appoggio = appoggio->fratellodx){
        pos = hashfunc(appoggio->nome, (nuovo+0)->file);
        while( (nuovo+pos)->sporco == 1 ){
            pos ++;
            if(pos == (nuovo+0)->file) pos = 1;
        }
        if(appoggio->nome != NULL){
            len = strlen(appoggio->nome);
            (nuovo+pos)->nome= (char*)malloc(len+1);
            strcpy( (nuovo+pos)->nome, appoggio->nome);
        }
        if(appoggio->nperc != NULL){
            len = strlen(appoggio->nperc);
            (nuovo+pos)->nperc = (char*)malloc(len+1);
            strcpy( (nuovo+pos)->nperc, appoggio->nperc);
        }
        if(appoggio->cont != NULL){
            len = strlen(appoggio->cont);
            (nuovo+pos)->cont = (char*)malloc(len+1);
            strcpy( (nuovo+pos)->cont, appoggio->cont);
        }
        (nuovo+pos)->file = appoggio->file;
        (nuovo+pos)->sporco = 1;
        (nuovo+pos)->ultimo = pos;
        (nuovo+pos)->figli = appoggio->figli;
         nuovo = fratelli( nuovo, pos);
        (nuovo+0)->larghezza++;
    }

    free(padre->figli);
    padre->figli = nuovo;
    return (nuovo+0);
}

nodo * fratelli(nodo *p, int flag){

    struct _nodo *appoggio;
    int x=0;

    if( (p+0)->larghezza == 0 ){
        (p+0)->fratellodx = (p+flag);
        (p+flag)->fratellodx = NULL;
        (p+flag)->fratellosx = p;
        (p+0)->indirizzo = (flag);
    }                                                              //** assegno ai campi fratello l'indirizzo della cella da puntare
    else{
         x = (p+0)->indirizzo;
        (p+flag)->fratellosx = (p+x);
        (p+flag)->fratellodx = NULL;
        (p+x)->fratellodx = (p+flag);
        (p+0)->indirizzo = (flag);
    }
return p;
}

void reset(nodo *p, int flag){

    (p+0)->larghezza--;
    strcpy((p+flag)->nome, "\0");
    if((p+flag)->cont != NULL){ strcpy((p+flag)->cont, "\0"); }
    strcpy((p+flag)->nperc, "\0");
    (p+flag)->file = 0;
    (p+flag)->figli = NULL;
    return;
}

void taglia (nodo *p, int flag){

    struct _nodo *appoggio;

        if((p+flag)->fratellodx == NULL){
            appoggio = (p+flag)->fratellosx;
            appoggio->fratellodx = NULL;
            (p+flag)->fratellosx = NULL;
            (p+0)->indirizzo = appoggio->ultimo;
        }
        else{
            appoggio = (p+flag)->fratellosx;
            appoggio->fratellodx = (p+flag)->fratellodx;
            appoggio = (p+flag)->fratellodx;
            appoggio->fratellosx = (p+flag)->fratellosx;
            (p+flag)->fratellosx = NULL;
            (p+flag)->fratellodx = NULL;
        }
}

void tagliaric(nodo *p){

    if(p == NULL) return;

    tagliaric(p->figli);
    tagliaric(p->fratellodx);
    free(p->nome);
    free(p->nperc);
    if(p->cont != NULL) { free(p->cont); }
    p->fratellodx = NULL;
    free(p->figli);
    p->figli = NULL;

}

elem_t *in_coda(elem_t *h, char *nperc){

    int i, j=0;
	elem_t *newel, *temp, *prima=NULL;


	if (newel=(elem_t*)malloc(sizeof(elem_t))){
		i = strlen(nperc);
        newel->parola = (char*)malloc(i+1);
        strcpy(newel->parola, nperc);
		newel->next = NULL;
    }
		if(!h)
			h=newel;

		else{
            temp = h;
           while ( j < i && temp != NULL){
                if(nperc[j] == temp->parola[j])
                    j++;
                else if(nperc[j] > temp->parola[j]){
                    prima = temp;
                    temp = temp->next;
                    j = 0;
                }else
                    break;
           }
            newel->next = temp;
           if(prima == NULL) h = newel;
           else prima->next = newel;
		}
	return h;
}

elem_t * search(char *percorso, nodo * testa, elem_t *t){

    if(testa == NULL) return t;

    t = search(percorso, testa->figli, t);

    if(testa->nome != NULL){
        if ( strcmp( (testa->nome), percorso) == 0){
        t = in_coda(t, testa->nperc);
        }
    }
    t = search(percorso, testa->fratellodx, t);
    return t;
}




