#include "CubicImages.h"
#include <kvs/OpenMP>
#include <kvs/Assert>
#include <kvs/Camera>
#include <kvs/Light>


namespace
{

inline void Draw(
    const kvs::ColorImage& source,
    const size_t offset_i,
    const size_t offset_j,
    kvs::ColorImage& result )
{
    const size_t w = source.width();
    const size_t h = source.height();
    for ( size_t j = 0; j < h; j++ )
    {
        for ( size_t i = 0; i < w; i++ )
        {
            const kvs::RGBColor pixel = source.pixel( i, j );
            result.setPixel( i + offset_i, j + offset_j, pixel );
        }
    }
}

inline kvs::RGBColor Interp(
    const kvs::ColorImage& image,
    const float x,
    const float y )
{
    KVS_ASSERT( 0.0f <= x && x <= image.width() - 1.0f );
    KVS_ASSERT( 0.0f <= y && y <= image.height() - 1.0f );

    const size_t x0 = kvs::Math::Floor(x);
    const size_t y0 = kvs::Math::Floor(y);
    const size_t x1 = x0 + ( image.width() - 1 > x0 ? 1 : 0 );
    const size_t y1 = y0 + ( image.height() - 1 > y0 ? 1 : 0 );
    const float xratio = x - x0;
    const float yratio = y - y0;

    const kvs::Vec3 p0 = image.pixel( x0, y0 ).toVec3();
    const kvs::Vec3 p1 = image.pixel( x0, y1 ).toVec3();
    const kvs::Vec3 p2 = image.pixel( x1, y0 ).toVec3();
    const kvs::Vec3 p3 = image.pixel( x1, y1 ).toVec3();

    const kvs::Vec3 d = p0 * ( 1.0f - xratio ) + p2 * xratio;
    const kvs::Vec3 e = p1 * ( 1.0f - xratio ) + p3 * xratio;
    return kvs::RGBColor( d * ( 1.0f - yratio ) + e * yratio );
}

}


namespace kvs
{

namespace osmesa
{

const kvs::ColorImage CubicImages::cubeMapImage() const
{
    const size_t w = this->frontImage().width();
    const size_t h = this->frontImage().height();

    kvs::ColorImage result( w * 4, h * 3 );
    ::Draw( this->topImage(), w, 0, result );
    ::Draw( this->leftImage(), 0, h, result );
    ::Draw( this->frontImage(), w, h, result );
    ::Draw( this->rightImage(), w * 2, h, result );
    ::Draw( this->backImage(), w * 3, h, result );
    ::Draw( this->bottomImage(), w, h * 2, result );

    return result;
}

const kvs::ColorImage CubicImages::sphericalMapImage() const
{
    const size_t w = this->frontImage().width();
    const size_t h = this->frontImage().height();

    kvs::ColorImage result( w * 4, h * 3 );
    KVS_OMP_PARALLEL_FOR( schedule(dynamic) )
    for ( size_t j = 0; j < result.height(); j++ )
    {
        const float v = 1.0f - (float)j / ( result.height() - 1 );
        const float theta = v * kvs::Math::pi;
        for ( size_t i = 0; i < result.width(); i++ )
        {
            const float u = (float)i / ( result.width() - 1 );
            const float phi = u * 2.0f * kvs::Math::pi;

            const float x = std::sin( phi ) * std::sin( theta ) * -1.0f;
            const float y = std::cos( theta );
            const float z = std::cos( phi ) * std::sin( theta ) * -1.0f;

            const float a = kvs::Math::Max(
                kvs::Math::Abs( x ), kvs::Math::Abs( y ), kvs::Math::Abs( z ) );
            const float xa = x / a;
            const float ya = y / a;
            const float za = z / a;

            kvs::RGBColor pixel;
            if ( xa == 1 )
            {
                const float si = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f - 1.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( this->rightImage(), si, sj );
            }
            else if ( xa == -1 )
            {
                const float si = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( this->leftImage(), si, sj );
            }
            else if ( ya == 1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f - 1.0f ) * ( h - 1 ) );
                pixel = ::Interp( this->bottomImage(), si, sj );
            }
            else if ( ya == -1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( this->topImage(), si, sj );
            }
            else if ( za == 1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( this->frontImage(), si, sj );
            }
            else if ( za == -1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f - 1.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::Interp( this->backImage(), si, sj );
            }
            result.setPixel( i, j, pixel );
        }
    }

    return result;
}

void CubicImages::draw( kvs::osmesa::Screen& screen, const kvs::Vec3 p )
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

void CubicImages::write( const std::string& basename )
{
    m_images[Front].write( basename + "_front.bmp" );
    m_images[Back].write( basename + "_back.bmp" );
    m_images[Left].write( basename + "_left.bmp" );
    m_images[Right].write( basename + "_right.bmp" );
    m_images[Top].write( basename + "_top.bmp" );
    m_images[Bottom].write( basename + "_bottom.bmp" );
}

kvs::ColorImage CubicImages::capture(
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

} // end of namespace osmesa

} // end of namespace kvs
