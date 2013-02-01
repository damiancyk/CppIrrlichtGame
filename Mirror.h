
#ifndef __CMIRROR_H_INCLUDED__
#define __CMIRROR_H_INCLUDED__

class cMirror: public scene::ISceneNode
    {
        /*
        This cMirro class is a reconstruction of Panther's version of cMirror.
        It allows for near-perfect reflections, with the exception of extreme angles.
        As the reflection is caculated for the center of the mirror, when the camera
        is close to the mirror, it distorts the rest of the image at odd or extreme
        angles. It also comes with an option for an overlay material for cracks,
        dirt, or to make it a semi-reflective surface such as marble. This class
        is completely self sufficient, to use, just create your mirror object,
        apply transformations or animators, and just before your main render in
        the main loop, use [mirror name]->reflect(); cMirror uses the default
        camera instead of an object, since reflection must be made to players
        point of view.
        */
    private:
        core::aabbox3d<f32> Box;
        scene::ISceneNode * cube;
        video::SMaterial reflectMaterial;
        video::ITexture * reflectTexture;
        scene::ICameraSceneNode * reflectCamera;
        scene::ICameraSceneNode * defaultCamera;
        f32 facos[360]; //Fast acos lookup table.
        f32 fsin[360]; //Fast sin lookup table.
        f32 w, h; //hieght and width of the mirror.
        bool hasExtraTexture;
        vector3df vUp, mirrorNormal;

    public:
        video::S3DVertex Vertices[4];

        ~cMirror()
        {
            free ( reflectTexture );
        }

        cMirror ( scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IrrlichtDevice* myDevice, scene::ICameraSceneNode* Camera, video::ITexture * mirrorOverlay = 0 ) : scene::ISceneNode ( parent, mgr, id )
        {
            defaultCamera = Camera;
            w = 25;
            h = 25;
            video::IVideoDriver* driver = myDevice->getVideoDriver();

            if ( driver->queryFeature ( video::EVDF_RENDER_TO_TARGET ) ) //If the driver can handle Render-To-Target, then...
                {

                    reflectTexture = driver->addRenderTargetTexture ( core::dimension2d<u32> ( 1024, 1024 ), "rt", ECF_UNKNOWN );
                    this->setMaterialTexture ( 0, reflectTexture ); // set material of mirror to render target and settings
                    this->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                    this->setMaterialFlag ( video::EMF_LIGHTING, false );
                    this->setMaterialFlag ( video::EMF_WIREFRAME, false );

                    if ( mirrorOverlay != 0 ) // If the mirror has an extra texture, apply it.
                        {
                            hasExtraTexture = true;
                            cube = SceneManager->addCubeSceneNode ( 50 );
                            cube->setMaterialTexture ( 0, mirrorOverlay );
                            cube->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL );
                            cube->setRotation ( getRotation() );
                            cube->setScale ( vector3df ( getScale().X, getScale().Y, 0.01f ) );
                            cube->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                            cube->setMaterialFlag ( video::EMF_LIGHTING, false );
                        }

                    else
                        hasExtraTexture = false;

                    this->setMaterialType ( video::EMT_SOLID );

                    reflectCamera = SceneManager->addCameraSceneNode ( 0, this->getPosition(),  // add dynamic camera..
                                    defaultCamera->getPosition() ); //Looking at the camera. This alows you to align the near-

                    //view plane directly with the mirror plane.
                }

            else
                {
                    //TODO: add error message
                }

            if ( driver->getDriverType() == video::EDT_OPENGL )
                {
                    //EDT_OPENGL requires we flip the image vertically
                    Vertices[0] = video::S3DVertex ( -h, w, 0, 1, 1, 0, video::SColor ( 255, 255, 255, 255 ), 0, 1 );
                    Vertices[1] = video::S3DVertex ( -h, -w, 0, 1, 0, 0, video::SColor ( 255, 255, 255, 255 ), 0, 0 );
                    Vertices[2] = video::S3DVertex ( h, w, 0, 0, 1, 1, video::SColor ( 255, 255, 255, 255 ), 1, 1 );
                    Vertices[3] = video::S3DVertex ( h, -w, 0, 0, 0, 1, video::SColor ( 255, 255, 255, 255 ), 1, 0 );
                }

            else
                {
                    //For other drivers don't need to flip it
                    Vertices[0] = video::S3DVertex ( -h, -w, 0, 1, 1, 0, video::SColor ( 255, 255, 255, 255 ), 1, 1 );
                    Vertices[1] = video::S3DVertex ( -h, w, 0, 1, 0, 0, video::SColor ( 255, 255, 255, 255 ), 1, 0 );
                    Vertices[2] = video::S3DVertex ( h, -w, 0, 0, 1, 1, video::SColor ( 255, 255, 255, 255 ), 0, 1 );
                    Vertices[3] = video::S3DVertex ( h, w, 0, 0, 0, 1, video::SColor ( 255, 255, 255, 255 ), 0, 0 );
                }

            Box.reset ( Vertices[0].Pos );

            for ( s32 i = 1; i < 4; ++i )
                Box.addInternalPoint ( Vertices[i].Pos );

            //C++ computation of cos/sin/tan can be slow, so precompute values and
            //create a lookup table.
            for ( int i = 0; i < 360; i++ )
                {
                    fsin[i] = ( f32 ) sin ( ( float ) i );
                    facos[i] = ( f32 ) acos ( ( float ) i );
                }

            vUp = vector3df ( 0, 1, 0 );

            mirrorNormal = vector3df ( 0, 0, -1 );
            vUp.rotateYZBy ( getRotation().X, core::vector3df ( 0, 0, 0 ) );
            vUp.rotateXZBy ( -getRotation().Y, core::vector3df ( 0, 0, 0 ) );
            vUp.rotateXYBy ( getRotation().Z, core::vector3df ( 0, 0, 0 ) );
            mirrorNormal.rotateYZBy ( -getRotation().X, core::vector3df ( 0, 0, 0 ) );
            mirrorNormal.rotateXZBy ( getRotation().Y, core::vector3df ( 0, 0, 0 ) );
            mirrorNormal.rotateXYBy ( getRotation().Z, core::vector3df ( 0, 0, 0 ) );
        }

        f32 acos_f ( f32 deg )
        {
            // Fast computation of acos
            deg = ( f32 ) fmodf ( deg, 360 );

            if ( deg < 0 )
                deg += 360;

            s32 deg_int = ( s32 ) deg;

            f32 deg_frac = deg - deg_int;

            return ( facos[deg_int] + deg_frac* ( facos[deg_int+1] - facos[deg_int] ) );
        }

        f32 sin_f ( f32 deg )
        {
            // Fast computation of sin
            deg = ( f32 ) fmodf ( deg, 360 );

            if ( deg < 0 )
                deg += 360;

            s32 deg_int = ( s32 ) deg;

            f32 deg_frac = deg - deg_int;

            return ( fsin[deg_int] + deg_frac* ( fsin[deg_int+1] - fsin[deg_int] ) );
        }

        virtual void render()
        {

            u16 indices[] = {       0, 2, 3, 2, 1, 3, 1, 0, 3, 2, 0, 1      };
            video::IVideoDriver* driver = SceneManager->getVideoDriver();

            driver->setMaterial ( reflectMaterial );
            driver->setTransform ( video::ETS_WORLD, AbsoluteTransformation );
            driver->drawIndexedTriangleList ( &Vertices[0], 4, &indices[0], 4 );

        }

        virtual const core::aabbox3d<f32>& getBoundingBox() const
            {
                return Box;
            }

        virtual u32 getMaterialCount() const
            {
                return 1;
            }

        virtual void OnRegisterSceneNode()
        {
            if ( IsVisible )
                SceneManager->registerNodeForRendering ( this );

            ISceneNode::OnRegisterSceneNode();
        }

        virtual video::SMaterial& getMaterial ( u32 i )
        {
            return reflectMaterial;
        }

        //Basic math functions, inlined because the call to the functions
        //take as much time as the functions themselves!
        inline irr::f32 len ( core::vector3df v )  //vector length
        {
            return ( irr::f32 ) sqrt ( ( v.X * v.X ) + ( v.Y * v.Y ) + ( v.Z * v.Z ) );
        }

        inline irr::f32 len2 ( core::vector3df v, core::vector3df v2 )  //Distance between
        {                                             //two vectors
            f32 dx = v.X - v2.X;
            f32 dy = v.Y - v2.Y;
            f32 dz = v.Z - v2.Z;
            return ( irr::f32 ) sqrt ( ( dx * dx ) + ( dy * dy ) + ( dz * dz ) );
        }


        inline irr::f32 acosd ( irr::f32 input ) //arc-cosine in degrees, uses f32 type (instead of "double")
        {                                     //Modified to use Fast acos instead of acos. Minute error rating
            //with a hit of speed.
            f32 value = acos_f ( input ) * 180.0f / 3.14159265358979f; //Converts from radians back to degrees
            return value;
        }

        inline irr::f32 angle ( core::vector3df v1, core::vector3df v2 ) //angle between two vectors
        {
            if ( ( len ( v1 ) == 0 ) || ( len ( v2 ) == 0 ) )
                {
                    return 0; //avoid division by zero!
                }

            else
                {
                    return acosd ( v1.dotProduct ( v2 ) / ( len ( v1 ) * len ( v2 ) ) );
                }
        }

        inline virtual void setRotation ( const core::vector3df &newpos )
        {
            if ( hasExtraTexture )
                cube->setRotation ( newpos );

            ISceneNode::setRotation ( newpos );

            vector3df rotate = getRotation();

            vUp = vector3df ( 0, -1, 0 );

            core::matrix4 m;

            m.setRotationDegrees ( rotate );

            m.rotateVect ( vUp );

            // Get a vector on the surface of the mirror
            vector3df mirrorV1 = Vertices[0].Pos - Vertices[3].Pos;

            mirrorV1.rotateYZBy ( getRotation().X, core::vector3df ( 0, 0, 0 ) );

            mirrorV1.rotateXZBy ( -getRotation().Y, core::vector3df ( 0, 0, 0 ) );

            mirrorV1.rotateXYBy ( getRotation().Z, core::vector3df ( 0, 0, 0 ) );

            // Get another vector on the surface of the mirror
            vector3df mirrorV2 = Vertices[2].Pos - Vertices[1].Pos;

            mirrorV2.rotateYZBy ( getRotation().X, core::vector3df ( 0, 0, 0 ) );

            mirrorV2.rotateXZBy ( -getRotation().Y, core::vector3df ( 0, 0, 0 ) );

            mirrorV2.rotateXYBy ( getRotation().Z, core::vector3df ( 0, 0, 0 ) );

            // Use the above 2 vectors to find the mirror's Normal
            mirrorNormal = mirrorV1.crossProduct ( mirrorV2 );

            // Normalize the Normal (to a length of 1).
            mirrorNormal = mirrorNormal.normalize();

        }

        virtual void setPosition ( const core::vector3df &v )
        {
            if ( hasExtraTexture )
                cube->setPosition ( v );

            ISceneNode::setPosition ( v );
        }

        virtual void setScale ( const core::vector3df &v )
        {
            if ( hasExtraTexture )
                cube->setScale ( vector3df ( v.X, v.Y, 0.01f ) );

            ISceneNode::setScale ( v );
        }

        void rotateVectorAroundAxis ( vector3df & vector, const vector3df & axis, f32 radians )
        {
            quaternion MrQuaternion;
            matrix4 MrMatrix;
            ( void ) MrQuaternion.fromAngleAxis ( radians, axis );
            MrQuaternion.getMatrix_transposed ( MrMatrix );
            MrMatrix.rotateVect ( vector );
        }

        void reflect() //Does everything required to reflect the image.
        {           //All put into one function to reduce funtion calls.
            f32 myAngle, dP;
            core::vector3df reflectionVector,
            sightVector;
            bool oldFrontSide;
            static bool frontSide = true;

            // Store the node's vector to the mirror
            sightVector = defaultCamera->getPosition() - getPosition();

            //check to see if this is the front of the mirror. This mirror is
            //one sided, do to the fact that in most games, mirrors will not be
            //floating in the middle of the air, but attached to another suface.
            //Also, mirror calculations can be stopped to increase FPS when you
            //dont need it.
            myAngle = angle ( sightVector, mirrorNormal );
            oldFrontSide = frontSide;

            if ( myAngle < 90 )
                frontSide = false;
            else
                frontSide = true;

            video::IVideoDriver* driver = SceneManager->getVideoDriver();

            if ( !frontSide )
                {
                    driver->setRenderTarget ( reflectTexture, true, true, video::SColor ( 0, 0, 0, 0 ) );
                    driver->setRenderTarget ( 0 );
                    return;
                }

            //plane3df mirrorplane = plane3df(vector3df(0,0,0),mirrorNormal);

            // Calculate the "dot product" - used to find the reflection
            //vector below
            dP = 2.0f * mirrorNormal.dotProduct ( sightVector );

            // Standard formula for a reflection vector!
            reflectionVector = core::vector3df ( dP, dP, dP ) * mirrorNormal - sightVector;

            //Set the lenght of the reflection vector to the length of
            //the matching sight vector. Used in camera placement.
            reflectionVector.setLength ( len ( sightVector ) );

            //This flips the image, so if the user flips the mirror vertically,
            //it still shows the ground on the bottom
            if ( getRotation().X > 90.0f )
                setRotation ( core::vector3df ( getRotation().X - 180.0f, getRotation().Y, getRotation().Z ) );

            if ( getRotation().X < -90.0f )
                setRotation ( core::vector3df ( getRotation().X + 180.0f, getRotation().Y, getRotation().Z ) );

            //Uses the mirror position and the reflection vector
            //to place the camera.

            //get half the length between the 2 cameras to find
            //the near clipping plane.
            f32 length2 = len2 ( defaultCamera->getPosition(),
                                 reflectCamera->getPosition() ) / 2;

            //create_glfrustumLH creates the view frustum from the camera to the
            //mirror (remeber reflection camera is actually pointing to the
            //original camera) But create_glfrustumLH requires fixing.
            vector3df f = vector3df ( 0, 1, 0 );

            // Fix for rot on Z axis
            //xfactor*=cos(getRotation().Z); //Zrot=0, then factor*1, to |Zrot|=90, factor*-1
            //yfactor*=asin(getRotation().Z);
            reflectCamera->setPosition ( getPosition() - reflectionVector );

            reflectCamera->setUpVector ( vUp );

            //Target the default camera.
            reflectCamera->setTarget ( defaultCamera->getPosition() );

            //Rotate the reflection vector to offset the rotation to the
            //reflection camera. (Note: The X axis is negitive here)
            //f.rotateXYBy(-getRotation().Z, core::vector3df(0, 0, 0));
            //vector3df rotate = getRotation();
            vector3df forward = vector3df ( 0, 0, 1 );

            vector3df up = vector3df ( 0, 1, 0 );

            vector3df right = vector3df ( 1, 0, 0 );

            rotateVectorAroundAxis ( reflectionVector, right, DEGTORAD* -getRotation().X );

            rotateVectorAroundAxis ( up, right, DEGTORAD* -getRotation().X );

            rotateVectorAroundAxis ( forward, right, DEGTORAD* -getRotation().X );

            rotateVectorAroundAxis ( reflectionVector, up, DEGTORAD* -getRotation().Y );

            rotateVectorAroundAxis ( forward, up, DEGTORAD* -getRotation().Y );

            rotateVectorAroundAxis ( reflectionVector, forward, DEGTORAD* -getRotation().Z );


            //reflectionVector.rotateYZBy(-rotate.X-(rotate.Y/.5f)*(rotate.X/45), core::vector3df(0, 0, 0));
            //reflectionVector.rotateXZBy(rotate.Y-(rotate.X/.5f)*(rotate.Y/45), core::vector3df(0, 0, 0));
            //reflectionVector.rotateXYBy(-rotate.Z, core::vector3df(0, 0, 0));

            f32 yfactor = -.5f;//*(f.Y);

            f32 xfactor = .5f;//-.5;//*(f.Y);

            //Create the view frustum. w= half the width of the mirror. For scalability,
            // times this by getScale().X. for H, multiply it by getScale().Y.
            // reflecionVector is the center of the mirror from the camera.
            reflectCamera->setProjectionMatrix ( create_glfrustumLH ( ( reflectionVector.X*xfactor -
                                                 ( w / 2*getScale().X ) ), ( reflectionVector.X*xfactor + ( w / 2*getScale().X ) )
                                                 , reflectionVector.Y*yfactor - ( h / 2*getScale().Y ),
                                                 reflectionVector.Y*yfactor + ( h / 2*getScale().Y ), length2 / 2,
                                                 defaultCamera->getFarValue() ) );

            //Set the up vector for the camera.
            if ( hasExtraTexture )
                cube->setVisible ( false );

            //render the image onto the mirror.
            driver->setRenderTarget ( reflectTexture, true, true, video::SColor ( 0, 0, 0, 255 ) ); // set render target texture (the mirror's texture)

            this->setVisible ( false ); // make mirror invisible (so it doesn't get painted over)

            SceneManager->setActiveCamera ( reflectCamera ); // set fixed camera as active camera

            SceneManager->drawAll(); // draw whole scene into render buffer

            if ( hasExtraTexture )
                cube->setVisible ( true );

            //Reset everything
            driver->setRenderTarget ( 0 ); // set back old render target

            this->setVisible ( true ); // make the mirror visible again

            SceneManager->setActiveCamera ( defaultCamera );
        }

        // function used to create the view matrix. Provided by FreakNigh.
        matrix4 create_glfrustumLH ( f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ )
        {
            f32 x = ( 2.0F * nearZ ) / ( right - left );
            f32 y = ( 2.0F * nearZ ) / ( top - bottom );
            f32 a = ( right + left ) / ( right - left );
            f32 b = ( top + bottom ) / ( top - bottom );
            f32 c = - ( farZ + nearZ ) / ( farZ - nearZ );
            f32 d = - ( 2.0F * farZ * nearZ ) / ( farZ - nearZ );

            matrix4 m;
            m.makeIdentity();
#define M(row,col)  m[col*4+row]
            M ( 0, 0 ) = x;     M ( 0, 1 ) = 0.0F;  M ( 0, 2 ) = a;      M ( 0, 3 ) = 0.0F;
            M ( 1, 0 ) = 0.0F;  M ( 1, 1 ) = y;     M ( 1, 2 ) = b;      M ( 1, 3 ) = 0.0F;
            M ( 2, 0 ) = 0.0F;  M ( 2, 1 ) = 0.0F;  M ( 2, 2 ) = -c;      M ( 2, 3 ) = d;
            M ( 3, 0 ) = 0.0F;  M ( 3, 1 ) = 0.0F;  M ( 3, 2 ) = 1.0F;  M ( 3, 3 ) = 0.0F;
#undef M

            return m;
        }

    };

#endif

