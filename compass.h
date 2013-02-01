#include <irrlicht.h>
using namespace irr;

#ifdef _MSC_VER
#  pragma comment(lib, "Irrlicht.lib")
#endif

class CGUICompass : public gui::IGUIElement
{
public:
    CGUICompass(core::rect<s32> rect, gui::IGUIEnvironment* env, gui::IGUIElement* parent, u32 faces)
        : gui::IGUIElement(gui::EGUIET_ELEMENT, env, parent, -1, rect)
        , OffsetXY(.5f, .5f), ScaleXY(1.f, 1.f), RotateZ(0.f)
    {
        // require at least 3 faces
        if (faces < 3)
            faces = 3;

        // make a triangle fan that looks like a disk
        Mesh = new scene::SMeshBuffer;

        Mesh->Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
//        Mesh->Material.TextureLayer[0].TextureWrap = video::ETC_CLAMP_TO_BORDER;
        Mesh->Material.Lighting = false;

        const video::SColor center (255, 255, 255, 255);
        const video::SColor edges  (1, 255, 255, 255);

        //
        // build up the vertex buffer. note that we use identity matrices
        // for world, view and projection, so all vertex positions have to
        // be in range -1 to 1
        //

        // the center of the disk
        const video::S3DVertex v0(0, 0, 0, 0, 0, -1, center, .5f, .5f);
        Mesh->Vertices.push_back(v0);

        const u32 nfaces = faces + 1;
        for (u32 i = 1; i < nfaces; ++i) {

            const f32 rad = core::PI * 2.f * i / faces;

            // range from -1 to 1
            const f32 x = sinf (rad);
            const f32 y = cosf (rad);

            // range from 0 to 1
            const f32 tu = (x * .5f) + .5f;
            const f32 tv = (y * .5f) + .5f;

            const video::S3DVertex v (x, y, 0, 0, 0, -1, edges, tu, tv);
            Mesh->Vertices.push_back (v);
        }

        //
        // build up the index buffer
        //

        // triangle fan
        //Mesh->Indices.push_back(0);
        //for (u32 j = 1; j < nfaces; ++j)
        //{
        //    Mesh->Indices.push_back (j);
        //}
        //Mesh->Indices.push_back(1);

        // triangle list
        for (u32 j = 0; j < faces; ++j)
        {
            Mesh->Indices.push_back (0);
            Mesh->Indices.push_back (j + 1);
            Mesh->Indices.push_back (j + 2);
        }
        Mesh->Indices [Mesh->Indices.size() - 1] = 1;

        // update the matrix to use initial values
        _updateMatrix ();
    }

    virtual ~CGUICompass()
    {
        if (Mesh)
            Mesh->drop();
    }

    f32 deg_to_rad (f32 deg)
{
    return (deg * 3.14159265359f) / 180.f;
}

f32 rad_to_deg (f32 rad)
{
    return rad * (180.f / 3.14159265359f);
}

    void setHeading(f32 deg)
    {
        RotateZ = deg;
        _updateMatrix ();
    }

    f32 getHeading () const
    {
        return RotateZ;
    }

    void setScale (f32 x, f32 y)
    {
        ScaleXY.set (x, y);
        _updateMatrix ();
    }

    core::vector2df getScale () const
    {
        return ScaleXY;
    }

    void setTranslation(f32 x, f32 y)
    {
        OffsetXY.set (x, y);
        _updateMatrix ();
    }

    core::vector2df getTranslation () const
    {
        return OffsetXY;
    }

    void setTexture (video::ITexture* newTex)
    {
        Mesh->Material.setTexture(0, newTex);
    }

    //! render the compass
    virtual void draw()
    {
        video::IVideoDriver* driver = Environment->getVideoDriver();

        if (! (driver && IsVisible))
            return;

        //driver->clearZBuffer();

        core::rect<s32> oldViewPort = driver->getViewPort();
        driver->setViewPort(getAbsolutePosition());

        static const core::matrix4 ident (core::matrix4::EM4CONST_IDENTITY);

        // clear the projection matrix
        core::matrix4 oldProjMat = driver->getTransform(video::ETS_PROJECTION);
        driver->setTransform(video::ETS_PROJECTION, ident);

        // clear the view matrix
        core::matrix4 oldViewMat = driver->getTransform(video::ETS_VIEW);
        driver->setTransform(video::ETS_VIEW, ident);

        // clear the world matrix
        driver->setTransform(video::ETS_WORLD, ident);

        // set the material and render
       driver->setMaterial(Mesh->getMaterial());
        //driver->drawVertexPrimitiveList(Mesh->getVertices(), Mesh->getVertexCount(),
        //                                Mesh->getIndices(), Mesh->getIndexCount() - 2,
        //                                video::EVT_STANDARD, scene::EPT_TRIANGLE_FAN);
        driver->drawMeshBuffer(Mesh);

        // restore view matrix
        driver->setTransform(video::ETS_VIEW, oldViewMat);

        // restore projection matrix
        driver->setTransform(video::ETS_PROJECTION, oldProjMat);

        // restore the view area
        driver->setViewPort(oldViewPort);
    }

private:

    void _updateMatrix ()
    {
        core::matrix4 offsetXY;
        offsetXY.setTextureTranslate(OffsetXY.X - .5f, OffsetXY.Y - .5f);

        core::matrix4 rotateZ;
        rotateZ.setTextureRotationCenter(-1.f * deg_to_rad(RotateZ));

        core::matrix4 scaleXY;
        scaleXY.setTextureScaleCenter(1.f / ScaleXY.X, 1.f / ScaleXY.Y);

        offsetXY *= rotateZ; // offset, then rotate
        offsetXY *= scaleXY; // then scale

        Mesh->getMaterial().setTextureMatrix(0, offsetXY);
    }

private:

    scene::SMeshBuffer* Mesh;
    core::vector2df OffsetXY;
    core::vector2df ScaleXY;
    f32 RotateZ;
};
