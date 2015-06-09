#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKIPLIST_MAX_LEVEL 3
#define NIL NULL
#define MAX_INT 10000
#define MAX_CHAR_FILE 1000

#ifndef LIST_H
	#include "list.h"
#endif

int main(int argc, char const *argv[])
{
	SkipList *list;
	list = Create_SkipList();
	int i;
	int arr[] = {3, 6, 7, 9, 12, 17};
	int keys[] = {3, 4, 5, 7};
	int delete[] = {1, 2, 6, 3};


	/* Si le programme est exécuté avec un arguement, on insert la les valeurs du fichier texte */
	if (argc > 1)
	{
		initializeFromFile(list, argv[1]);
		printf("//////////////// INSERT ///////////////\n\n");
		SkipList_Print(list);

	}

	/* Sinon, on insert depuis notre tableau */
	else
	{
	  printf("//////////////// INSERT ///////////////\n\n");

	  for (i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
	  {
	    SkipList_Insert(list, arr[i], arr[i]);
	  }
	  SkipList_Print(list);
	}


  printf("//////////////// SEARCH ///////////////\n\n");

  for (i = 0; i < sizeof(keys)/sizeof(keys[0]); i++)
  {
    Node* x = SkipList_Search(list, keys[i]);
    printf("Key =  %d -> ", keys[i]);
    Node_Print(x);
  }

  printf("\n");

  printf("//////////////// DELETE ///////////////\n\n");

  for (i = 0; i < sizeof(delete)/sizeof(delete[0]); i++)
  {
    printf("Delete key %d\n", delete[i]);
    if(SkipList_Delete(list, delete[i]) == 0)
    {
      printf("Key not found, SkipList has not changed \n\n");
    }
    else
    {
      printf("This is the new SkipList : \n");
      SkipList_Print(list);
    }
  }
	free_SkipList(list);
	return 0;
}
