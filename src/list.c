/* 
 * list.c
 */
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* 
 * list_init
 */ 
{
	/*  Initialize the list.  */ 
	list->size = 0;  

/*   
 * list_destroy
 */    
{
	void *data;
	/*  Remove each element. */ 
	while (list_size (list) > 0) { 
		if (list_rem_next (list, NULL, (void **) &data) == 0
		   && list->destroy != NULL) {
			/* Call a user-defined function to free 
			 * dynamically allocated data.  */ 
			list->destroy (data);
		}
}
	 * but clear the structure as a precaution
	 */
	return;

/* 
 * list_ins_next
 */ 
int list_ins_next (List * list, ListElmt * element, const void *data)
{
	ListElmt * new_element;
	/*  Allocate storage for the element.  */ 
	if ((new_element = (ListElmt *) malloc (sizeof (ListElmt))) == NULL)
	  	return -1;
	
	/*  Insert the element into the list.  */ 
	new_element->data = (void *) data;
		if (element == NULL) {
		/*  Handle insertion at the head of the list. */ 
			if (list_size (list) == 0)

			list->tail = new_element;
			new_element->next = list->head;
			list->head = new_element;

	  	} else {
		/*  Handle insertion somewhere other than at the head.  */ 
			if (element->next == NULL)
			
			list->tail = new_element; 
		}
	/*  Adjust the size of the list to account for the inserted element.   */ 
	list->size++;
	return 0;


/* 
 * list_rem_next  
 */ 
int list_rem_next (List * list, ListElmt * element, void **data)
{
	ListElmt * old_element;
	
	/*  Do not allow removal from an empty list.  */ 
	if (list_size (list) == 0)
	return -1;
	
	/*  Remove the element from the list.  */ 
	if (element == NULL) {
		  /*  Handle removal from the head of the list. */ 
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;
		if (list_size (list) == 1)
			list->tail = NULL;
	} else {  /*  Handle removal from somewhere other than the head.   */ 
	
	if(element->next == NULL)
		return -1;
		
	*data = element->next->data;
	old_element = element->next;
	element->next = element->next->next;
	
	if (element->next == NULL)
	}
	free (old_element);	
	list->size--;
	return 0;


{
	ListElmt * old_element;
	if (list_size (list) == 0)	/* Do not allow removal from an empty list. */
	return -1;

	if (element == NULL)
	{
	/* Remove the element from the list.
	 * Handle removal from the head of the list. 
	 */

	*data = list->head->data;
	old_element = list->head;
	list->head = list->head->next;
	if (list_size (list) == 1)
	list->tail = NULL;
	  	} else(element->next == NULL) {
		  /* Handle removal from somewhere other than the head. */ 
		  return -1;

			*data = element->next->data;
			old_element = element->next;
			element->next = element->next->next;
			if (element->next == NULL)
			list->tail = element;
	  	}
		free (old_element);		
		/* Free the a node in the list */		
		list->size--;		
		/* decrement list size */		
		return 0;
