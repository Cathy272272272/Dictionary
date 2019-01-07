#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#define WORD_LEN 32
#define DEF_LEN 256
#define COMMAND_LEN 32
#define SUCCESS 0
#define FAILURE 1
#define BLACK 1
#define RED 0
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
/* Left Rotation */
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
            printf("Duplicate entry!!\n");
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
        	printf("Binary Search Tree is out of station!!\n");
            return 1;
    }
    temp = dictionary;
    res = strncmp(temp->word, word, WORD_LEN);
    while (temp) {
		if (res == 0) {
		    printf("Word   : %s ", word);
            printf("Definition: %s ", temp->definition);
			return 0;
        }
        temp = (res > 0) ? temp->left : temp->right;
        if ( !temp || !temp->word ) break;
        res = strncmp(temp->word, word, WORD_LEN);
    }
    printf("Search Element not found in Binary Search Tree\n");
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
/*int import_file(char * fname, Tree_node ** dictionary )
{
    FILE *fp = fopen(fname, "r");
    char line[WORD_LEN + DEF_LEN];
    char * word;
    char * definition;
    char * temp = NULL;
    if (fp == 0)
    {
        return 1;
    }
    while (fgets(line, 1000, fp))
    {
		printf("%s ", line);
        temp = strtok(line, "\"");
        word = temp;
        word[strlen(temp) - 1] = '\0';
        temp = strtok(NULL, "\"");
        definition = temp;
        add_word(dictionary, word, definition);
    }
    fclose(fp);
    return 0;
}*/
/* This should be identical to your import_file() function from Problem 4 */
int import_file(char * fname, Tree_node ** dictionary )
{
	int word_cnt = 0;
	FILE* fp = fopen(fname, "r");
	if(NULL == fp)
	{
		printf("Failed to open file: %s\n", strerror(errno));
		return FAILURE;
	}
	else
	{
		/* Claim ignore-error when import file*/
		printf("Option 'ignore-error' is opened when import file...\n");

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
			if(SUCCESS == error)
				word_cnt++;
			memset(word, 0x00, sizeof(char) * WORD_LEN);
			memset(definition, 0x00, sizeof(char) * DEF_LEN);
		}
	}

	fclose(fp);
	fp = NULL;

	printf("\n%d words have been added\n", word_cnt);
	int tree_depth = 0;
	Tree_node* node = *dictionary;
	while(NULL != node && NULL != node->left)
	{
		tree_depth++;
		node = node->right;
	}
	printf("\nThe depth of tree is %d\n", tree_depth);
	return SUCCESS;
}
int main(void)
{
	Tree_node * dictionary = NULL;

	while(1)
	{
		int error;
		char command[COMMAND_LEN];

		/* Get the Users Command */
		printf(">$ ");
		scanf("%s", command);

		/* Add word */
		if( strncmp(command, "add", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			char definition[DEF_LEN];
			char * pos;
			scanf("%s ",word);
			/* Gets the rest of the input */
			fgets(definition, DEF_LEN, stdin);
			if ((pos=strchr(definition, '\n')) != NULL)
				*pos = '\0';

			/* Adds to dictionary */
			error = add_word(&dictionary, word, definition);

			if( error == 0 )
				printf("The word \"%s\" has been added successfully.\n", word);
			else
				printf("Error when adding word \"%s\".\n", word);

		}
		/* Import file */
		if( strncmp(command, "import", COMMAND_LEN) == 0 )
		{
			char fname[DEF_LEN];
			scanf("%s",fname);
			error = import_file(fname, &dictionary );
			if( error == 0 )
				printf("The file \"%s\" has been imported successfully.\n", fname);
			else
				printf("Error when adding file \"%s\".\n", fname);
		}
		/* Find word */
		if( strncmp(command, "find", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			scanf("%s",word);
			error = find_word(dictionary, word);
			if( error == 1 )
				printf("Error when finding word \"%s\".\n", word);
		}
		/* Print dictionary */
		if( strncmp(command, "print", COMMAND_LEN) == 0 )
			print_dictionary(dictionary);
		/* clear dictionary */
		if( strncmp(command, "clear", COMMAND_LEN) == 0 )
			clear_dictionary(&dictionary);
		/* quit program */
		if( strncmp(command, "quit", COMMAND_LEN) == 0 )
			break;
	}

	return 0;
}
