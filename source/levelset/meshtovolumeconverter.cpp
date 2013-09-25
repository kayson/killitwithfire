#include "meshtovolumeconverter.h"
#include <openvdb/math/Coord.h>
bool MeshToVolumeConverter::convert(const char *pFile, float voxelSize)
{
#ifdef __unix__
#ifndef __APPLE__
	vector<openvdb::math::Vec3s> points;
	vector<openvdb::math::Vec4ui> triangles;
	bool success;
	success = loadFromDisk(pFile, points, triangles);
	if (!success)
	{
		return false;
	}
	success = sceneToOpenVdbGrid(m_grid, voxelSize, points, triangles);
	if (!success)
	{
		return false;
	}
	openvdb::CoordBBox box = m_grid->evalActiveVoxelBoundingBox();
	openvdb::Coord c = box.getStart();
	openvdb::Coord c2 = box.getEnd();
	std::cout << c << " " << c2 << std::endl;
	return true;
#endif
#endif
}

bool MeshToVolumeConverter::sceneToOpenVdbGrid(openvdb::FloatGrid::Ptr &grid,
						float voxelSize,
						vector<openvdb::math::Vec3s> &points,
						vector<openvdb::math::Vec4ui> &triangles)
{
	std::cout << points.size() << " " << triangles.size() << endl;
	int bandwidth = 100; // Need to fill the whole grid with values.
	openvdb::math::Transform::Ptr transform = openvdb::math::Transform::createLinearTransform(voxelSize);
	openvdb::tools::MeshToVolume<openvdb::FloatGrid> mtv =
			openvdb::tools::MeshToVolume<openvdb::FloatGrid>(transform);

	mtv.convertToLevelSet(points,
							triangles,
							bandwidth,
							bandwidth);
	grid = mtv.distGridPtr();
	if (grid == NULL)
		return false;
	return true;
}

 bool MeshToVolumeConverter::loadFromDisk(const char *pFile,
                      vector<openvdb::math::Vec3s> &points,
                      vector<openvdb::math::Vec4ui> &triangles)
{
	Assimp::Importer importer = Assimp::Importer();
	//check if file exists
	std::ifstream fin(pFile);
	if(!fin.fail()) {
		fin.close();
	}
	else{
		printf("Couldn't open file: %s\n", pFile);
		printf("%s\n", importer.GetErrorString());
		return false;
	}
	
	const aiScene *scene = importer.ReadFile( pFile,
							   aiProcess_CalcTangentSpace       | 
							   aiProcess_JoinIdenticalVertices  |
							   aiProcess_SortByPType);
	
	// If the import failed, report it
	if( !scene)
	{
		printf("Error: ");
		printf("%s\n", importer.GetErrorString());
		return false;
	}
	for (unsigned int n = 0, offset = 0; n < scene->mNumMeshes; ++n)
	{
		const aiMesh *mesh = scene->mMeshes[n];

		if (!mesh)
		{
			cout << "Error";
			return false;
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace* face = &mesh->mFaces[i];
			openvdb::math::Vec4ui v;
			v.init(face->mIndices[0] + offset,
					face->mIndices[1] + offset,
					face->mIndices[2] + offset,
					openvdb::util::INVALID_IDX);
			triangles.push_back(v);
		}
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			std::cout << "Mesh: " << mesh->mVertices[i].x << std::endl;
			float scale = 100.f;
			float x = mesh->mVertices[i].x / scale + 1.0;
			float y = mesh->mVertices[i].y / scale + 1.0;
			float z = mesh->mVertices[i].z / scale + 1.0;

			openvdb::math::Vec3s point;
			point.init(x, y, z);
			std::cout << "Point: " << point.x() << std::endl;
			points.push_back(point);
		}
		offset += mesh->mNumFaces;
	}
	// Now we can access the file's contents.
	printf("Import of scene %s succeeded.\n", pFile);
	return true;
}


