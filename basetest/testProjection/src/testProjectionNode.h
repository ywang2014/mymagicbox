//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////////////////
//
// quadricShape.cpp
//
// Description:
//    Registers a new type of shape with maya called "quadricShape".
//    This shape will display spheres, cylinders, disks, and partial disks
//    using the OpenGL gluQuadric functions.
//
//    There are no output attributes for this shape.
//    The following input attributes define the type of shape to draw.
//
//       shapeType  : 0=cylinder, 1=disk, 2=partialDisk, 3=sphere
//       radius1	: cylinder base radius, disk inner radius, sphere radius
//       radius2	: cylinder top radius, disk outer radius
//       height		: cylinder height
//       startAngle	: partial disk start angle
//       sweepAngle	: partial disk sweep angle
//       slices		: cylinder, disk, sphere slices
//       loops		: disk loops
//       stacks		: cylinder, sphere stacks
//
////////////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MPoint.h>
#include <maya/MPlug.h>
#include <maya/MDrawData.h>
#include <maya/MDrawRequest.h>
#include <maya/MSelectionMask.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MMaterial.h>
#if defined(OSMac_MachO_)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

/////////////////////////////////////////////////////////////////////
//
// Geometry class
//
class quadricGeom
{
public:
	double radius1;
	double radius2;
	double height;
	double startAngle;
	double sweepAngle;
	short slices;
	short loops;
	short stacks;
    short shapeType;
};

/////////////////////////////////////////////////////////////////////
//
// Shape class - defines the non-UI part of a shape node
//
class quadricShape : public MPxSurfaceShape
{
public:
	quadricShape();
	virtual ~quadricShape();

	virtual void			postConstructor();
	virtual MStatus			compute( const MPlug&, MDataBlock& );
    virtual bool			getInternalValue( const MPlug&,
											  MDataHandle& );
    virtual bool			setInternalValue( const MPlug&,
											  const MDataHandle& );

	virtual bool            isBounded() const;
	virtual MBoundingBox    boundingBox() const;

	static  void *		creator();
	static  MStatus		initialize();
	quadricGeom*		geometry();

private:
	quadricGeom*		fGeometry;

	// Attributes
	//
    static  MObject     shapeType;
	static	MObject		radius1;
	static	MObject		radius2;
	static	MObject		height;
	static	MObject		startAngle;
	static	MObject		sweepAngle;
	static	MObject		slices;
	static	MObject		loops;
	static	MObject		stacks;

public:
	// Shape type id
	//
	static	MTypeId		id;
};

/////////////////////////////////////////////////////////////////////
//
// UI class	- defines the UI part of a shape node
//
class quadricShapeUI : public MPxSurfaceShapeUI
{
public:
	quadricShapeUI();
	virtual ~quadricShapeUI();

	virtual void	getDrawRequests( const MDrawInfo & info,
									 bool objectAndActiveOnly,
									 MDrawRequestQueue & requests );
	virtual void	draw( const MDrawRequest & request,
						  M3dView & view ) const;
	virtual bool	select( MSelectInfo &selectInfo,
							MSelectionList &selectionList,
							MPointArray &worldSpaceSelectPts ) const;

	void			getDrawRequestsWireframe( MDrawRequest&,
											  const MDrawInfo& );
	void			getDrawRequestsShaded(	  MDrawRequest&,
											  const MDrawInfo&,
											  MDrawRequestQueue&,
											  MDrawData& data );

	static  void *  creator();

private:
	enum {
		kDrawCylinder,
		kDrawDisk,
		kDrawPartialDisk,
		kDrawSphere
	};

	// Draw Tokens
	//
	enum {
		kDrawWireframe,
		kDrawWireframeOnShaded,
		kDrawSmoothShaded,
		kDrawFlatShaded,
		kLastToken
	};
};