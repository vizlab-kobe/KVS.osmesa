#pragma once
#include "CubicImages.h"


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

}

kvs::ColorImage CubeMapImage( const CubicImages& cubic_images )
{
    const size_t w = cubic_images.front().width();
    const size_t h = cubic_images.front().height();

    kvs::ColorImage result( w * 4, h * 3 );
    ::Draw( cubic_images.top(), w, 0, result );
    ::Draw( cubic_images.left(), 0, h, result );
    ::Draw( cubic_images.front(), w, h, result );
    ::Draw( cubic_images.right(), w * 2, h, result );
    ::Draw( cubic_images.back(), w * 3, h, result );
    ::Draw( cubic_images.bottom(), w, h * 2, result );

    return result;
}
