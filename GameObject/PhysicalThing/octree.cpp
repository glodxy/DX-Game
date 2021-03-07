#include"octree.h"
using namespace GRCore;
OctreeNode::OctreeNode(float _x,float _y,float _z,float _xSize,float _ySize,float _zSize,OctreeType _octreeNodeType,int _level,int _maxLevel):
	x(_x),
	y(_y),
	z(_z),
	xSize((_xSize-_x)),
	ySize((_ySize - _z) ),
	zSize((_zSize - _z) ),
	octreeNodeType(_octreeNodeType),
	level(_level),
	maxLevel(_maxLevel)
{
	//初始子节点都赋空值
	bottom_left_front_node=nullptr;
	bottom_right_front_node=nullptr;
	bottom_left_back_node=nullptr;
	bottom_right_back_node=nullptr;
	top_left_front_node=nullptr;
	top_right_front_node=nullptr;
	top_left_back_node=nullptr;
	top_right_back_node=nullptr;
	std::cout << "我的世界大小" << x << " " << y << " " << z << " " << std::endl;
	std::cout << "我的世界大小" << xSize << " " << ySize << " " << zSize << " " << std::endl;
}
 
OctreeNode::~OctreeNode()
{
	
}
 
 // 是否包含物体
//px, py, pz是区域最小值
//bool OctreeNode::IsContain(float px,float py,float pz,float x_size,float y_size,float z_size,Collider *object) const
//{
//	float x = (object->x)-(object->radius);
//	float y = (object->y)-(object->radius);
//	float z = (object->z)-(object->radius);
//	float xSize = (object->radius) * 2;
//	float ySize = (object->radius) * 2;
//	float zSize = (object->radius) * 2;
//	if(x >= px
//		&&x+xSize <= px+x_size
//		&&y >= py
//		&&y+ySize <= py+y_size
//		&&z>=pz
//		&&z+zSize <= pz+z_size)
//		return true;
//	return false;
//}
 // 重载 是否包含节点
bool OctreeNode::IsContain(float px,float py,float pz,float x_size,float y_size,float z_size,OctreeNode *octreeNode) const
{
	if(octreeNode->x>=px
		&&octreeNode->x+octreeNode->xSize <= px+x_size
		&&octreeNode->y >= py
		&&octreeNode->y+octreeNode->ySize <= py+y_size
		&&octreeNode->z>=pz
		&&octreeNode->z+octreeNode->zSize <= pz+z_size)
		return true;
	return false;
}
 //是否相交
bool OctreeNode::IsInterSect(float px,float py,float pz,float x_size,float y_size,float z_size,OctreeNode *octreeNode) const
{
	if(octreeNode->x>px+x_size
		||octreeNode->x+xSize<px
		||octreeNode->y>py+y_size
		||octreeNode->y+ySize<py
		||octreeNode->z+zSize<pz
		||octreeNode->z>pz+z_size)
		return false;
	return true;
}
bool OctreeNode::IsInterSect(float px, float py, float pz, float x_size, float y_size, float z_size, Collider* object) const
{
	float x, y, z, xSize, ySize, zSize;
	if (object->getName() == "SphereCollider") {
		SphereCollider* object2 = dynamic_cast<SphereCollider*>(object);
		x = (object2->x) - (object2->radius);
		y = (object2->y) - (object2->radius);
		z = (object2->z) - (object2->radius);
		xSize = (object2->radius) * 2;
		ySize = (object2->radius) * 2;
		zSize = (object2->radius) * 2;
		std::cout << "this data all" << x << " " << y << " " << z << " " << xSize << " " << ySize << " " << zSize << " " << std::endl;
	}
	else if (object->getName()=="RectCollider") {
		RectCollider* object2 = dynamic_cast<RectCollider*>(object);
		x = (object2->x) - (object2->extent.x);
		y = (object2->y) - (object2->extent.y);
		z = (object2->z) - (object2->extent.z);
		xSize = (object2->extent.x) * 2;
		ySize = (object2->extent.y) * 2;
		zSize = (object2->extent.z) * 2;
	}
	else {
		TriangleCollider* object2 = dynamic_cast<TriangleCollider*>(object);

		x = min(min(object2->a1.x, object2->a2.x), object2->a3.x);
		y = min(min(object2->a1.y, object2->a2.y), object2->a3.y);
		z = min(min(object2->a1.z, object2->a2.z), object2->a3.z);
		xSize = max(max(object2->a1.x, object2->a2.x), object2->a3.x)-x;
		ySize = max(max(object2->a1.y, object2->a2.y), object2->a3.y)-y;
		zSize = max(max(object2->a1.z, object2->a2.z), object2->a3.z)-z;
	}
	if (x > px+x_size
		|| x+xSize  < px
		|| y > py+y_size
		|| y + ySize < py
		|| z > pz+z_size
		|| z + zSize < pz )
		return false;
	return true;
}
void OctreeNode::BuildTree(int level)
{
	//递归地进行八叉树空间划分，直到最大深度
	if(level==maxLevel)
		return;
	//创建子节点
	bottom_left_back_node=new OctreeNode(x,y,z,xSize/2,ySize/2,zSize/2,BOTTOM_LEFT_BACK,level+1,maxLevel);
	bottom_right_back_node=new OctreeNode(x+xSize/2,y,z,xSize/2,ySize/2,zSize/2,BOTTOM_RIGHT_BACK,level+1,maxLevel);
	top_left_back_node=new OctreeNode(x,y+ySize/2,z,xSize/2,ySize/2,zSize/2,TOP_LEFT_BACK,level+1,maxLevel);
	top_right_back_node=new OctreeNode(x+xSize/2,y+ySize/2,z,xSize/2,ySize/2,zSize/2,TOP_RIGHT_BACK,level+1,maxLevel);
	bottom_left_front_node=new OctreeNode(x,y,z+zSize/2,xSize/2,ySize/2,zSize/2,BOTTOM_LEFT_FRONT,level+1,maxLevel);
	bottom_right_front_node=new OctreeNode(x+xSize/2,y,z+zSize/2,xSize/2,ySize/2,zSize/2,BOTTOM_RIGHT_FRONT,level+1,maxLevel);
	top_left_front_node=new OctreeNode(x,y+ySize/2,z+zSize/2,xSize/2,ySize/2,zSize/2,TOP_LEFT_FRONT,level+1,maxLevel);
	top_right_front_node=new OctreeNode(x+xSize/2,y+ySize/2,z+zSize/2,xSize/2,ySize/2,zSize/2,TOP_RIGHT_FRONT,level+1,maxLevel);
	//递归构造
	bottom_left_front_node->BuildTree(level+1);
	bottom_right_front_node->BuildTree(level+1);
	bottom_left_back_node->BuildTree(level+1);
	bottom_right_back_node->BuildTree(level+1);
	top_left_front_node->BuildTree(level+1);
	top_right_front_node->BuildTree(level+1);
	top_left_back_node->BuildTree(level+1);
	top_right_back_node->BuildTree(level+1);
}
 
void OctreeNode::InsertObject(Collider *object)
{
	std::cout << "你在哪你是谁" << object->getPosition().x<<" " << object->getPosition().y<<" "<< object->getPosition().z << std::endl;
	if(level==maxLevel)
	{
		//插入结尾
		objectList.push_back(object);
		std::cout << "可不可以出现" << objectList.size() << std::endl;
		return;
	}
	//递归地插入，直到叶子节点
	//1
	//std::cout<<
	if(bottom_left_back_node&&IsInterSect(x,y,z,xSize/2,ySize/2,zSize/2,object))
	{
		bottom_left_back_node->InsertObject(object);
		return;
	}
	//2
	if(bottom_right_back_node&& IsInterSect(x+xSize/2,y,z,xSize/2,ySize/2,zSize/2,object))
	{
		bottom_right_back_node->InsertObject(object);
		return;
	}
	//3
	if(top_left_back_node&& IsInterSect(x,y+ySize/2,z,xSize/2,ySize/2,zSize/2,object))
	{
		top_left_back_node->InsertObject(object);
		return;
	}
	//4
	if(top_right_back_node&& IsInterSect(x+xSize/2,y+ySize/2,z,xSize/2,ySize/2,zSize/2,object))
	{
		top_right_back_node->InsertObject(object);
		return;
	}
	//5
	if(bottom_left_front_node&& IsInterSect(x,y,z+zSize/2,xSize/2,ySize/2,zSize/2,object))
	{
		bottom_left_front_node->InsertObject(object);
		return;
	}
	//6
	if(bottom_right_front_node&& IsInterSect(x+xSize/2,y,z+zSize/2,xSize/2,ySize/2,zSize/2,object))
	{
		bottom_right_front_node->InsertObject(object);
		return;
	}
	//7
	if(top_left_front_node&& IsInterSect(x,y+ySize/2,z+zSize/2,xSize/2,ySize/2,zSize/2,object))
	{
		top_left_front_node->InsertObject(object);
		return;
	}
	//8
	if(top_right_front_node&& IsInterSect(x+xSize/2,y+ySize/2,z+zSize/2,xSize/2,ySize/2,zSize/2,object))
	{
		top_right_front_node->InsertObject(object);
		return;
	}
}

// 获得该区域内的物体列表
std::list<Collider*> OctreeNode::GetObjectsAt(float px,float py,float pz,float x_size,float y_size,float z_size)
{
	if(level==maxLevel)
		return objectList;
	std::list<Collider *> resObjects;
	//递归地判断选定区域是否与某个节点相交（包含或被包含都算）
	//1
	if(bottom_left_front_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,bottom_left_front_node))
	{
		std::list < Collider * > childObjects1 = bottom_left_front_node->GetObjectsAt(px, py, pz, x_size, y_size, z_size);
		resObjects.insert(resObjects.end(),childObjects1.begin(),childObjects1.end());
	}
	//2
	if(bottom_right_front_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,bottom_right_front_node))
	{
		std::list<Collider *> childObjects2=bottom_right_front_node->GetObjectsAt(px,py,pz,x_size,y_size,z_size);
		resObjects.insert(resObjects.end(),childObjects2.begin(),childObjects2.end());
	}
	//3
	if(bottom_left_back_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,bottom_left_back_node))
	{
		std::list<Collider *> childObjects3=bottom_left_back_node->GetObjectsAt(px,py,pz,x_size,y_size,z_size);
		resObjects.insert(resObjects.end(),childObjects3.begin(),childObjects3.end());
	}
	//4
	if(bottom_right_back_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,bottom_right_back_node))
	{
		std::list<Collider *> childObjects4=bottom_right_back_node->GetObjectsAt(px,py,pz,x_size,y_size,z_size);
		resObjects.insert(resObjects.end(),childObjects4.begin(),childObjects4.end());
	}
	//5
	if(top_left_front_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,top_left_front_node))
	{
		std::list<Collider *> childObjects5=top_left_front_node->GetObjectsAt(px,py,pz,x_size,y_size,z_size);
		resObjects.insert(resObjects.end(),childObjects5.begin(),childObjects5.end());
	}
	//6
	if(top_right_front_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,top_right_front_node))
	{
		std::list<Collider *> childObjects6=top_right_front_node->GetObjectsAt(px,py,pz,x_size,y_size,z_size);
		resObjects.insert(resObjects.end(),childObjects6.begin(),childObjects6.end());
	}
	//7
	if(top_left_back_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,top_left_back_node))
	{
		std::list<Collider *> childObjects7=top_left_back_node->GetObjectsAt(px,py,pz,x_size,y_size,z_size);
		resObjects.insert(resObjects.end(),childObjects7.begin(),childObjects7.end());
	}
	//8
	if(top_right_back_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,top_right_back_node))
	{
		std::list<Collider *> childObjects8=top_right_back_node->GetObjectsAt(px,py,pz,x_size,y_size,z_size);
		resObjects.insert(resObjects.end(),childObjects8.begin(),childObjects8.end());
	}
 
	return resObjects;
}
 
void OctreeNode::RemoveObjectsAt(float px,float py,float pz,float x_size,float y_size,float z_size)
{
	if(level==maxLevel)
	{
		if(IsInterSect(px,py,pz,x_size,y_size,z_size,this))
        {
		    objectList.clear(); //到了叶子节点且完全被包含就把该节点存储的对象清空
        }
		return;
	}
	//递归地判断选定区域是否与某个节点相交（包含或被包含都算）,没有相交就不用再递归了
	//1
	if(bottom_left_front_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,bottom_left_front_node))
		bottom_left_front_node->RemoveObjectsAt(px,py,pz,x_size,y_size,z_size);
	//2
	if(bottom_right_front_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,bottom_right_front_node))
		bottom_right_front_node->RemoveObjectsAt(px,py,pz,x_size,y_size,z_size);
	//3
	if(bottom_left_back_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,bottom_left_back_node))
		bottom_left_back_node->RemoveObjectsAt(px,py,pz,x_size,y_size,z_size);
	//4
	if(bottom_right_back_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,bottom_right_back_node))
		bottom_right_back_node->RemoveObjectsAt(px,py,pz,x_size,y_size,z_size);
	//5
	if(top_left_front_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,top_left_front_node))
		top_left_front_node->RemoveObjectsAt(px,py,pz,x_size,y_size,z_size);
	//6
	if(top_right_front_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,top_right_front_node))
		top_right_front_node->RemoveObjectsAt(px,py,pz,x_size,y_size,z_size);
	//7
	if(top_left_back_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,top_left_back_node))
		top_left_back_node->RemoveObjectsAt(px,py,pz,x_size,y_size,z_size);
	//8
	if(top_right_back_node&&IsInterSect(px,py,pz,x_size,y_size,z_size,top_right_back_node))
		top_right_back_node->RemoveObjectsAt(px,py,pz,x_size,y_size,z_size);
}
/*
enum OctreeType        
{
	ROOT,                   //根
	BOTTOM_LEFT_FRONT,		// 1 
	BOTTOM_RIGHT_FRONT,		// 2 
	BOTTOM_LEFT_BACK,		// 3 
	BOTTOM_RIGHT_BACK,      // 4 
	TOP_LEFT_FRONT,         // 5 
	TOP_RIGHT_FRONT,        // 6 
	TOP_LEFT_BACK,          // 7 
	TOP_RIGHT_BACK          // 8   
};

*/
// 必须是第二层八叉树调用
std::list<Collider *> OctreeNode::getYourDaughter(int area){
	switch (area) {
	case BOTTOM_LEFT_FRONT:
		return this->bottom_left_front_node->objectList;
	case BOTTOM_RIGHT_FRONT:
		return this->bottom_right_front_node->objectList;
	case BOTTOM_LEFT_BACK:
		return this->bottom_left_back_node->objectList;
	case BOTTOM_RIGHT_BACK:
		return this->bottom_right_back_node->objectList;

	case TOP_LEFT_FRONT:
		return this->top_left_front_node->objectList;
	case TOP_RIGHT_FRONT:
		return this->top_right_front_node->objectList;
	case TOP_LEFT_BACK:
		return this->top_left_back_node->objectList;
	case TOP_RIGHT_BACK:
		return this->top_right_back_node->objectList;
	}
		

}