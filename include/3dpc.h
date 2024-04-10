#include <stdint.h>
#include <cppPACE.h>

/*
3dpc file structure

Header(size 5 bytes):
1 byte containing the number of vertices
3 bytes setting the width, height and depth
1 byte containing the symerty of the model(last four bits aren't used)
	symetry is encoded by:
	1/0   wether it is symetrical
	1/0   positive(1) or negative(0) axis
	11/01 the index of the axis it is to mirrored around(X: 1, Y: 2, Z: 3)

List of all vertices(up to 255 in number)
List of all faces and uv coordinates(v1, v2, v3, uv1, uv2, ...)
*/

typedef struct p3dpc
{
	uint8_t numVertices;
	uint8_t width;
	uint8_t height;
	uint8_t depth;
	uint8_t symetryData;
}p3dpc;

PAMesh* LoadMeshFromFile(const char *path, PAShader *shader);
