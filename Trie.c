#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    struct Trie *children[26];
    int isEndOfWord;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    //allocate memory for Trie
    struct Trie *root = (struct Trie*) malloc(sizeof(struct Trie));

    // Check if memory allocation was successful
    if (root == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize root node
    for (int i = 0; i < 26; i++) {
        root->children[i] = NULL;
    }
    root->isEndOfWord = 0;

    return root;
}

// Function to initialize a new Trie node
struct Trie* getNode() {
    struct Trie* pNode = (struct Trie*)malloc(sizeof(struct Trie));
    pNode->isEndOfWord = 0;
    for (int i = 0; i < 26; i++)
        pNode->children[i] = NULL;
    return pNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    //initialize the current pointer with root node
   struct Trie* current = pTrie;

   //iterate across length of the string
   for(int i=0;word[i]!='\0';i++){
        // Calculate the index for the current character
        int index = word[i] - 'a';
        
        // Check if the node exists for the current character
        if (current->children[index] == NULL) {
            // Create character node if it doesn't exist
            struct Trie* newnode = getNode();
            // Keep reference for the newly created node
            current->children[index] = newnode;
        }
        // Move current to new node
        current = current->children[index];
   }
   //increment word count
   current->isEndOfWord += 1;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    // Initialize the current pointer to the root of the trie
    struct Trie *current = pTrie;

    // Iterate through each character of the word
    for (int i = 0; word[i] != '\0'; i++) {
        // Calculate the index for the current character
        int index = word[i] - 'a';

        // Check if the node for the current character exists
        if (current->children[index] == NULL) {
            // Word doesn't exist in the trie, return 0
            return 0;
        }

        // Move to the next node
        current = current->children[index];
    }

    // Return the count if the word is found, otherwise return 0
    return current->isEndOfWord;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
        return NULL;

    // Recursively deallocate children nodes
    for (int i = 0; i < 26; i++) {
        deallocateTrie(pTrie->children[i]);
    }

    // Free the current node
    free(pTrie);

    return NULL; // Return NULL to indicate that the Trie is deallocated
}


// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    int numWords = 0;
    //scan in number of words
    fscanf(file, "%d", &numWords);
    char word[100]; // Assuming maximum word length is 100 characters

    // Read words from file until num words 
    for(int i=0;i<numWords;i++) {
        pInWords[i] = (char *)malloc(100 * sizeof(char));
        fscanf(file, "%s", word);
        // Check if memory allocation was successful
        if (pInWords[numWords] == NULL) {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        // Copy the word into the allocated memory
        strcpy(pInWords[i], word);
        
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    //read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i=0;i<numWords;++i)
    {
        printf("%s\n",inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i=0;i<numWords;i++)
    {
        insert(pTrie, inWords[i]);
    }
    
    // parse lineby line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i=0;i<5;i++){
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL){
        printf("There is an error in this program\n");
    }
    return 0;
}