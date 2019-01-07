/* Begin by importing your code from Problem 5 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<time.h>
#define WORD_LEN 32
#define DEF_LEN 256
#define COMMAND_LEN 32
#define BLACK 1
#define RED 0
#define WORD_NUM 90000
/* Note your red-black node will need a few more items than
 * your naive node did (e.g., color, parent) */
typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	struct tree_node * parent;
	int color;
	char *word;      /* key   */
	char *definition; /* value */
} Tree_node;
Tree_node* createDummy(Tree_node* parent){
	Tree_node *newnode;
	newnode = (Tree_node*)malloc(sizeof(Tree_node));
	newnode->word = newnode->definition = NULL;
	newnode->parent = parent;
	newnode->color = BLACK;
	newnode->left = newnode->right = NULL;
	return newnode;
}
Tree_node* createNode(Tree_node* parent, char *word, char *definition) {
    Tree_node *newnode;
    newnode = (Tree_node*)malloc(sizeof(Tree_node));
	newnode->word = (char*)malloc(sizeof(char)*WORD_LEN);
	newnode->definition = (char*)malloc(sizeof(char)*DEF_LEN);
    strcpy(newnode->word, word);
    strcpy(newnode->definition, definition);
	newnode->left = createDummy(newnode);
	newnode->right = createDummy(newnode);
	newnode->parent = parent;
	newnode->color = RED;
    return newnode;
}
/* This is the same interface as was in Problem 4, but you will
 * need to implement a number of helper functions to achieve the
 * ability to self balance your dictionary whenever nodes are added.
 * The interfaces for these functions (e.g., rotations,
 * grandparent/sibling helper functions, node swaps, etc) are up
 * for you to design and implement. */
void left(Tree_node* n)
{
              
	Tree_node  *tmp_node; 
	int tmp_color;
	char word[WORD_LEN];      /* key   */
	char definition[DEF_LEN]; /* value */   
	tmp_node  =  n->left;
	strcpy(word, n->word);
    strcpy(definition, n->definition);
	tmp_color = n->color;
	n->left  =  n->right;    
	strcpy(n->word, n->right->word);
	strcpy(n->definition, n->right->definition);  
	n->color = n->right->color;     
	n->right  =  n->left->right; 
	n->right->parent = n;            
	n->left->right  =  n->left->left;              
	n->left->left  =  tmp_node; 
	n->left->left->parent = n->left;
	strcpy(n->left->word, word);
	strcpy(n->left->definition, definition);  
	n->left->color = tmp_color;
}
void right(Tree_node* n)
{	       
	Tree_node  *tmp_node; 
	int tmp_color;
	char word[WORD_LEN];      /* key   */
	char definition[DEF_LEN]; /* value */   
	tmp_node  =  n->right;
	strcpy(word, n->word);
    strcpy(definition, n->definition);
	tmp_color = n->color;
	n->right  =  n->left;    
	strcpy(n->word, n->left->word);
	strcpy(n->definition, n->left->definition);    
	n->color = n->left->color;   
	n->left  =  n->right->left; 
	n->left->parent = n;            
	n->right->left  =  n->right->right;              
	n->right->right  =  tmp_node; 
	n->right->right->parent = n->right;
	strcpy(n->right->word, word);
	strcpy(n->right->definition, definition);  
	n->right->color = tmp_color;
}
Tree_node* grandparent(Tree_node *n)
{
	if (!n->parent) return NULL;
	return n->parent->parent;
}
Tree_node* uncle(Tree_node *n)
{
	Tree_node* g = grandparent(n);
	if(g == NULL) return NULL;
	return strncmp(g->word, n->parent->word, WORD_LEN) > 0 ? g->right : g->left;
}
void insert_case1(Tree_node *n);
void insert_case2(Tree_node *n);
void insert_case3(Tree_node *n);
void insert_case4(Tree_node *n);
void insert_case5(Tree_node *n);

void insert_case5(Tree_node  *n){ 
	Tree_node  *g  =  grandparent(n); 
	n->parent->color  =  BLACK;  
	g->color = RED;  
	if  (n  ==  n->parent->left) {
		right(g);
	}
	else {
		left(g);
	}
}  
void insert_case4(Tree_node  *n)
{  
	Tree_node  *g  =  grandparent(n); 
	if  ((n == n->parent->right)  &&  (n->parent  ==  g->left))  
	{          
		left(n->parent);
	}  
	else  if  ((n  ==  n->parent->left)  &&  (n->parent  ==  g->right))  
	{          
		right(n->parent);
	}  
	insert_case5(n);  
}
/*parent and uncle are red.*/
void insert_case3(Tree_node  *n){   
	Tree_node  *u  =  uncle(n),  *g;   
	if  ((u  !=  NULL)  &&  (u->color  ==  RED))  
	{  
		n->parent->color  =  BLACK;
		u->color  =  BLACK;  
		g  =  grandparent(n); 
		g->color  =  RED;  
		insert_case1(g);    
	} 
	else insert_case4(n);    
} 
/*parent is black. */
void insert_case2(Tree_node  *n)
{    
	if  (n->parent->color  ==  BLACK)  return;  
	/*  Tree  is  still  valid  */    
	else  insert_case3(n);  
} 
/* n is the root of the tree. */
void insert_case1(Tree_node  *n)  
{    
	if  (n->parent  ==  NULL) {
		n->color  =  BLACK;
	}    
	else insert_case2(n);    
}
int add_word(Tree_node ** dictionary, char * word, char * definition)
{
	Tree_node* parent = NULL, *current = *dictionary, *newnode = NULL;
    int res = 0;
    if (!(*dictionary)) {
        *dictionary = createNode(parent, word, definition);
		insert_case1(*dictionary);
        return 0;
    }
    for (current = *dictionary; current->word!= NULL;current = (res > 0) ? current->right : current->left) {
        res = strncmp(word, current->word, WORD_LEN);
        if (res == 0) {
            return 1;
        }
        parent = current;
    }
    newnode = createNode(parent, word, definition);
    res > 0 ? (parent->right = newnode) : (parent->left = newnode);
	insert_case1(newnode);
    return 0; 
}

/* This should be identical to your find_word() function from Problem 4 */
int find_word(Tree_node * dictionary, char * word)
{
    Tree_node *temp = NULL;
    int res = 0;
    if (dictionary == NULL) {
            return 1;
    }
    temp = dictionary;
    res = strncmp(temp->word, word, WORD_LEN);
    while (temp) {
	if (res == 0) return 0;
        temp = (res > 0) ? temp->left : temp->right;
        if ( !temp ) break;
        res = strncmp(temp->word, word, WORD_LEN);
    }
    return 1;
}
/* This should be identical to your print_dictionary() function from Problem 4 */
void print_dictionary(Tree_node * dictionary)
{
	/* Never print null node or NIL */
	if(NULL == dictionary || NULL == dictionary->left)
		return;

	/* In-order traversal(recursion) */
	print_dictionary(dictionary->left);
	printf("%s ", dictionary->word);
	print_dictionary(dictionary->right);
}

/* This should be identical to your clear_dictionary() function from Problem 4 */
void clear_dictionary(Tree_node ** dictionary)
{
    if ( *dictionary == NULL ) return;
    clear(*dictionary);
    *dictionary = NULL;
}
void clear(Tree_node * root)
{    
    if ( root == NULL ) return;
    clear(root->left);
    clear(root->right);
    root->left = NULL;
    root->right = NULL;
    free(root);
    root = NULL;
}
/* This should be identical to your import_file() function from Problem 4 */
int import_file(char * fname, Tree_node ** dictionary)
{
	int word_cnt = 0;
    int i = 0;
	FILE* fp = fopen(fname, "r");
	if(NULL == fp)
	{
		return 0;
	}
	else
	{
		char word[WORD_LEN];
		char definition[DEF_LEN];
		char* pos;
		int error;
		while(EOF != fscanf(fp, "%s", word))
		{
			fgets(definition, DEF_LEN, fp);
			if(NULL != (pos = strchr(definition, '\n')))
				*pos = '\0';
			error = add_word(dictionary, word, definition);
			if(0 == error)
				word_cnt++;
			memset(word, 0x00, sizeof(char) * WORD_LEN);
			memset(definition, 0x00, sizeof(char) * DEF_LEN);
		}
	}

	fclose(fp);
	fp = NULL;
    return word_cnt;
}
int main(void)
{
    Tree_node * dictionary = NULL;
    clock_t start, end;
    int count, i, r;
    long avg;
    char *block;
    srand(time(NULL));
    start = clock();
    count = import_file("alphabetized_dictionary.txt", &dictionary);
    end = clock();
    printf("time of import: \t\t\t%.5f (s)\n", (double)(end - start)/CLOCKS_PER_SEC);
	char arr[WORD_NUM][WORD_LEN];
	FILE* fp = fopen("alphabetized_dictionary.txt", "r");
	if(NULL == fp)
	{
		return 0;
	}
	char definition[DEF_LEN];
	size_t index = 0;
	while(EOF != fscanf(fp, "%s", arr[index]))
	{
		fgets(definition, DEF_LEN, fp);
		memset(definition, 0x00, sizeof(char) * DEF_LEN);
		index++;
	}
	fclose(fp);
	fp = NULL;
start = clock();
    for ( i = 0; i < 100000; i++ ){
        r = rand() % count;
        find_word(dictionary, arr[r]);
    }
	end = clock();
    printf("Avg time of find: \t\t\t%.5f (s)\n", (double)(end-start)*1000000/100000/CLOCKS_PER_SEC);

	return 0;
}
