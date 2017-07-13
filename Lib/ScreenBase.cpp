#include "ScreenBase.h"
#include <kvs/ColorImage>
#include <kvs/OpenGL>


namespace kvs
{

namespace osmesa
{

ScreenBase::ScreenBase()
{
}

ScreenBase::~ScreenBase()
{
}

kvs::ColorImage ScreenBase::capture() const
{
    const size_t width = BaseClass::width();
    const size_t height = BaseClass::height();

    // RGBA to RGB
    const kvs::ValueArray<kvs::UInt8>& buffer = m_surface.buffer();
    kvs::ValueArray<kvs::UInt8> pixels( width * height * 3 );
    for ( size_t i = 0; i < width * height; i++ )
    {
        pixels[ 3 * i + 0 ] = buffer[ 4 * i + 0 ];
        pixels[ 3 * i + 1 ] = buffer[ 4 * i + 1 ];
        pixels[ 3 * i + 2 ] = buffer[ 4 * i + 2 ];
    }

    return kvs::ColorImage( width, height, pixels );
}

void ScreenBase::draw()
{
    if ( !m_context.isValid() ) { this->create(); }
    this->paintEvent();
}

void ScreenBase::create()
{
    // Create OSMesa context
    const GLenum format = OSMESA_RGBA;
    const GLint depth_bits = 24;
    const GLint stencil_bits = 0;
    const GLint accum_bits = 0;
    m_context.create( format, depth_bits, stencil_bits, accum_bits );
    if ( !m_context.isValid() )
    {
        kvsMessageError( "Cannot create OSMesa context." );
        return;
    }

    // Create OSMesa drawing surface
    const GLsizei width = GLsizei( BaseClass::width() );
    const GLsizei height = GLsizei( BaseClass::height() );
    m_surface.create( width, height, format );

    // Bind the context to the surface
    if ( !m_context.makeCurrent( m_surface ) )
    {
        kvsMessageError( "Cannot bind buffer." );
        return;
    }

    // Y coordinates increase downward
    kvs::osmesa::Context::SetYAxisDirectionToDown();

    this->initializeEvent();
}

void ScreenBase::show()
{
    if ( !m_context.isValid() ) { this->create(); }
}

void ScreenBase::redraw()
{
    this->draw();
}

} // end of namespace osmesa

} // end of namespace kvs
