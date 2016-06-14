#include "ScreenBase.h"
#include <kvs/ColorImage>
#include <kvs/OpenGL>
#include <kvs/PaintEvent>
#include <kvs/EventHandler>


namespace kvs
{

namespace osmesa
{

ScreenBase::ScreenBase():
    m_context( NULL ),
    m_scene( new kvs::Scene() )
{
}

ScreenBase::~ScreenBase()
{
    if ( m_context )
    {
        OSMesaDestroyContext( m_context );
        m_context = NULL;
    }

    if ( m_scene )
    {
        delete m_scene;
        m_scene = NULL;
    }
}

void ScreenBase::create()
{
    // Create OSMesa context
    const GLenum format = OSMESA_RGB;
    const GLint depth_bits = 24;
    const GLint stencil_bits = 0;
    const GLint accum_bits = 0;
    m_context = OSMesaCreateContextExt( format, depth_bits, stencil_bits, accum_bits, NULL );
    if ( !m_context )
    {
        kvsMessageError( "Cannot create OSMesa context." );
        return;
    }

    // Allocate memory for the buffer
    const GLsizei width = GLsizei( BaseClass::width() );
    const GLsizei height = GLsizei( BaseClass::height() );
    m_buffer.allocate( width * height * 3 );

    // Bind the buffer to the context
    if ( !OSMesaMakeCurrent( m_context, m_buffer.data(), GL_UNSIGNED_BYTE, width, height) )
    {
        kvsMessageError( "Cannot bind buffer." );
        return;
    }
}

void ScreenBase::draw()
{
    if ( !m_context ) { this->create(); }

    kvs::OpenGL::WithPushedMatrix p( GL_MODELVIEW );
    p.loadIdentity();
    {
        m_scene->paintFunction();
        kvs::PaintEvent event;
        BaseClass::eventHandler()->notify( &event );
    }

    kvs::OpenGL::Flush();
}

kvs::ColorImage ScreenBase::capture() const
{
    const size_t width = BaseClass::width();
    const size_t height = BaseClass::height();
    kvs::ColorImage ret( width, height, m_buffer );
    return ret;
}

} // end of namespace osmesa

} // end of namespace kvs
