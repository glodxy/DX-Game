#include "octree.h"
//#include "octree.cpp"
#include "../../Component/Collider/Collider.h"
#include <set>
namespace GRCore {
    class PhysicalWorld {
    public:
        static PhysicalWorld* GetPhysicalWorld();
        static void Initialize(XMFLOAT3 start,XMFLOAT3 end);

        XMFLOAT3 start;
        XMFLOAT3 end;
        //OctreeTree *octree;
        OctreeNode* octree;
        OctreeNode* terrain;

        void update(float timeDelta);
        void deleteObject(Collider* collider);
        void radioDetect(XMFLOAT3 direction);
        void soloUpdate(OctreeNode* node, OctreeNode* terrain, int area);
        void updateCollision(OctreeNode* node, OctreeNode *terrain);
    private:
        PhysicalWorld(){PhysicalWorld(XMFLOAT3(10,-10,-10),XMFLOAT3(10,10,10));}
        PhysicalWorld(XMFLOAT3 start, XMFLOAT3 end);
        static PhysicalWorld* mWorld;
        std::set<Collider*> removedCollider;

    };
}
