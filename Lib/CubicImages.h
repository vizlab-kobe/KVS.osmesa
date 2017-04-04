#pragma once
#include <kvs/Vector3>
#include <kvs/ColorImage>
#include "Screen.h"


namespace kvs
{

namespace osmesa
{

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
    const kvs::ColorImage cubeMapImage() const;
    const kvs::ColorImage sphericalMapImage() const;
    void draw( kvs::osmesa::Screen& screen, const kvs::Vec3 p );
    void write( const std::string& basename );

private:
    kvs::ColorImage capture(
        kvs::osmesa::Screen& screen,
        const kvs::Vec3& position,
        const kvs::Vec3& look_at,
        const kvs::Vec3& up );
};

} // end of namespace osmesa

} // end of namespace kvs
