#include <3dpc.h>
#include <stdlib.h>
#include <dict.h>

struct meshData
{
	int numVertices;
	float *data;
};

dict *meshDictionary = NULL;

float* MirrorModel(float *data, int *length, p3dpc header)
{
	int num = *length;

	if(!(header.symetryData & 0b00001000))
	{
		for(int i = 0; i < num; i+=5)
		{
			data[i+0]-=header.width>>1;
			data[i+1]-=header.height>>1;
			data[i+2]-=header.depth>>1;
		}

		return data;
	}

	//Model is mirrored. Double pointer capacity and modify mirrored values
	//Double capacity
	float *tmp = realloc(data, num*2*sizeof(float));
	if(!tmp)
	{
		free(data);
		return NULL;
	}

	data = tmp;

	int isPositive = header.symetryData & 0b00000100;

	switch(header.symetryData & 0b00000011)
	{
		//X
		case 1:
			for(int i = 0; i < num; i+=5)
			{
				data[i+num+0] = data[i+0]+(isPositive ? header.width : -header.width);
				data[i+num+1] = data[i+1];
				data[i+num+2] = data[i+2];
				data[i+num+3] = data[i+3];
				data[i+num+4] = data[i+4];
			}
			*length = num*2;
			return data;
		//Y
		case 2:
			for(int i = 0; i < num; i+=5)
			{
				data[i+num+0] = data[i+0];
				data[i+num+1] = data[i+1]+(isPositive ? header.height : -header.height);
				data[i+num+2] = data[i+2];
				data[i+num+3] = data[i+3];
				data[i+num+4] = data[i+4];
			}
			*length = num*2;
			return data;
		//Z
		case 3:
			for(int i = 0; i < num; i+=5)
			{
				data[i+num+0] = data[i+0];
				data[i+num+1] = data[i+1];
				data[i+num+2] = data[i+2]+(isPositive ? header.depth : -header.depth);
				data[i+num+3] = data[i+3];
				data[i+num+4] = data[i+4];
			}
			*length = num*2;
			return data;
		default:
			free(data);
			data = NULL;
			return NULL;
	}
}

PAMesh* LoadMeshFromFile(const char *path, PAShader *shader)
{
	PAMesh *mesh = CreateMesh();

	if(!mesh || !shader)
		return NULL;
	mesh->shader = shader;

	if(!meshDictionary)
		meshDictionary = CreateDict(dict_size_small);

	struct meshData *mData = (struct meshData*)dictGetValue(meshDictionary, (char*)path);

	//File was already loaded once
	if(mData)
	{
		printf("mesh found!\n");
		EnableShader(shader);
		SetPAMeshVertices(mesh, mData->data, mData->numVertices);

		return mesh;
	}
	printf("mesh not found.\n");

	FILE *fptr = fopen(path, "rb");

	if(!fptr)
	{
		free(mesh);
		return NULL;
	}

	fseek(fptr, 0, SEEK_END);
	long fsize = ftell(fptr)-sizeof(p3dpc);
	fseek(fptr, 0, SEEK_SET);

	//Read header and create vertex/face pointer
	p3dpc header;
	fread(&header, sizeof(header), 1, fptr);
	uint8_t *vertices = malloc(header.numVertices*3);
	if(!vertices)
	{
		free(mesh);
		return NULL;
	}

	size_t faceSize = fsize-header.numVertices*3;
	uint8_t *faces = malloc(faceSize);
	if(!faces)
	{
		free(mesh);
		free(vertices);
		return NULL;
	}

	//Read values
	fread(vertices, header.numVertices*3, 1, fptr);
	fread(faces, faceSize, 1, fptr);
	fclose(fptr);

	//Create data pointer
	int dataLength = (faceSize/3)*5;
	float *data = malloc(dataLength*sizeof(float));
	if(!data)
	{
		free(mesh);
		free(vertices);
		free(faces);
		return NULL;
	}

	//Transform vertex/face values into usable values
	for(int i = 0; i < faceSize/3; ++i)
	{
		//TODO:Apply symetry on the correct axis(i.e. excluding width when -X or adding whole width when +X)
		data[i*5+0] = vertices[faces[i*3+0]*3+0];	//X
		data[i*5+1] = vertices[faces[i*3+0]*3+1];	//Y
		data[i*5+2] = vertices[faces[i*3+0]*3+2];	//Z
		data[i*5+3] = faces[i*3+1]/255.0;		//UV.x
		data[i*5+4] = faces[i*3+2]/255.0;		//UV.y
	}

	//Apply mirror if neccessary
	data = MirrorModel(data, &dataLength, header);
	if(!data)
	{
		free(mesh);
		free(vertices);
		free(faces);
		return NULL;
	}

	EnableShader(shader);
	mesh->vertices = data;
	//Apply data pointer to mesh
	SetPAMeshVertices(mesh, data, dataLength);

	mData = malloc(sizeof(struct meshData));

	if(!mData)
	{
		free(mesh);
		free(vertices);
		free(faces);
		return NULL;
	}

	mData->numVertices = dataLength;
	mData->data = data;

	dictAddEntry(meshDictionary, (char*)path, mData);

	return mesh;
}
