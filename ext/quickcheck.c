#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ruby.h>

const char delimiters[] = " ";
char *piece, *piece2, *one, *two;

struct list {
   char *word;
   struct list *next;
};

typedef struct list item;


static VALUE check(VALUE self, VALUE first, VALUE second) {
	char *first_name = StringValuePtr(first);
	char *second_name = StringValuePtr(second);
	return INT2NUM(compare(first_name,second_name));
}

/* ruby calls this to load the extension */
void Init_quickcheck(void) {

  VALUE klass = rb_define_class("Quickcheck",rb_cObject);
  rb_define_singleton_method(klass,"check", check, 2);
}

static int levenshtein (const char * word1,int len1,const char * word2,int len2){
    int matrix[len1 + 1][len2 + 1];
    int i;
    for (i = 0; i <= len1; i++) {
        matrix[i][0] = i;
    }
    for (i = 0; i <= len2; i++) {
        matrix[0][i] = i;
    }
    for (i = 1; i <= len1; i++) {
        int j;
        char c1;

        c1 = word1[i-1];
        for (j = 1; j <= len2; j++) {
            char c2;

            c2 = word2[j-1];
            if (c1 == c2) {
                matrix[i][j] = matrix[i-1][j-1];
            }
            else {
                int delete;
                int insert;
                int substitute;
                int minimum;

                delete = matrix[i-1][j] + 1;
                insert = matrix[i][j-1] + 1;
                substitute = matrix[i-1][j-1] + 1;
                minimum = delete;
                if (insert < minimum) {
                    minimum = insert;
                }
                if (substitute < minimum) {
                    minimum = substitute;
                }
                matrix[i][j] = minimum;
            }
        }
    }
    return matrix[len1][len2];
}
     
int compare (char string[], char string2[]){

    item *curr, *head;
    item *curr2, *head2;

    head = NULL;
    head2 = NULL;

    one = strdup(string); 
    two = strdup(string2); 

    int match = 0;

    while((piece = strsep(&one, delimiters)) != NULL){
        curr = (item *)malloc(sizeof(item));
        curr->word = (char *)malloc((strlen(piece) * sizeof(char))+1);
        strcpy(curr->word,piece);
        curr->next = head;
        head = curr;
	}

    curr = head;

	while((piece = strsep(&two, delimiters)) != NULL){
        curr2 = (item *)malloc(sizeof(item));
        curr2->word = (char *)malloc((strlen(piece) * sizeof(char))+1);
        strcpy(curr2->word,piece);
        curr2->next  = head2;
        head2 = curr2;
    }

    free(piece);

    curr2 = head2;
	
    while(curr) {
        while(curr2) {
            int len1 = strlen(curr->word);
            int len2 = strlen(curr2->word);
            int shortest;
            int distance = 2;
            int dist = levenshtein (curr->word,strlen(curr->word),curr2->word,strlen(curr2->word));
            if(len1 < len2){
                shortest = len1;
            } else {
                shortest = len2;
            }
            if(shortest <= 3){
                distance = 1;
            } 
            if (dist<=distance){
                match = 1;
                break;
            }
            curr2 = curr2->next;
        }
        if(match==1){
            break;
        }
        curr2 = head2;
        curr = curr->next;
    }

    // free up memory
    curr = head;
    while(curr){
        free(curr->word);
        item *to_free = curr;
        curr = curr->next;
        free(to_free);
    }
    curr2 = head2;
    while(curr2){
        free(curr2->word);
        item *to_free = curr2;
        curr2 = curr2->next;
        free(to_free);
    }

    free(one);
    free(two);

    return match;

}
