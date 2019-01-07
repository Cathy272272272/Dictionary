#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define WORD_LEN 32
#define DEF_LEN 256
#define COMMAND_LEN 32
#define SUCCESS 0
#define FAILURE 1

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
            printf("Duplicate entry!!\n");
            return 1;
        }
        parent = current;
    }
    newnode = createNode(word, definition);
    res > 0 ? (parent->right = newnode) : (parent->left = newnode);
    return 0;    
}

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
        if ( !temp ) break;
        res = strncmp(temp->word, word, WORD_LEN);
    }
    printf("Search Element not found in Binary Search Tree\n");
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
int find_depth(Tree_node * dictionary)
{
    /*inorder transverse. */
    if ( dictionary == NULL ) return 0;
    int left = find_depth(dictionary->left);
    int right = find_depth(dictionary->right);
    return 1 + (left > right ? left : right);
}
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
    tree_depth = find_depth(node);
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

		/* Get the User's Command */
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
		/* Delete word */
		if( strncmp(command, "delete", COMMAND_LEN) == 0 )
		{
			char word[WORD_LEN];
			scanf("%s",word);
			error = delete_word(&dictionary, dictionary, word);
			if( error == 0 )
				printf("The word \"%s\" has been deleted successfully.\n", word);
			else
				printf("Error when deleting word \"%s\".\n", word);
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
