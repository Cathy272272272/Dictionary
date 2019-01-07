/* Begin by importing your finished code from problem 4 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define WORD_LEN 32
#define DEF_LEN 256
#define COMMAND_LEN 32
#define SUCCESS 1
#define FAILURE 0
#define WORD_NUM 90000

typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	char word[WORD_LEN];      /* key   */
	char definition[DEF_LEN]; /* value */
} Tree_node;
Tree_node* createNode(char *word, char *definition) {
        Tree_node *newnode;
        newnode = (Tree_node*)malloc(sizeof(Tree_node));
        strcpy(newnode->word, word);
        strcpy(newnode->definition, definition);
        newnode->left = newnode->right = NULL;
        return newnode;
}
int add_word(Tree_node ** dictionary, char * word, char * definition)
{
	Tree_node* parent = NULL, *current = *dictionary, *newnode = NULL;
    int res = 0;
    if (!(*dictionary)) {
        *dictionary = createNode(word, definition);
        return 0;
    }
    for (current = *dictionary; current !=NULL; current = (res > 0) ? current->right : current->left) {
        res = strncmp(word, current->word, WORD_LEN);
        if (res == 0) {
            return 1;
        }
        parent = current;
    }
    newnode = createNode(word, definition);
    res > 0 ? (parent->right = newnode) : (parent->left = newnode);
    return 0;    
}

/*int find_word(Tree_node * dictionary, char * word)
{
	Tree_node *temp = NULL;
    int res = 0;
    if (dictionary == NULL) {
            return 1;
    }
    temp = dictionary;
    res = strncmp(temp->word, word, WORD_LEN);
    while (temp) {
		if (res == 0) {
			return 0;
        }
        temp = (res > 0) ? temp->left : temp->right;
        if ( !temp ) break;
        res = strncmp(temp->word, word, WORD_LEN);
    }
    return 1;
}*/
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
		if (res == 0) {
			return 0;
        }
        temp = (res > 0) ? temp->left : temp->right;
        if ( !temp || !temp->word ) break;
        res = strncmp(temp->word, word, WORD_LEN);
    }
    return 1;
}

int delete_word(Tree_node ** dictionary, Tree_node * parent, char * word)
{
    Tree_node *current = NULL, *successor = NULL, *temp = NULL;
    int flag = 0;
    /* parent does not exist. */
    if ( parent == NULL ) return 1;
    flag = strncmp(parent->word, word, WORD_LEN);
    if ( flag != 0 ) {
        current = (flag > 0) ? parent->left : parent->right;
        while (current) {
            printf("Word   : %s ", current->word);
            printf("Definition: %s ", current->definition);
            flag = strncmp(current->word, word, WORD_LEN);
		    if (flag == 0) {
			    break;
            }
            parent = current;
            current = (flag > 0) ? current->left : current->right;
        }
    }
    if ( current == NULL && parent != *dictionary )
    {
        printf("Search Element not found in Binary Search Tree\n");
        return 1;
    }
    if ((current == NULL && (parent->right == NULL || parent->left == NULL )) || (current != NULL && (current->right == NULL || current->left == NULL)))
    {
        /* no children. */
        if ((current == NULL && parent->left == NULL && parent->right == NULL ) || (current != NULL && current->left == NULL && current->right == NULL)) {
            /* if root is deleted. */
            if ( current == NULL )
            {
                free(*dictionary);
                *dictionary = NULL;
            }
            else
            {
                strncmp(parent->word, current->word, WORD_LEN) > 0 ? (parent->left = NULL) : (parent->right = NULL);
                free(current);
            }
            return 0;
        }
        /* no right/left child. */
        else {
            /* if root is deleted. */
            if (current == NULL) 
            {
                /*change the root to left/right child. */
                *dictionary = parent->right == NULL ? parent->left : parent->right;
                free(parent);
                return 0;
            }
            if ( current->left == NULL ) temp = current ->right;
            else temp = current->left;
		    strncmp(parent->word, temp->word, WORD_LEN) > 0 ? (parent->left = temp) : (parent->right = temp); 
            free(current);
            return 0; 
        }  
    }
    /* two children. */
   else
    {   
        /* Find the least element in the right tree of current. */
        if ( current == NULL ) temp = parent->right;
        else temp = current->right;
        if ( !temp->left ){
            if ( current == NULL ) {
                temp->left = parent->left;
                *dictionary = temp;
                free(parent);
                return 0;
            }
            temp->left = current->left;
            strncmp(parent->word, current->word, WORD_LEN) > 0 ? (parent->left = temp) : (parent->right = temp); 
            free(current);
            return 0;
        }
        successor = temp;
        while (1) {
            if (!successor->left) break;
            temp = successor;
            successor = temp->left;
        }
        temp->left = successor->right;
        /*put the least element in the place of the element we delete. */
        if ( current == NULL ) temp = *dictionary;
        else temp = current;
        successor->left = temp->left;
        successor->right = temp->right;
		if ( current == NULL ) (*dictionary) = successor;
        else strncmp(parent->word, current->word, WORD_LEN) > 0 ? (parent->left = temp) : (parent->right = temp); 
        free(temp);
        return 0;
    }
}

void print_dictionary(Tree_node * dictionary)
{
    /*inorder transverse. */
    if ( dictionary == NULL ) return;
    print_dictionary(dictionary->left);
    printf("%s %s \n", dictionary->word, dictionary->definition);
    print_dictionary(dictionary->right);
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
void clear_dictionary(Tree_node ** dictionary)
{
    if ( *dictionary == NULL ) return;
    clear(*dictionary);
    *dictionary = NULL;
}
/* This should be identical to your import_file() function from Problem 4 */
int import_file(char * fname, Tree_node ** dictionary)
{
    int word_cnt = 0;
    int i = 0;
    char word[WORD_LEN];
    char definition[DEF_LEN];
    char* pos;
    int error;
	FILE* fp = fopen(fname, "r");
	if(NULL == fp)
	{
		return FAILURE;
	}
	else
	{
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
		/*printf("%s ", arr[index]);*/
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
