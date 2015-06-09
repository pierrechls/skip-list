typedef struct Node{
    int level;
    int key;
    int value;
    struct Node **forward;
} Node;

typedef struct{
		int level;
    Node *header;
} SkipList;

Node* CreateNode(int value, int key, int level);
SkipList* Create_SkipList();
void SkipList_Insert(SkipList *list, int key, int value);
int randomLevel();
Node *SkipList_Search(SkipList *list, int key);
int SkipList_Delete(SkipList *list, int key);
void SkipList_Print(SkipList *list);
void Node_Print(Node *node);
void free_Node(Node *node);
void free_SkipList(SkipList * list);
void initializeFromFile(SkipList *list, char const * path);
