#pragma once
#include <kvs/Vector3>
#include <kvs/Matrix33>
#include <kvs/Xform>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ColorImage>
#include <KVS.osmesa/Lib/Screen.h>


class CubicImages
{
public:
    enum Direction
    {
        Right = 0,  // +x
        Left = 1,   // -x
        Top = 2,    // +y
        Bottom = 3, // -y
        Back = 4,   // +z
        Front = 5,  // -z
        NumberOfDirections = 6
    };

private:
    kvs::ColorImage m_images[NumberOfDirections];

public:
    CubicImages(){}

    const kvs::ColorImage& image( const Direction& dir ) const { return m_images[dir]; }
    const kvs::ColorImage& rightImage() const { return this->image( Right ); }
    const kvs::ColorImage& leftImage() const { return this->image( Left ); }
    const kvs::ColorImage& topImage() const { return this->image( Top ); }
    const kvs::ColorImage& bottomImage() const { return this->image( Bottom ); }
    const kvs::ColorImage& backImage() const { return this->image( Back ); }
    const kvs::ColorImage& frontImage() const { return this->image( Front ); }

    void draw( kvs::osmesa::Screen& screen, const kvs::Vec3 p )
    {
        const kvs::Vec3 dir[NumberOfDirections] = {
            kvs::Vec3( +1,  0,  0 ), // right
            kvs::Vec3( -1,  0,  0 ), // left
            kvs::Vec3(  0, +1,  0 ), // top
            kvs::Vec3(  0, -1,  0 ), // bottom
            kvs::Vec3(  0,  0, +1 ), // back
            kvs::Vec3(  0,  0, -1 )  // front
        };

        const kvs::Vec3 up[NumberOfDirections] = {
            kvs::Vec3( 0, 1,  0 ), // right
            kvs::Vec3( 0, 1,  0 ), // left
            kvs::Vec3( 0, 0,  1 ), // top
            kvs::Vec3( 0, 0, -1 ), // bottom
            kvs::Vec3( 0, 1,  0 ), // back
            kvs::Vec3( 0, 1,  0 )  // front
        };

        const float F = screen.scene()->camera()->fieldOfView();
        screen.scene()->camera()->setFieldOfView( 90.0 );
        screen.scene()->camera()->setFront( 0.1 );
        for ( size_t i = 0; i < NumberOfDirections; i++ )
        {
            m_images[Direction(i)] = this->capture( screen, p, p + dir[i], up[i] );
        }
        screen.scene()->camera()->setFieldOfView( F );
    }

    void write( const std::string& basename )
    {
        m_images[Front].write( basename + "_front.bmp" );
        m_images[Back].write( basename + "_back.bmp" );
        m_images[Left].write( basename + "_left.bmp" );
        m_images[Right].write( basename + "_right.bmp" );
        m_images[Top].write( basename + "_top.bmp" );
        m_images[Bottom].write( basename + "_bottom.bmp" );
    }

private:
    kvs::ColorImage capture(
        kvs::osmesa::Screen& screen,
        const kvs::Vec3& position,
        const kvs::Vec3& look_at,
        const kvs::Vec3& up )
    {
        screen.scene()->light()->setPosition( position );
        screen.scene()->camera()->setPosition( position, look_at, up );
        screen.draw();
        return screen.capture();
    }
};
