#ifndef DICT_IMPLEMENTATION
#define DICT_IMPLEMENTATION

enum dict_size
{
	dict_size_small = 127,
	dict_size_medium = 511,
	dict_size_large = 2047
};

typedef struct dict_entry
{
	char *key;
	void *value;
	struct dict_entry * next;//Used when multiple keys hash to the same index
}dict_entry;

typedef struct dict
{
	int size;
	dict_entry *entries;
}dict;

dict* CreateDict(enum dict_size dictAllocatedSize);
void* dictGetValue(dict *targetDict, char *key);
int dictAddEntry(dict *targetDict, char *key, void *value);
int dictRemoveEntry(dict *targetDict, char *key);

#endif