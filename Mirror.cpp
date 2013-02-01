// Mirror.cpp - version 2 - This is a simple example that uses the Irrlicht 3D Engine to create a mirror effect 
#define _CRT_SECURE_NO_DEPRECATE 1 //suppress stupid swprintf warning! (for VC++ 2005) 
#include <stdio.h> 
#include <wchar.h> 
#include <irrlicht.h> 
#include <windows.h> 
#include <stdlib.h> 
#include <math.h> 
#include "irrMath.h" 
#include "irrTypes.h" 
using namespace irr; 
#pragma comment(lib, "Irrlicht.lib") 

scene::ISceneNode* cube = 0; 
scene::ISceneNode* box = 0; 
scene::IAnimatedMeshSceneNode* dwarf = 0; 
scene::ICameraSceneNode* camera = 0; 
IrrlichtDevice* device = 0; 
bool quit; 
core::vector3df myReflectionVector, cameraPosition; 
scene::ICameraSceneNode* fixedCam = 0; 

class CMirror : public scene::ISceneNode 
{ 
   core::aabbox3d<f32> Box; 
   video::SMaterial Material; 

public: 
   video::S3DVertex Vertices[4]; 

      CMirror(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IrrlichtDevice* myDevice) 
      : scene::ISceneNode(parent, mgr, id) 
   { 
      Material.Wireframe = false; 
      Material.Lighting = false; 

      if (myDevice->getVideoDriver()->getDriverType() == video::EDT_OPENGL) 
      { 
         //EDT_OPENGL requires we flip the image vertically 
         Vertices[0] = video::S3DVertex(-70,70,0, 1,1,0, video::SColor(255,255,255,255), 0, 1); 
         Vertices[1] = video::S3DVertex(-70,-70,0, 1,0,0, video::SColor(255,255,255,255), 0, 0); 
         Vertices[2] = video::S3DVertex(70,70,0, 0,1,1, video::SColor(255,255,255,255), 1, 1); 
         Vertices[3] = video::S3DVertex(70,-70,0, 0,0,1, video::SColor(255,255,255,255), 1, 0); 
      } 
      else 
      { 
         //For other drivers don't need to flip it 
         Vertices[0] = video::S3DVertex(-70,-70,0, 1,1,0, video::SColor(255,255,255,255), 1, 1); 
         Vertices[1] = video::S3DVertex(-70,70,0, 1,0,0, video::SColor(255,255,255,255), 1, 0); 
         Vertices[2] = video::S3DVertex(70,-70,0, 0,1,1, video::SColor(255,255,255,255), 0, 1); 
         Vertices[3] = video::S3DVertex(70,70,0, 0,0,1, video::SColor(255,255,255,255), 0, 0); 
      } 

      Box.reset(Vertices[0].Pos); 
      for (s32 i=1; i<4; ++i) 
         Box.addInternalPoint(Vertices[i].Pos); 
   } 

   virtual void OnPreRender() 
   { 
      if (IsVisible) 
         SceneManager->registerNodeForRendering(this); 

      ISceneNode::OnPreRender(); 
   } 

   virtual void render() 
   { 
      u16 indices[] = {   0,2,3, 2,1,3, 1,0,3, 2,0,1   }; 
      video::IVideoDriver* driver = SceneManager->getVideoDriver(); 

      driver->setMaterial(Material); 
      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation); 
      driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 4); 
   } 

   virtual const core::aabbox3d<f32>& getBoundingBox() const 
   { 
      return Box; 
   } 

   virtual s32 getMaterialCount() 
   { 
      return 1; 
   } 

   virtual video::SMaterial& getMaterial(s32 i) 
   { 
      return Material; 
   } 

   virtual irr::f32 acosd(irr::f32 input)  //arc-cosine in degrees, uses f32 type (instead of "double") 
   { 
      double value = acos((double)input) * 180.0 / 3.14159265358979;  //Converts from radians back to degrees 
      return (irr::f32)value; 
   } 

   virtual irr::f32 len(core::vector3df v)  //vector length 
   { 
      return (irr::f32)sqrt((v.X * v.X) + (v.Y * v.Y) + (v.Z * v.Z)); 
   } 

   virtual irr::f32 angle(core::vector3df v1, core::vector3df v2)  //angle between two vectors 
   { 
      if ((len(v1) == 0) || (len(v2) == 0)) 
      { 
         return 0;  //avoid division by zero! 
      } else { 
         return (irr::f32)acosd(v1.dotProduct(v2) / (len(v1) * len(v2))); 
      } 
   } 

   virtual void setRotationUp(core::vector3df rotationVector, scene::ICameraSceneNode* renderCamera) 
   { 
      // If you just use setRotation(), then the whole image rotates (real mirrors don't do this)! 
      // If you were rotating a television, the whole image would rotate.  But not a mirror. 
      // Unfortunately, this does cause some "jitter" in the image (not sure why - looks like it is being drawn twice). 
      core::vector3df upV; 
      upV = core::vector3df(0, 1, 0); //fixedCam->getUpVector(); 
      upV.rotateYZBy(rotationVector.X, core::vector3df(0, 0, 0)); 
      upV.rotateXZBy(-rotationVector.Y, core::vector3df(0, 0, 0)); 
      upV.rotateXYBy(rotationVector.Z, core::vector3df(0, 0, 0)); 
      renderCamera->setUpVector(upV); 
      setRotation(rotationVector); 
   } 

   virtual core::vector3df getMirrorReflection(scene::ISceneNode* nodeLookingAtMirror, scene::ICameraSceneNode* renderCamera) 
   { 
      f32 myAngle, dP, tempX, tempY; 
      core::vector3df reflectionVector, mirrorV1, mirrorV2, mirrorNormal, dwarfVector, delVector; 
      bool oldFrontSide; 
      static bool frontSide = true; 

      dwarfVector = nodeLookingAtMirror->getPosition() - getPosition(); // Store the node's vector to the mirror 

      mirrorV1 = Vertices[0].Pos - Vertices[3].Pos; // Get a vector on the surface of the mirror 
      mirrorV1.rotateYZBy(getRotation().X, core::vector3df(0, 0, 0)); 
      mirrorV1.rotateXZBy(-getRotation().Y, core::vector3df(0, 0, 0)); 
      mirrorV1.rotateXYBy(getRotation().Z, core::vector3df(0, 0, 0)); 

      mirrorV2 = Vertices[2].Pos - Vertices[1].Pos; // Get another vector on the surface of the mirror 
      mirrorV2.rotateYZBy(getRotation().X, core::vector3df(0, 0, 0)); 
      mirrorV2.rotateXZBy(-getRotation().Y, core::vector3df(0, 0, 0)); 
      mirrorV2.rotateXYBy(getRotation().Z, core::vector3df(0, 0, 0)); 

      mirrorNormal = mirrorV1.crossProduct(mirrorV2); // Use the above 2 vectors to find the mirror's Normal 
      mirrorNormal = mirrorNormal.normalize(); // Normalize the Normal (to a length of 1). 
      myAngle = angle(dwarfVector, mirrorNormal); 
      dP = 2.0f * mirrorNormal.dotProduct(dwarfVector); // Calculate the "dot product" - used to find the reflection vector below 
      reflectionVector = core::vector3df(dP, dP, dP) * mirrorNormal - dwarfVector; // Standard formula for a reflection vector! 
      reflectionVector.setLength(100.0f); 

      oldFrontSide = frontSide; 
      if (myAngle < 90) 
         frontSide = false; 
      else 
         frontSide = true; 

      if (frontSide != oldFrontSide) // This flips the image horizontally, so the image is reversed, like a real mirror. 
      { 
         tempX = Vertices[0].TCoords.X; // Set the texture coordinates so the image is reversed on front side 
         tempY = Vertices[0].TCoords.Y; 
         Vertices[0].TCoords.X = Vertices[2].TCoords.X; 
         Vertices[0].TCoords.Y = Vertices[2].TCoords.Y; 
         Vertices[2].TCoords.X = tempX; 
         Vertices[2].TCoords.Y = tempY; 

         tempX = Vertices[1].TCoords.X; 
         tempY = Vertices[1].TCoords.Y; 
         Vertices[1].TCoords.X = Vertices[3].TCoords.X; 
         Vertices[1].TCoords.Y = Vertices[3].TCoords.Y; 
         Vertices[3].TCoords.X = tempX; 
         Vertices[3].TCoords.Y = tempY; 
      } 

      if (getRotation().X > 90.0f) //This flips the image, so if the user flips the mirror vertically, it still shows the ground on the bottom 
         setRotation(core::vector3df(getRotation().X - 180.0f, getRotation().Y, getRotation().Z)); 
      if (getRotation().X < -90.0f) //This flips the image, so if the user flips the mirror vertically, it still shows the ground on the bottom 
         setRotation(core::vector3df(getRotation().X + 180.0f, getRotation().Y, getRotation().Z)); 
      return reflectionVector; 
   } 
}; 

CMirror* mirror = 0; 

class MyEventReceiver : public IEventReceiver 
{ 
public: 
   virtual bool OnEvent(SEvent event) 
   { 
      core::vector3df mirVector; 

      if (cube != 0 && event.EventType == irr::EET_KEY_INPUT_EVENT&& 
         !event.KeyInput.PressedDown) 
      { 
         switch(event.KeyInput.Key) 
         { 
         case KEY_ESCAPE: //Quit the program 
            { 
            quit = true; 
            return true; 
            } 
         case KEY_COMMA: // rotate mirror left 
            { 
               mirVector = mirror->getRotation(); 
               mirVector.Y += 6.0f; //Rotate a little 
               mirror->setRotationUp(mirVector, fixedCam); 
               return true; 
            } 
         case KEY_PERIOD:  // rotate mirror right 
            { 
               mirVector = mirror->getRotation(); 
               mirVector.Y -= 6.0f; //Rotate a little 
               mirror->setRotationUp(mirVector, fixedCam); 
               return true; 
            } 
         case KEY_KEY_P:  // rotate mirror +Z 
            { 
               mirVector = mirror->getRotation(); 
               mirVector.Z += 6.0f; // Rotate a little 
               mirror->setRotationUp(mirVector, fixedCam); 
               return true; 
            } 
         case KEY_KEY_L:  // rotate mirror -Z 
            { 
               mirVector = mirror->getRotation(); 
               mirVector.Z -= 6.0f; // Rotate a little 
               mirror->setRotationUp(mirVector, fixedCam); 
               return true; 
            } 
         case KEY_KEY_O:  // rotate mirror +X 
            { 
               mirVector = mirror->getRotation(); 
               mirVector.X += 6.0f; //Rotate a little 
               mirror->setRotationUp(mirVector, fixedCam); 
               return true; 
            } 
         case KEY_KEY_K:  // rotate mirror -X 
            { 
               mirVector = mirror->getRotation(); 
               mirVector.X -= 6.0f; //Rotate a little 
               mirror->setRotationUp(mirVector, fixedCam); 
               return true; 
            } 
         } 
      } 
      return false; 
   } 
}; 
