#ifndef MESHTOVOLCONVERTER_H
#define MESHTOVOLCONVERTER_H
#ifdef __unix__
#ifndef __APPLE__

#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#endif
#endif
#include <vector>

using namespace std;

class MeshToVolumeConverter
{
  public:
    MeshToVolumeConverter()
    {
        //initialize();
    }
    ~MeshToVolumeConverter()
    {

    }
    
	bool convert(const char *pFile, float voxelSize)
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
        return true;
#endif
#endif
    }

	double fill(int x, int y, int z)
	{
#ifdef __unix__
#ifndef __APPLE__
		m_grid->getAccessor().getValue(openvdb::math::Coord(x, y, z));
#endif
#endif
	}

  private:
#ifdef __unix__
#ifndef __APPLE__
	openvdb::FloatGrid::Ptr m_grid;

	void initialize()
	{
		openvdb::initialize();
	}

    bool loadFromDisk(const char *pFile,
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
            // int numVertices = mesh->mNumVertices;
            // int numFaces = mesh->mNumFaces;
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
                float x = mesh->mVertices[i].x;
                float y = mesh->mVertices[i].y;
                float z = mesh->mVertices[i].z;

                openvdb::math::Vec3s point;
                point.init(x, y, z);
                points.push_back(point);
            }
            offset += mesh->mNumFaces;
        }
        // Now we can access the file's contents.
        printf("Import of scene %s succeeded.\n", pFile);
        return true;
    }

	bool sceneToOpenVdbGrid(openvdb::FloatGrid::Ptr &grid,
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

    void openvdbGridToFireGridConverter()
    {
        
    }
#endif
#endif
};
#endif

