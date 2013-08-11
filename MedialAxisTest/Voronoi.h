/*
 * Voronoi.h
 *
 *  Created on: Mar 14, 2009
 *      Author: dankrusi
 */

#ifndef VORONOI_H_
#define VORONOI_H_

#include <QList>
#include <QHash>

#include <limits.h>
#include <math.h>


#import "vector2.h"
#import "BinaryPriorityQueue.h"

#define VVZero Vector2D(0,0)
#define VVUnkown Vector2D(NAN,NAN)
#define VVInfinate Vector2D(INT_MAX,INT_MAX)



class VoronoiGraph;
class VoronoiEdge;
class VNode;
class VDataNode;
class VEdgeNode;
class VEvent;
class VDataEvent;
class VCircleEvent;


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

class VoronoiGraph
{
public:
	QList<Vector2D> *Vertizes;
	QList<VoronoiEdge*> *Edges;
public:
	VoronoiGraph();
	virtual ~VoronoiGraph();
};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

class VoronoiEdge
{

public:
	Vector2D RightData;
	Vector2D LeftData;
	Vector2D VVertexA;
	Vector2D VVertexB;

public:
	VoronoiEdge();
	virtual ~VoronoiEdge();
	void AddVertex(Vector2D V);
};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


// VoronoiVertex or VoronoiDataPoint are represented as Vector2D

class VNode
{
public:
	enum VNodeType { VNodeTypeVNode, VNodeTypeVDataNode, VNodeTypeVEdgeNode };

private:
	VNode *_Parent;
	VNode *_Left;
	VNode *_Right;

public:
	VNode();
	virtual ~VNode();
	virtual VNodeType type();
	VNode* Left();
	void SetLeft(VNode *value);
	VNode* Right();
	void SetRight(VNode *value);
	VNode* Parent();
	void SetParent(VNode *value);
	void Replace(VNode *ChildOld, VNode *ChildNew);
	static VDataNode* FirstDataNode(VNode *Root);
	static VDataNode* LeftDataNode(VDataNode *Current);
	static VDataNode* RightDataNode(VDataNode *Current);
	static VEdgeNode* EdgeToRightDataNode(VDataNode *Current);
	static VDataNode* FindDataNode(VNode *Root, double ys, double x);
	static VNode* ProcessDataEvent(VDataEvent *e, VNode *Root, VoronoiGraph *VG, double ys, QList<VNode*> *CircleCheckList);
	static VNode* ProcessCircleEvent(VCircleEvent *e, VNode *Root, VoronoiGraph *VG, double ys, QList<VNode*> *CircleCheckList);
	static VCircleEvent* CircleCheckDataNode(VDataNode *n, double ys);
};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


class VDataNode : public VNode
{

public:
	Vector2D DataPoint;

public:

	VDataNode(Vector2D DP);
	virtual ~VDataNode();
	VNodeType type();
};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


class VEdgeNode : public VNode
{

private:
	VoronoiEdge *_Edge;

public:
	bool Flipped;

public:

	VEdgeNode(VoronoiEdge *E, bool Flipped);
	virtual ~VEdgeNode();
	VNodeType type();
	VoronoiEdge *Edge();
	void SetEdge(VoronoiEdge *Edge);
	double Cut(double ys, double x);
};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


class VEvent
{
public:
	enum VEventType { VEventTypeVEvent, VEventTypeVDataEvent, VEventTypeVCircleEvent };

public:

	VEvent();
	virtual ~VEvent();
	virtual VEventType type();
	virtual double Y() = 0;
	virtual double X() = 0;
	int CompareTo(VEvent & obj);
	virtual bool operator==(VEvent & v) {
		return CompareTo(v) == 0;
	};
	virtual bool operator<(VEvent & v) {
		return CompareTo(v) == -1;
	};
	virtual bool operator>(VEvent & v) {
		return CompareTo(v) == +1;
	};
};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


class VDataEvent : public VEvent
{

public:
	Vector2D DataPoint;

public:

	VDataEvent(Vector2D DP);
	virtual ~VDataEvent();
	virtual VEventType type();
	virtual double Y();
	virtual double X();
};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


class VCircleEvent : public VEvent
{
private:
	VDataNode *_NodeN;
	VDataNode *_NodeL;
	VDataNode *_NodeR;

public:
	Vector2D Center;
	bool Valid;


public:

	VCircleEvent();
	virtual ~VCircleEvent();
	virtual VEventType type();
	virtual double Y();
	virtual double X();
	VDataNode* NodeN();
	VDataNode* NodeL();
	VDataNode* NodeR();
	void SetNodeN(VDataNode *value);
	void SetNodeL(VDataNode *value);
	void SetNodeR(VDataNode *value);

};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



class Fortune
{
public:
	static double ParabolicCut(double x1, double y1, double x2, double y2, double ys);
	static Vector2D CircumCircleCenter(Vector2D A, Vector2D B, Vector2D C);
	static VoronoiGraph* ComputeVoronoiGraph(QList<Vector2D> *Datapoints);
	static int ccw(double P0x, double P0y, double P1x, double P1y, double P2x, double P2y, bool PlusOneOnZeroDegrees);
	static double dist(double x1, double y1, double x2, double y2);
};










////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


VoronoiGraph::VoronoiGraph() {
	Vertizes = new QList<Vector2D>;
	Edges = new QList<VoronoiEdge*>;
}

VoronoiGraph::~VoronoiGraph() {
	delete Vertizes;
	delete Edges;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


VoronoiEdge::VoronoiEdge() {
	RightData = VVZero;
	LeftData = VVZero;
	VVertexA = VVUnkown;
	VVertexB = VVUnkown;
}

VoronoiEdge::~VoronoiEdge() {

}

void VoronoiEdge::AddVertex(Vector2D V)
{
	if(VVertexA == VVUnkown)
		VVertexA = V;
	else if(VVertexB == VVUnkown)
		VVertexB = V;
	else throw QString("Tried to add third vertex!");
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


VNode::VNode() {
	_Parent = NULL;
	_Left = NULL;
	_Right = NULL;
}

VNode::~VNode() {

}

VNode::VNodeType VNode::type() {
	return VNode::VNodeTypeVNode;
}

VNode* VNode::Left() {
	return _Left;
}

void VNode::SetLeft(VNode *value) {
	_Left = value;
	value->SetParent(this);
}

VNode* VNode::Right() {
	return _Right;
}

void VNode::SetRight(VNode *value) {
	_Right = value;
	value->SetParent(this);
}

VNode* VNode::Parent() {
	return _Parent;
}

void VNode::SetParent(VNode *value) {
	_Parent = value;
}


void VNode::Replace(VNode *ChildOld, VNode *ChildNew)
{
	if(Left()==ChildOld)
		SetLeft(ChildNew);
	else if(Right()==ChildOld)
		SetRight(ChildNew);
	else throw QString("Child not found!");
	ChildOld->SetParent(NULL);
}

VDataNode* VNode::FirstDataNode(VNode *Root)
{
	VNode *C = Root;
	while(C->Left()!=NULL)
		C = C->Left();
	return (VDataNode*)C;
}

VDataNode* VNode::LeftDataNode(VDataNode *Current)
{
	VNode *C = Current;
	//1. Up
	do
	{
		if(C->Parent()==NULL)
			return NULL;
		if(C->Parent()->Left() == C)
		{
			C = C->Parent();
			continue;
		}
		else
		{
			C = C->Parent();
			break;
		}
	}while(true);
	//2. One Left
	C = C->Left();
	//3. Down
	while(C->Right()!=NULL)
		C = C->Right();
	return (VDataNode*)C; // Cast statt 'as' damit eine Exception kommt
}

VDataNode* VNode::RightDataNode(VDataNode *Current)
{
	VNode *C = Current;
	//1. Up
	do
	{
		if(C->Parent()==NULL)
			return NULL;
		if(C->Parent()->Right() == C)
		{
			C = C->Parent();
			continue;
		}
		else
		{
			C = C->Parent();
			break;
		}
	}while(true);
	//2. One Right
	C = C->Right();
	//3. Down
	while(C->Left()!=NULL)
		C = C->Left();
	return (VDataNode*)C; // Cast statt 'as' damit eine Exception kommt
}

VEdgeNode* VNode::EdgeToRightDataNode(VDataNode *Current)
{
	VNode *C = Current;
	//1. Up
	do
	{
		if(C->Parent()==NULL)
			throw QString("No Left Leaf found!");
		if(C->Parent()->Right() == C)
		{
			C = C->Parent();
			continue;
		}
		else
		{
			C = C->Parent();
			break;
		}
	}while(true);
	return (VEdgeNode*)C;
}

VDataNode* VNode::FindDataNode(VNode *Root, double ys, double x)
{
	VNode *C = Root;
	do
	{
		if(C->type()==VNodeTypeVDataNode)
			return (VDataNode*)C;
		if(((VEdgeNode*)C)->Cut(ys,x)<0)
			C = C->Left();
		else
			C = C->Right();
	}while(true);
}

/// <summary>
/// Will return the new root (unchanged except in start-up)
/// </summary>
VNode* VNode::ProcessDataEvent(VDataEvent *e, VNode *Root, VoronoiGraph *VG, double ys, QList<VNode*> *CircleCheckList)
{
	if(Root==NULL)
	{
		Root = new VDataNode(e->DataPoint);
		//CircleCheckList = new VDataNode[] {(VDataNode)Root};
		CircleCheckList->append(Root);
		return Root;
	}
	//1. Find the node to be replaced
	VNode *C = VNode::FindDataNode(Root, ys, e->DataPoint.x());
	//2. Create the subtree (ONE Edge, but two VEdgeNodes)
	VoronoiEdge *VE = new VoronoiEdge();
	VE->LeftData = ((VDataNode*)C)->DataPoint;
	VE->RightData = e->DataPoint;
	VE->VVertexA = VVUnkown;
	VE->VVertexB = VVUnkown;
	VG->Edges->append(VE);

	VNode *SubRoot;
	if(abs(VE->LeftData.y()-VE->RightData.y())<1e-10)
	{
		if(VE->LeftData.x()<VE->RightData.x())
		{
			SubRoot = new VEdgeNode(VE,false);
			SubRoot->SetLeft(new VDataNode(VE->LeftData));
			SubRoot->SetRight(new VDataNode(VE->RightData));
		}
		else
		{
			SubRoot = new VEdgeNode(VE,true);
			SubRoot->SetLeft(new VDataNode(VE->RightData));
			SubRoot->SetRight(new VDataNode(VE->LeftData));
		}
		//CircleCheckList = new VDataNode[] {(VDataNode)SubRoot.Left,(VDataNode)SubRoot.Right};
		CircleCheckList->append(SubRoot->Left());
		CircleCheckList->append(SubRoot->Right());
	}
	else
	{
		SubRoot = new VEdgeNode(VE,false);
		SubRoot->SetLeft(new VDataNode(VE->LeftData));
		SubRoot->SetRight(new VEdgeNode(VE,true));
		SubRoot->Right()->SetLeft(new VDataNode(VE->RightData));
		SubRoot->Right()->SetRight(new VDataNode(VE->LeftData));
		//CircleCheckList = new VDataNode[] {(VDataNode)SubRoot.Left,(VDataNode)SubRoot.Right.Left,(VDataNode)SubRoot.Right.Right};
		CircleCheckList->append(SubRoot->Left());
		CircleCheckList->append(SubRoot->Right()->Left());
		CircleCheckList->append(SubRoot->Right()->Right());
	}

	//3. Apply subtree
	if(C->Parent() == NULL)
		return SubRoot;
	C->Parent()->Replace(C,SubRoot);
	return Root;
}

VNode* VNode::ProcessCircleEvent(VCircleEvent *e, VNode *Root, VoronoiGraph *VG, double ys, QList<VNode*> *CircleCheckList)
{
	VDataNode *a = NULL;
	VDataNode *b = NULL;
	VDataNode *c = NULL;
	VEdgeNode *eu = NULL;
	VEdgeNode *eo = NULL;
	b = e->NodeN();
	a = VNode::LeftDataNode(b);
	c = VNode::RightDataNode(b);
	if(a==NULL || b->Parent()==NULL || c==NULL || !(a->DataPoint == e->NodeL()->DataPoint) || !(c->DataPoint == e->NodeR()->DataPoint))
	{
		//CircleCheckList = new VDataNode[]{};
		return Root; // Abbruch da sich der Graph verändert hat
	}
	eu = (VEdgeNode*)b->Parent();
	//CircleCheckList = new VDataNode[] {a,c};
	CircleCheckList->append(a);
	CircleCheckList->append(c);
	//1. Create the new Vertex
	Vector2D VNew = Vector2D(e->Center.x(),e->Center.y());
	VNew = Vector2D(
						Fortune::ParabolicCut(a->DataPoint.x(),a->DataPoint.y(),c->DataPoint.x(),c->DataPoint.y(),ys),
						(ys + a->DataPoint.y())/2 - 1/(2*(ys-a->DataPoint.y()))*(VNew.x()-a->DataPoint.x())*(VNew.x()-a->DataPoint.x())
					);
	VG->Vertizes->append(VNew);
	//2. Find out if a or c are in a distand part of the tree (the other is then b's sibling) and assign the new vertex
	if(eu->Left()==b) // c is sibling
	{
		eo = VNode::EdgeToRightDataNode(a);

		// replace eu by eu's Right
		eu->Parent()->Replace(eu,eu->Right());
	}
	else // a is sibling
	{
		eo = VNode::EdgeToRightDataNode(b);

		// replace eu by eu's Left
		eu->Parent()->Replace(eu,eu->Left());
	}
	eu->Edge()->AddVertex(VNew);
//			///////////////////// uncertain
//			if(eo==eu)
//				return Root;
//			/////////////////////
	eo->Edge()->AddVertex(VNew);
	//2. Replace eo by new Edge
	VoronoiEdge *VE = new VoronoiEdge();
	VE->LeftData = a->DataPoint;
	VE->RightData = c->DataPoint;
	VE->AddVertex(VNew);
	VG->Edges->append(VE);

	VEdgeNode *VEN = new VEdgeNode(VE, false);
	VEN->SetLeft(eo->Left());
	VEN->SetRight(eo->Right());
	if(eo->Parent() == NULL)
		return VEN;
	eo->Parent()->Replace(eo,VEN);
	return Root;
}

VCircleEvent* VNode::CircleCheckDataNode(VDataNode *n, double ys)
{
	VDataNode *l = VNode::LeftDataNode(n);
	VDataNode *r = VNode::RightDataNode(n);
	if(l==NULL || r==NULL || l->DataPoint==r->DataPoint || l->DataPoint==n->DataPoint || n->DataPoint==r->DataPoint)
		return NULL;
	if(Fortune::ccw(l->DataPoint.x(),l->DataPoint.y(),n->DataPoint.x(),n->DataPoint.y(),r->DataPoint.x(),r->DataPoint.y(),false)<=0)
		return NULL;
	Vector2D Center = Fortune::CircumCircleCenter(l->DataPoint,n->DataPoint,r->DataPoint);
	VCircleEvent *VC = new VCircleEvent();
	VC->SetNodeN(n);
	VC->SetNodeL(l);
	VC->SetNodeR(r);
	VC->Center = Center;
	VC->Valid = true;
	if(VC->Y()>=ys)
		return VC;
	return NULL;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

VDataNode::VDataNode(Vector2D DP) : VNode()
{
	DataPoint = DP;
}

VDataNode::~VDataNode() {

}

VNode::VNodeType VDataNode::type() {
	return VNode::VNodeTypeVDataNode;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



VEdgeNode::VEdgeNode(VoronoiEdge *E, bool Flipped) : VNode()
{
	this->_Edge = E;
	this->Flipped = Flipped;
}

VEdgeNode::~VEdgeNode() {

}

VNode::VNodeType VEdgeNode::type() {
	return VNode::VNodeTypeVEdgeNode;
}

VoronoiEdge* VEdgeNode::Edge() {
	return _Edge;
}

void VEdgeNode::SetEdge(VoronoiEdge *Edge) {
	_Edge = Edge;
}

double VEdgeNode::Cut(double ys, double x)
{
	if(!Flipped)
		return round(x-Fortune::ParabolicCut(_Edge->LeftData.x(), _Edge->LeftData.y(), _Edge->RightData.x(), _Edge->RightData.y(), ys));
	return round(x-Fortune::ParabolicCut(_Edge->RightData.x(), _Edge->RightData.y(), _Edge->LeftData.x(), _Edge->LeftData.y(), ys));
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////





VEvent::VEvent() {

}

VEvent::~VEvent() {

}

VEvent::VEventType VEvent::type() {
	return VEvent::VEventTypeVEvent;
}

int VEvent::CompareTo(VEvent & obj)
{
//		if(!(obj is VEvent))
//			throw new ArgumentException("obj not VEvent!");
//		int i = Y.CompareTo(((VEvent)obj).Y);
//		if(i!=0)
//			return i;
//		return X.CompareTo(((VEvent)obj).X);
	if(this->Y() != obj.Y()) {
		if(this->Y() < obj.Y()) 	return -1;
		else						return +1;
	} else {
		if(this->X() < obj.X()) 	return -1;
		else						return +1;
	}
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////




VDataEvent::VDataEvent(Vector2D DP) : VEvent() {
	this->DataPoint = DP;
}

VDataEvent::~VDataEvent() {

}

VEvent::VEventType VDataEvent::type() {
	return VEvent::VEventTypeVDataEvent;
}

double VDataEvent::Y()
{
	return DataPoint.y();
}

double VDataEvent::X()
{
	return DataPoint.x();
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////




VCircleEvent::VCircleEvent() : VEvent() {
	Valid = true;
	Center = VVZero;
	_NodeL = NULL;
	_NodeR = NULL;
	_NodeN = NULL;
}

VCircleEvent::~VCircleEvent() {

}

VEvent::VEventType VCircleEvent::type() {
	return VEvent::VEventTypeVCircleEvent;
}

double VCircleEvent::Y()
{
	// return Math.Round(Center[1]+MathTools.Dist(NodeN.DataPoint[0],NodeN.DataPoint[1],Center[0],Center[1]),10);
	return round(Center.y()+Fortune::dist(NodeN()->DataPoint.x(),NodeN()->DataPoint.y(),Center.x(),Center.y()));
}

double VCircleEvent::X()
{
	return Center.x();
}


VDataNode* VCircleEvent::NodeN() {
	return _NodeN;
}

VDataNode* VCircleEvent::NodeL() {
	return _NodeL;
}

VDataNode* VCircleEvent::NodeR() {
	return _NodeR;
}

void VCircleEvent::SetNodeN(VDataNode *value) {
	_NodeN = value;
}

void VCircleEvent::SetNodeL(VDataNode *value) {
	_NodeL = value;
}

void VCircleEvent::SetNodeR(VDataNode *value) {
	_NodeR = value;
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

double Fortune::dist(double x1, double y1, double x2, double y2)
{
	return sqrtf((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

int Fortune::ccw(double P0x, double P0y, double P1x, double P1y, double P2x, double P2y, bool PlusOneOnZeroDegrees)
{
	double dx1, dx2, dy1, dy2;
	dx1 = P1x - P0x; dy1 = P1y - P0y;
	dx2 = P2x - P0x; dy2 = P2y - P0y;
	if (dx1*dy2 > dy1*dx2) return +1;
	if (dx1*dy2 < dy1*dx2) return -1;
	if ((dx1*dx2 < 0) || (dy1*dy2 < 0)) return -1;
	if ((dx1*dx1+dy1*dy1) < (dx2*dx2+dy2*dy2) && PlusOneOnZeroDegrees)
		return +1;
	return 0;
}

double Fortune::ParabolicCut(double x1, double y1, double x2, double y2, double ys)
{
//			y1=-y1;
//			y2=-y2;
//			ys=-ys;
//

	if(abs(x1-x2)<1e-10 && abs(y1-y2)<1e-10)
	{
//				if(y1>y2)
//					return double.PositiveInfinity;
//				if(y1<y2)
//					return double.NegativeInfinity;
//				return x;
		throw QString("Identical datapoints are not allowed!");
	}

	if(abs(y1-ys)<1e-10 && abs(y2-ys)<1e-10)
		return (x1+x2)/2;
	if(abs(y1-ys)<1e-10)
		return x1;
	if(abs(y2-ys)<1e-10)
		return x2;
	double a1 = 1/(2*(y1-ys));
	double a2 = 1/(2*(y2-ys));
	if(abs(a1-a2)<1e-10)
		return (x1+x2)/2;
	double xs1 = 0.5/(2*a1-2*a2)*(4*a1*x1-4*a2*x2+2*sqrtf(-8*a1*x1*a2*x2-2*a1*y1+2*a1*y2+4*a1*a2*x2*x2+2*a2*y1+4*a2*a1*x1*x1-2*a2*y2));
	double xs2 = 0.5/(2*a1-2*a2)*(4*a1*x1-4*a2*x2-2*sqrtf(-8*a1*x1*a2*x2-2*a1*y1+2*a1*y2+4*a1*a2*x2*x2+2*a2*y1+4*a2*a1*x1*x1-2*a2*y2));
	xs1=round(xs1);
	xs2=round(xs2);
	if(xs1>xs2)
	{
		double h = xs1;
		xs1=xs2;
		xs2=h;
	}
	if(y1>=y2)
		return xs2;
	return xs1;
}

Vector2D Fortune::CircumCircleCenter(Vector2D A, Vector2D B, Vector2D C)
{
	if(A==B || B==C || A==C)
		throw QString("Need three different points!");
	double tx = (A.x() + C.x())/2;
	double ty = (A.y() + C.y())/2;

	double vx = (B.x() + C.x())/2;
	double vy = (B.y() + C.y())/2;

	double ux,uy,wx,wy;

	if(A.x() == C.x())
	{
		ux = 1;
		uy = 0;
	}
	else
	{
		ux = (C.y() - A.y())/(A.x() - C.x());
		uy = 1;
	}

	if(B.x() == C.x())
	{
		wx = -1;
		wy = 0;
	}
	else
	{
		wx = (B.y() - C.y())/(B.x() - C.x());
		wy = -1;
	}

	double alpha = (wy*(vx-tx)-wx*(vy - ty))/(ux*wy-wx*uy);

	return Vector2D(tx+alpha*ux,ty+alpha*uy);
}

VoronoiGraph* Fortune::ComputeVoronoiGraph(QList<Vector2D> *Datapoints)
{
	BinaryPriorityQueue<VEvent*> *PQ = new BinaryPriorityQueue<VEvent*>();
	QHash<VDataNode*,VCircleEvent*> *CurrentCircles = new QHash<VDataNode*,VCircleEvent*>();
	VoronoiGraph *VG = new VoronoiGraph();
	VNode *RootNode = NULL;
//	foreach(Vector2D V, Datapoints)
//	{
//		PQ.Push(new VDataEvent(V));
//	}
	for(int i = 0; i < Datapoints->count(); i++) {
		PQ->push(new VDataEvent(Datapoints->at(i)));
	}
	while(PQ->count()>0)
	{
		VEvent *VE = PQ->pop();
		QList<VNode*> *CircleCheckList = new QList<VNode*>();
		if(VE->type() == VEvent::VEventTypeVDataEvent) {
			//RootNode = VNode::ProcessDataEvent(VE as VDataEvent,RootNode,VG,VE.Y,out CircleCheckList);
			RootNode = VNode::ProcessDataEvent((VDataEvent*)VE,RootNode,VG,VE->Y(),CircleCheckList);
		}
		else if(VE->type() == VEvent::VEventTypeVCircleEvent)
		{
			CurrentCircles->remove(((VCircleEvent*)VE)->NodeN());
			if(!((VCircleEvent*)VE)->Valid)
				continue;
			RootNode = VNode::ProcessCircleEvent((VCircleEvent*)VE,RootNode,VG,VE->Y(),CircleCheckList);
		}
		else throw QString("Got event of unknown type !");
		//foreach(VDataNode VD in CircleCheckList)
		for(int i = 0; i < CircleCheckList->count(); i++) {

			VDataNode *VD = (VDataNode*)CircleCheckList->at(i);

			if(CurrentCircles->contains(VD))
			{
				CurrentCircles->value(VD)->Valid=false;
				CurrentCircles->remove(VD);
			}
			VCircleEvent *VCE = VNode::CircleCheckDataNode(VD,VE->Y());
			if(VCE!=NULL)
			{
				PQ->push(VCE);
				CurrentCircles->insert(VD,VCE); //CurrentCircles[VD]=VCE;
			}
		}
		if(VE->type() == VEvent::VEventTypeVDataEvent)
		{
			Vector2D DP = ((VDataEvent*)VE)->DataPoint;
			//foreach(VCircleEvent VCE in CurrentCircles.Values)
			for(int i = 0; i < CurrentCircles->values().count(); i++) {
				VCircleEvent *VCE = CurrentCircles->values().at(i);
				if(Fortune::dist(DP.x(),DP.y(),VCE->Center.x(),VCE->Center.y())<VCE->Y()-VCE->Center.y() && abs(Fortune::dist(DP.x(),DP.y(),VCE->Center.x(),VCE->Center.y())-(VCE->Y()-VCE->Center.y()))>1e-10)
					VCE->Valid = false;
			}
		}
	}
	return VG;
}




#endif /* VORONOI_H_ */
