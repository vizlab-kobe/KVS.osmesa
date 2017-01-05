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
    kvs::ColorImage m_front;
    kvs::ColorImage m_back;
    kvs::ColorImage m_left;
    kvs::ColorImage m_right;
    kvs::ColorImage m_top;
    kvs::ColorImage m_bottom;

public:
    CubicImages(){}

    const kvs::ColorImage& front() const { return m_front; }
    const kvs::ColorImage& back() const { return m_back; }
    const kvs::ColorImage& left() const { return m_left; }
    const kvs::ColorImage& right() const { return m_right; }
    const kvs::ColorImage& top() const { return m_top; }
    const kvs::ColorImage& bottom() const { return m_bottom; }

    void draw( kvs::osmesa::Screen& screen, const kvs::Vec3 p )
    {
        const float F = screen.scene()->camera()->fieldOfView();
//        const kvs::Vec3 T = P - screen.scene()->camera()->position();

        const kvs::Vec3 dir[6] = {
            kvs::Vec3(  0,  0, -1 ), // front
            kvs::Vec3(  0,  0, +1 ), // back
            kvs::Vec3( -1,  0,  0 ), // left
            kvs::Vec3( +1,  0,  0 ), // right
            kvs::Vec3(  0, +1,  0 ), // top
            kvs::Vec3(  0, -1,  0 )  // bottom
        };

        const kvs::Vec3 up[6] = {
            kvs::Vec3( 0, 1,  0 ), // front
            kvs::Vec3( 0, 1,  0 ), // back
            kvs::Vec3( 0, 1,  0 ), // left
            kvs::Vec3( 0, 1,  0 ), // right
            kvs::Vec3( 0, 0,  1 ), // top
            kvs::Vec3( 0, 0, -1 )  // bottom
        };

        screen.scene()->camera()->setFieldOfView( 90.0 );
        screen.scene()->camera()->setFront( 0.1 );
//        screen.scene()->camera()->multiplyXform( kvs::Xform::Translation( T ) );
        {
            m_front = this->capture( screen, p, p + dir[0], up[0] );
            m_back = this->capture( screen, p, p + dir[1], up[1] );
            m_left = this->capture( screen, p, p + dir[2], up[2] );
            m_right = this->capture( screen, p, p + dir[3], up[3] );
            m_top = this->capture( screen, p, p + dir[4], up[4] );
            m_bottom = this->capture( screen, p, p + dir[5], up[5] );
/*
            m_front = this->capture( screen, kvs::Mat3::RotationY( 0.0 ) ); // -z
            m_back = this->capture( screen, kvs::Mat3::RotationY( 180.0 ) ); // +z
            m_left = this->capture( screen, kvs::Mat3::RotationY( 90.0 ) ); // -x
            m_right = this->capture( screen, kvs::Mat3::RotationY( -90.0 ) ); // +x
            m_top = this->capture( screen, kvs::Mat3::RotationX( 90.0 ) ); // +y
            m_bottom = this->capture( screen, kvs::Mat3::RotationX( -90.0 ) ); // -y
*/
        }
        screen.scene()->camera()->setFieldOfView( F );
//        screen.scene()->camera()->multiplyXform( kvs::Xform::Translation( -T ) );
    }

    void write( const std::string& basename )
    {
        m_front.write( basename + "_front.bmp" );
        m_back.write( basename + "_back.bmp" );
        m_left.write( basename + "_left.bmp" );
        m_right.write( basename + "_right.bmp" );
        m_top.write( basename + "_top.bmp" );
        m_bottom.write( basename + "_bottom.bmp" );
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

    kvs::ColorImage capture( kvs::osmesa::Screen& screen, const kvs::Mat3 R )
    {
        screen.scene()->camera()->multiplyXform( kvs::Xform::Rotation( R ) );
        screen.draw();
        kvs::ColorImage image = screen.capture();
        screen.scene()->camera()->multiplyXform( kvs::Xform::Rotation( R.inverted() ) );
        return image;
    }
};
