#ifndef INC_REPOSITORY
#define INC_REPOSITORY

/* 
 * Repository access functions
 */

void Repository_init  ( int p ); /* percentage for the Super List */
int  Repository_insert( int key, int data );
int  Repository_delete( int key );
int  Repository_get   ( int key, int *data );
void Repository_print ( int print_elements );

#endif /* INC_REPOSITORY */
