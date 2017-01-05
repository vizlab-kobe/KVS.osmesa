#pragma once
#include "CubicImages.h"
#include <kvs/Math>


kvs::ColorImage SphericalMapImage( const CubicImages& cubic_images )
{
    const size_t w = cubic_images.frontImage().width();
    const size_t h = cubic_images.frontImage().height();

    kvs::ColorImage result( w * 4, h * 3 );

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
                const int si = kvs::Math::Abs( int( ( ( za + 1.0f ) / 2.0f - 1.0f ) * ( w - 1 ) ) );
                const int sj = kvs::Math::Abs( int( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) ) );
                pixel = cubic_images.rightImage().pixel( si, sj );
            }
            else if ( xa == -1 )
            {
                const int si = kvs::Math::Abs( int( ( ( za + 1.0f ) / 2.0f ) * ( w - 1 ) ) );
                const int sj = kvs::Math::Abs( int( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) ) );
                pixel = cubic_images.leftImage().pixel( si, sj );
            }
            else if ( ya == 1 )
            {
                const int si = kvs::Math::Abs( int( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) ) );
                const int sj = kvs::Math::Abs( int( ( ( za + 1.0f ) / 2.0f - 1.0f ) * ( h - 1 ) ) );
//                pixel = cubic_images.topImage().pixel( si, sj );
                pixel = cubic_images.bottomImage().pixel( si, sj );
            }
            else if ( ya == -1 )
            {
                const int si = kvs::Math::Abs( int( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) ) );
                const int sj = kvs::Math::Abs( int( ( ( za + 1.0f ) / 2.0f ) * ( h - 1 ) ) );
//                pixel = cubic_images.bottomImage().pixel( si, sj );
                pixel = cubic_images.topImage().pixel( si, sj );
            }
            else if ( za == 1 )
            {
                const int si = kvs::Math::Abs( int( ( ( xa + 1.0f ) / 2.0f ) * ( w - 1 ) ) );
                const int sj = kvs::Math::Abs( int( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) ) );
                pixel = cubic_images.frontImage().pixel( si, sj );
            }
            else if ( za == -1 )
            {
                const int si = kvs::Math::Abs( int( ( ( xa + 1.0f ) / 2.0f - 1.0f ) * ( w - 1 ) ) );
                const int sj = kvs::Math::Abs( int( ( ( ya + 1.0f ) / 2.0f ) * ( h - 1 ) ) );
                pixel = cubic_images.backImage().pixel( si, sj );
            }
            result.setPixel( i, j, pixel );
        }
    }

    return result;
}
