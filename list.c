#ifndef LIST_H
	#include "list.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKIPLIST_MAX_LEVEL 4
#define NIL NULL
#define MAX_INT 100000
#define MAX_CHAR_FILE 1000

/* ///////////////////////////////////// */
/* ////////// INITIALISATION  ////////// */
/* ///////////////////////////////////// */

/* Fonction allouant la mémoire pour un noeud */
/* Retourne le noeud alloué */

 Node* CreateNode(int value, int key, int level){

	Node* new_node = (Node *)malloc(sizeof(Node)); //Initialisation d'une Node et allocation de mémoire

	if (new_node == NULL) //Si l'allocation n'a pas réussi, alors on arrête le programme
  {
		fprintf(stderr,"Error during memory allocation\n");
		exit(EXIT_FAILURE);
  }

	new_node->value = value; //On initialise la valeur à la valeur passée en paramètre
	new_node->key = key; //On initialise la key à la key passée en paramètre
	new_node->level = level; //On initialise le niveau au niveau passé en paramètre
	new_node->forward = calloc(level+1,sizeof(Node*)); // On alloue de la mémoire pour n Node, n correspondant à level+1

  if (new_node->forward == NULL) //Si l'allocation n'a pas réussi, alors on arrête le programme
  {
		fprintf(stderr,"Error during memory allocation\n");
		exit(EXIT_FAILURE);
  }

  return new_node; //On retourne la nouvelle créée
 }

/* Fonction initialisant une skipList */
/* Retourne une skiplist sans noeud avec la mémoire allouée */

SkipList * Create_SkipList(){

	SkipList * list = (SkipList *)malloc(sizeof(SkipList)); //Initialisation d'une SkipLip et allocation de mémoire
	if ( list == NULL ) //Si l'allocation n'a pas réussi, alors on arrête le programme
  {
     fprintf(stderr,"Error during memory allocation\n");
     exit(EXIT_FAILURE);
  }

  list->header = CreateNode(0,MAX_INT,SKIPLIST_MAX_LEVEL); //On présume que 0 n'est pas une valeur insérable dans le tableau
	list->level = 0; //On initialise le niveau de la SkipList à 0

	return list;
 }

/* ///////////////////////////////////// */
/* /// INSERTION D'UNE NOUVELLE NODE /// */
/* ///////////////////////////////////// */

/* Ne retourne rien, insert une nouvelle node dans une SkipList */

void SkipList_Insert(SkipList *list, int key, int value){
	int i;
	Node *temp; //On initialise une node temporaire
	Node *update[SKIPLIST_MAX_LEVEL+1]; //On initialise un tableau de Node, contenant les pointers des précédentes nodes

	temp = list->header; //La node temporaire pointe vers le header de la SkipList

  //Parcours de la SkipList pour savoir où insérer la nouvelle node
  for (i = list->level; i >= 0; i--)
  {
    while (temp->forward[i] != NIL && temp->forward[i]->key < key) //Tant que la clé recherchée est plus grande que la clé de la node temporaire et que celle-ci est non nulle
    {
        temp = temp->forward[i]; //Alors la node temporaire devient la node suivante
    }
    update[i] = temp; //On insert la node temporaire dans le tableau de node
  }

  temp = temp->forward[0]; //la variable temporaire pointe maintenant sur la dernière node insérée dans le tableau node

  if (temp != NIL && temp->key == key) //si la node temporaire n'est pas nulle et que sa clé est égale à la key recherchée
  {
    temp->value = value; //Alors on initialise la valeur de la node temporaire à la valeur rentrée en paramètre de la fonction
    return; //On arrete la fonction
  }
  else //Si la node temporaire est nulle ou que sa clé n'est pas égale à la clé recherchée
  {
    	int newLevel = randomLevel(); //On initialise un niveau aléatoire

    	if (newLevel > list->level) //Si le niveau aléatoire est supérieur au niveau de la SkipList
    	{
    		for (i = list->level + 1; i <= newLevel; i++) //On parcourt les niveaux de la Skiplist jusqu'au niveau aléatoire
        {
    			update[i] = list->header; //On place le header de la Skiplist dans le tableau de node
    		}
    		list->level = newLevel; //On met à jour le nouveau niveau aléatoire
    	}

      temp=CreateNode(value,key,newLevel); //On alloue de la mémoire à temp qui correspond à l'ensemble de nodes, puis on lui initialise la clé, la valeur et le niveau

			//On met à jours les nouveaux pointeurs
    	for (i = 0; i <= newLevel; i++)
    	{
    		temp->forward[i] = update[i]->forward[i];
    		update[i]->forward[i] = temp;
    	}
    }
}

/* ///////////////////////////////////// */
/* //// RECHERCHE DANS UNE SKIPLIST //// */
/* ///////////////////////////////////// */

/* Retourne la node avec la même clé recherchée, retourne NULL si la clé n'existe pas */

Node *SkipList_Search(SkipList *list, int key){
	int i;
	Node *temp = list->header; //On initialise notre variable temporaire comme le header de la SkipList

	for (i = list->level; i >= 0; i--) //On part du niveau de la SkipList et on décrémente
	{
		while(temp->forward[i]!=NIL && temp->forward[i]->key < key) //Tant que la clé de la node est inférieure à la clé recherchée
    {
			temp = temp->forward[i]; //Alors la node temporaire devient la node d'après
		}
	}

	temp = temp->forward[0]; //La node temporaire est égale à la node où la clé est inférieur ou égale à la clé recherchée
	if (temp->key == key) //Si la clé de la node est égale à la clé recherchée
	{
		return temp; //Alors on retourne la node avec la clé que l'on recherche
	}
  else //Sinon, cela signifie que la clé de la node n'est pas égale à la clé recherchée, la clé n'est donc pas présente dans la SkipList
  {
		return NIL; //On retourne alors NULL
	}
}

/* //////////////////////////////////// */
/* ////// SUPPRESSION D'UNE NODE ////// */
/* //////////////////////////////////// */

/* Retourne 1 si la node demandée est supprimée, retourne 0 si la node n'existe pas */

int SkipList_Delete(SkipList *list, int key){
	int i;
	Node *temp = list->header; //On initialise notre variable temporaire comme le header de la SkipList
	Node *update[SKIPLIST_MAX_LEVEL+1]; //On initialise un tableau de Node, contenant les pointers des précédentes nodes

	for (i = list->level; i >= 0; i--) //On part du niveau de la SkipList et on décrémente
	{
		while(temp->forward[i] != NIL && temp->forward[i]->key < key) //Tant que la clé de la node est inférieure à la clé recherchée
    {
			temp = temp->forward[i]; //Alors la node temporaire devient la node d'après
		}
		update[i] = temp; //On met la variable temporaire dans le tableau de node
	}

	temp = temp->forward[0]; //Une fois la boucle finie, la node temporaire est égale à la derniere node

	if (temp->key == key) //Si la clé de la node est égale à la node cherchée
	{
		for (i = 0; i <= list->level; i++) //On part de 0 et on accrémente jusqu'au niveau de la SkipList
		{
			if (update[i]->forward[i] != temp) //Si le la node du tableau de node n'est pas égale à la node temporaire
			{
				break; //Alors on arrete la boucle for
			}
			update[i]->forward[i] = temp->forward[i]; //Sinon, on place la node temporaire dans le tableau de node
		}

		free_Node(temp); //On libère la mémoire pour la node temporaire

		while(list->level > 0 && list->header->forward[list->level] == NIL) //tant que le level de la SkipList est supérieur à 0 et que les nodes du header pointent vers NULL
    {
			list->level--; //Alors on décrémente le niveau de la SkipList
		}
	}
  else //Si la clé n'existe pas dans la SkipList
  {
    return 0; //Alors on retourne 0
  }
	return 1; //On retourne 1 si tout s'est bien déroulé
}


/* //////////////////////////////////// */
/* ///// AFFICHAGE D'UNE SKIPLIST ///// */
/* //////////////////////////////////// */

/* Ne retourne rien, affiche seulement la SkipList */

void SkipList_Print(SkipList *list)
{
	int i;
  Node *temp; //On initialie une node temporaire
  temp = list->header; //A laquelle on lui donne la node header de la SkipList

  //Affichage du header
  printf("H : ");
  for(i=0; i <= list->level; i++)
  {
      printf("[%d]->", temp->value);
  }
  printf("NIL\n");

	temp = list->header->forward[0]; //La node temporaire prend maintenant la valeur de la node après le header

	while (temp && temp->forward[0] != list->header) //Tant qu'il existe une node et quelle est différente du header
  {
    printf("%d : ", temp->key); //On affiche sa clé
    for(i=0; i <= temp->level ; i++){ //Autant de fois que son niveau
      printf("[%d]->", temp->value); //On affiche sa valeur
    }
    printf("NIL\n"); //et on affiche NULL pour finir la node
		temp = temp->forward[0]; //La node temporaire prend maintenant la valeur de la node d"après
  }
  printf("\n");
}

/* //////////////////////////////////// */
/* /////// AFFICHAGE D'UNE NODE /////// */
/* //////////////////////////////////// */

/* Ne retourne rien, afffiche seulement la valeur de la node */

void Node_Print(Node *node)
{
  if(node == NIL) //Si la node n'existe pas
  {
    printf("Key not found\n"); //On affiche que la clé demandée n'existe pas
  }
  else //Si la node existe
  {
    printf("Value : %d\n", node->value); //On affiche la valeur de la node
  }
}

/* //////////////////////////////////// */
/* ///////// NIVEAU ALÉATOIRE ///////// */
/* //////////////////////////////////// */

/* Retourne un niveau aléatoire */

int randomLevel()
{
	int level = 0;
	while (rand() < RAND_MAX/2 && level < SKIPLIST_MAX_LEVEL)
  {
        level++;
	}
  return level;
}

/* //////////////////////////////////// */
/* ///// LIBERATION DE LA MÉMOIRE ///// */
/* //////////////////////////////////// */

/* Fonction libérant l'espace mémoire alloué pour une skipList */

void free_SkipList(SkipList * list){
  Node *p1, *p2;
  p1=list->header;
  while (p1!=NULL)
  {
    p2 = p1;
    p1 = p1->forward[0];
    free_Node(p2);
  }
  free(list);

}

/* Fonction libérant l'espace mémoire alloué pour un noeud */

void free_Node(Node* p){

  if (p!=NULL)
  {
    free(p->forward);
        free(p);
  }
}
/* //////////////////////////////////// */
/* /// INITIALISE DEPUIS UN FICHIER /// */
/* //////////////////////////////////// */

/* Initialise une SkipList depuis un fichier contenant des valeurs */

void initializeFromFile(SkipList *list, char const * path){

	int i = 0;
	int value, key;
	FILE *file = fopen(path, "r"); //Ouvre le fichier pointé par le dossier, en utilisant le mode "r" comme "read" pour la lecteur simple.
	char buffer[MAX_CHAR_FILE]; // On initialise un tableau mémoire pour stocker des données temporaires, entre la lecture du fichier et son traitement.

	if (file == NULL) // Si le fichier n'existe pas
	{
		printf("Error opening the file !\n"); // On affiche un message d'erreur
		exit (EXIT_FAILURE); // Et on arrête le programme
	}
	else {
		while(fgets(buffer, MAX_CHAR_FILE, file) != NULL) // La fonction fgets lit les caractères du fichier et les range dans le buffer jusqu'à rencontre d'un line-feed. La fonction fgets rend le pointeur buffer en cas de lecture sans erreur, ou NULL dans le cas de fin de fichier ou d'erreur.
		{

			if (strstr(buffer, ",") != NULL) // Vérifie si il y a une virgule avant le buffer. Elle renvoit NULL si une virgule n'est pas trouvée. Ici, c'est si il y en a une virgule.
			{
				key = atoi(strtok(buffer, ",")); // Converti en entier la chaine de caractère renvoyée par la fonction strtok qui retourne le buffer en enlevant toutes les virgules
				value = atoi(strtok(NULL, ",")); // Converti en entier la chaine de caractère renvoyée par la fonction strtok qui retourne la chaine NULL en enlevant toutes les virgules

			}
			else // Sinon, il n'y a pas de virgule, c'est un simple fichier texte, chaque valeur est une nouvelle ligne
			{
				value = atoi(buffer); // Converti le buffer en entier
				key = value; // La key prend la même valeur que la value
			}

			SkipList_Insert(list, key, value); //On insert dans la SkipList

			i++;
		}

		fclose(file); //Si il rencontre une erreur lors du traitement, on ferme le fichier

	}
}
