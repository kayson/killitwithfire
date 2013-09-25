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
        initialize();
    }
    ~MeshToVolumeConverter()
    {

    }
    
	bool convert(const char *pFile, float voxelSize);

	double fill(double xv, double yv, double zv)
	{
#ifdef __unix__
#ifndef __APPLE__
		int x = xv * 1000;
		int y = yv * 1000;
		int z = zv * 1000;
		double v = m_grid->getAccessor().getValue(openvdb::math::Coord(x, y, z));
		return v;
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
                      vector<openvdb::math::Vec4ui> &triangles);
   
	bool sceneToOpenVdbGrid(openvdb::FloatGrid::Ptr &grid,
                            float voxelSize,
                            vector<openvdb::math::Vec3s> &points,
							vector<openvdb::math::Vec4ui> &triangles);

    void openvdbGridToFireGridConverter()
    {
        
    }
#endif
#endif
};
#endif

