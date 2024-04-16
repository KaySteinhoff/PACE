#include <dict.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int hash(char *key)
{
	int hashval = 0;
	for(char *c = key; *c != '\0'; c++)
		hashval += *c + 31 * hashval;
	return hashval;
}

dict* CreateDict(enum dict_size dictAllocatedSize)
{
	dict *result = malloc(sizeof(dict));
	
	if(!result)
		return NULL;
	
	result->size = (int)dictAllocatedSize;
	result->entries = malloc(result->size*sizeof(dict_entry));
	
	if(!result->entries)
	{
		free(result);
		return NULL;
	}
	
	for(int i = 0; i < result->size; ++i)
		result->entries[i].key = "\0";
	
	return result;
}

void* dictGetValue(dict *targetDict, char *key)
{
	//Get index
	int idx = hash(key) & targetDict->size;
	//get (potentially) first element of linked list
	dict_entry *current = &targetDict->entries[idx];
	
	//check if key matches
	while(strcmp(current->key, key))
	{
		//if it doesn't check if next element doesn't exist
		if(!current->next)
			return NULL;//if it doesnt't return NULL as no entry could be found
		//if it does go to next element
		current = current->next;
	}

	//if it does return void*
	return current->value;
}

int dictAddEntry(dict *targetDict, char *key, void *value)
{
	//Get index
	int idx = hash(key) & targetDict->size;
	dict_entry *current = &targetDict->entries[idx];
	
	//Check if current element isn't used
	while(current->key[0] != '\0')
	{
		//check if used and next element NULL
		if(!current->next)
		{
			//if null create next element
			current->next = malloc(sizeof(dict_entry));
			//if failed to allocate: fail
			if(!current->next)
				return 0;
			//otherwise set current element to new and break from the loop
			current = current->next;
			break;
		}
		
		//if not move to next element
		current = current->next;
	}
	
	current->key = key;
	current->value = value;

	return 1;
}

int dictRemoveEntry(dict *targetDict, char *key)
{
	//Get index
	int idx = hash(key) & targetDict->size;
	//get (potentially) first element of linked list
	dict_entry *current = &targetDict->entries[idx];
	
	//check if key matches
	while(strcmp(current->key, key))
	{
		//if no next element is available: fail
		if(!current->next)
			return 0;
		//Otherwise update current element to next
		current = current->next;
	}
	
	//if it does set all to null except pointer to next element as it may be needed
	current->key = "\0";
	current->value = NULL;
	
	return 1;
}