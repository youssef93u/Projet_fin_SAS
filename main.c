#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXTE 2000
#define MAX_MOTS 600
#define MAX_MOT 60
#define MAX_POS 100

typedef struct {
    char valeur[MAX_MOT];     
    int occurance;                  
    int longueur;                  
    int position[MAX_POS];         
    int nbPos;                
} Mot;

void nettoyer(char *txt){
    for(int i=0; txt[i]; i++)
        txt[i] = isalnum(txt[i]) ? tolower(txt[i]) : ' ';
}

int est_palindrome(char *s){
    for(int i=0,j=strlen(s)-1; i<j; i++,j--)
        if(s[i]!=s[j]) 
        return 0;
    return 1;
}

void trier_lettres(char *s,char *msrt){
    strcpy(msrt,s);
    for(int i=0;i<strlen(msrt);i++)
        for(int j=i+1;j<strlen(msrt);j++)
            if(msrt[i]>msrt[j]){ 
                char t=msrt[i]; 
                msrt[i]=msrt[j]; 
                msrt[j]=t; }
}

int chercher(Mot d[],int n,char *w){
    for(int i=0;i<n;i++) 
    if(!strcmp(d[i].valeur,w)) 
    return i;
    return -1;
}


void ajouter(Mot d[],int *n,char *w,int position){
    int i=chercher(d,*n,w);
    if(i>=0){
        d[i].occurance++;
        if(d[i].nbPos<MAX_POS) d[i].position[d[i].nbPos++]=position;
    } else if(*n<MAX_MOTS){
        strcpy(d[*n].valeur,w);
        d[*n].occurance=1;
        d[*n].longueur=strlen(w);
        d[*n].nbPos=1;
        d[*n].position[0]=position;
        (*n)++;
    }
}

void analyser(char *txt,Mot d[],int *n){
    *n=0;
    nettoyer(txt);
    char *tok=strtok(txt," \t\n");
    int position=0;
    while(tok){
        ajouter(d,n,tok,++position);
        tok=strtok(NULL," \t\n");
    }
    printf("%d mots uniques.\n",*n);
}
void afficher(Mot d[],int n){
    for(int i=0;i<n;i++){ 
        printf("%-12s occurance:%d, longueur:%d, position:",d[i].valeur,d[i].occurance,d[i].longueur);
        for(int j=0;j<d[i].nbPos;j++) 
        printf("%d ",d[i].position[j]);
        printf("\n");
    }
}

void rechercher(Mot d[],int n,char *w,int partiel){
    int trouve=0;
    for(int i=0;i<n;i++){
        if((!partiel && !strcmp(d[i].valeur,w)) || (partiel && strstr(d[i].valeur,w))){
            printf("%s occurance:%d\n",d[i].valeur,d[i].occurance);
            trouve=1;
        }
    }
    if(!trouve) printf("Aucun resultat.\n");
}

void trier(Mot d[],int n,int critere){
    for(int i=0;i<n-1;i++) 
        for(int j=i+1;j<n;j++){
            int c=0;
            if(critere==1) c=strcmp(d[i].valeur,d[j].valeur)>0;   
            else if(critere==2) 
            c=d[i].occurance<d[j].occurance;              
            else if(critere==3) 
            c=d[i].longueur>d[j].longueur;               
            if(c){ 
                Mot t=d[i]; 
                d[i]=d[j]; 
                d[j]=t; }
        }
}

void statistiques(Mot d[],int n){
    int total=0, idxFreq=0, idxLong=0, idxCourt=0; 
    double somme=0;
    for(int i=0;i<n;i++){ 
        total+=d[i].occurance; 
        somme+=d[i].longueur*d[i].occurance;
        if(d[i].occurance>d[idxFreq].occurance) idxFreq=i;
        if(d[i].longueur>d[idxLong].longueur) idxLong=i;
        if(d[i].longueur<d[idxCourt].longueur) idxCourt=i;
    }
    printf("Total:%d\nuniques:%d\nmoyLongueur:%.2f\n",total,n,somme/total);
    printf("Frequent:%s\nLong:%s\nCourt:%s\n",
           d[idxFreq].valeur,d[idxLong].valeur,d[idxCourt].valeur);
}

void analyses(Mot d[],int n){
    printf("Palindromes:\n"); 
    int trouve=0;
    for(int i=0;i<n;i++) 
        if(est_palindrome(d[i].valeur)&&d[i].longueur>1){ 
            printf(" %s",d[i].valeur); 
            trouve=1; 
        }
    if(!trouve) 
    printf(" Aucun"); 
    printf("\n");

    printf("Nuage de mots:\n"); 
    for(int i=0;i<n;i++){ 
        printf("%-10s ",d[i].valeur); 
        for(int j=0;j<d[i].occurance;j++) 
        putchar('*'); 
        printf("\n"); 
    }
}

int main(){
    Mot d[MAX_MOTS]; 
    int n=0,choix; 
    char texte[MAX_TEXTE],mot[MAX_MOT];

    do{
        printf("\n1.Analyser texte\n2.Afficher dictionnaire\n3.Recherche exacte\n4.Recherche partielle\n5.Trier\n6.Statistiques\n7.Analyses\n8.Quitter\nChoix:");
        scanf("%d",&choix); getchar();

        switch(choix){
            case 1: printf("Entrez le texte:\n");
            fgets(texte,MAX_TEXTE,stdin); 
            analyser(texte,d,&n); 
                   break;
            case 2: afficher(d,n); 
                   break;
            case 3: printf("Mot exacte:"); 
            scanf("%s",mot); rechercher(d,n,mot,0); 
                   break;
            case 4: printf("Mot partiel:"); 
            scanf("%s",mot); rechercher(d,n,mot,1); 
                   break;
            case 5: printf("1.Alphabetique 2.Frequence 3.Longueur\n");
            scanf("%d",&choix); trier(d,n,choix); 
            afficher(d,n);
                   break;
            case 6: statistiques(d,n); 
                   break;
            case 7: analyses(d,n); 
                   break;
            case 8: printf("A bientot !\n"); 
                   break;
        }
    }while(choix!=8);

    return 0;
}