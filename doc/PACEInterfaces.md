# PACE interfaces

As C doesn't provide interfacing for it's structures, the PACE game engine however needing some interface capacity, I've decided to copy Tsodings interface logic for panim.<br>
This provides all the interfacing capacity needed for the PACE game engine so far.<br>
<br>
Interfaces operate on a type tag basis, whereby the implementations have to be registered, which will return the interface type tag.<br>
Although all interfaces work on almost always the same struct definition, it is best to define a struct for each new interfaces that is being added.<br>
Using one struct be for multiple interfaces can lead to confusion on the developer side and will probably lead to problems in the future, should a seperate struct be needed. Just be reasonable and make a new struct for each interface.<br>

## The odd ball out

As the only irregular shipped interface <a href="IPADraw.md">IPADraw</a> shows how multiple struct defines are helpful even if more writing is needed.<br>
Drawables visibility such as Text or Meshes can be toggled, and is a inherent property of drawables.<br>
As such the struct contains a 'visible' field which is used to toggle this behaviour.

## OK, but how are interfaces implemented?

Interfaces utilize a virtual table into which the implementations have to be registered.<br>
Firstly a struct is defined for the purpose of containing the function pointers to these implementations:
```C
typedef struct
{
	void (*Interface_Function)(void *data);
}IPACustom_Funcs;
```

This struct will, once instanced, hold the function pointer(s) of the interface implementation.<br>
To manage this a second struct will have to be defined, although it is advised to hide it from the user, as 1. it doesn't concern him so why try to make the sutuation confusing and 2. it cleans up the codebase to streamline the readability from the perspective of the user.<br>
This struct will essentially be our virtual table.

```C
typedef struct
{
	IPACustom_Funcs *items;
	unsigned int count;
	unsigned int capacity;
}IPACustomVTable;
```

By writing and exposing a registering and calling function we can then create and register interface implementations:
```C
IPACustomVTable customVTable = { 0 };

void IPACustom_Interface_Function(IPACustom obj)
{
	if(obj.typeTag > 0 && obj.typeTag <= customVTable.count)
		customVTable.items[obj.typeTag-1].Interface_Function(obj.data);
}

unsigned int RegisterCustomImplementation(IPACustom_Funcs item)
{
	if(!customVTable.items)
	{
		// It is advised to allocate two entries on start as it is very likely that one entry will be used 
		// and in the case of no second one being registered a little bit of processing power is saved, at the cost of some bytes in memory
		customVTable.items = malloc(sizeof(IPACustom_Funcs) * 2);
		if(!customVTable.items)
			return 0;
		customVTable.count = 0;
		customVTable.capacity = 2;
	}

	customVTable.items[customVTable.count++] = item;
	if(customVTable.count < customVTable.capacity)
		return customVTable.count; // By using a index starting at 1 we can use the return value as boolean and check whether the registering was a success or not

	IPACustom_Funcs *tmp = realloc(customVTable.items, customVTable.capacity * 2 * sizeof(IPACustom_Funcs)); // Double capacity
	if(!tmp)
		return 0;
	customVTable.items = tmp;
	customVTable.capacity *= 2;
	return customVTable.count;
}
```

And a registered implementation would look like this:
```C
unsigned int TYPE_TAG_CUSTOM = 0;

typedef struct
{
	unsigned int typeTag;
	void *data;
}IPACustom;

void MyImplementation(void *data)
{
	IPACustom *this = data;
	// Do stuff
	puts("Hello world!");
}

int main(void)
{
	if((TYPE_TAG_CUSTOM = RegisterIPACustomImplementation((IPACustom_Funcs){
		.Interface_Function = MyImplementation
		})))
	{
		puts("Registering failed!");
		return 1;
	}

	IPACustom instance = {
		.typeTag = TYPE_TAG_CUSTOM,
		.data = NULL // Passing NULL as we don't need any data for this example
	};

	IPACustom_Interface_Function(instance);
	return 0;
}
```
