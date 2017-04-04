#pragma once
#include "CubicImages.h"
#include <kvs/Math>
#include <kvs/OpenMP>
#include <kvs/Assert>


namespace
{

inline kvs::RGBColor InterpolatedColor( const kvs::ColorImage& image, const float x, const float y )
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

kvs::ColorImage SphericalMapImage( const CubicImages& cubic_images )
{
    const size_t w = cubic_images.frontImage().width();
    const size_t h = cubic_images.frontImage().height();

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
                pixel = ::InterpolatedColor( cubic_images.rightImage(), si, sj );
            }
            else if ( xa == -1 )
            {
                const float si = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::InterpolatedColor( cubic_images.leftImage(), si, sj );
            }
            else if ( ya == 1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f - 1.0f ) * ( h - 1 ) );
                pixel = ::InterpolatedColor( cubic_images.bottomImage(), si, sj );
            }
            else if ( ya == -1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( za + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::InterpolatedColor( cubic_images.topImage(), si, sj );
            }
            else if ( za == 1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::InterpolatedColor( cubic_images.frontImage(), si, sj );
            }
            else if ( za == -1 )
            {
                const float si = kvs::Math::Abs( ( ( xa + 1.0f ) / 2.0f - 1.0f ) * ( w - 1 ) );
                const float sj = kvs::Math::Abs( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) );
                pixel = ::InterpolatedColor( cubic_images.backImage(), si, sj );
            }
            result.setPixel( i, j, pixel );
        }
    }

    return result;
}
