#include "Context.h"
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

namespace osmesa
{

/*===========================================================================*/
/**
 *  @brief  Construct a Context class.
 */
/*===========================================================================*/
Context::Context():
    m_handle( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroy the Context class.
 */
/*===========================================================================*/
Context::~Context()
{
    this->destroy();
}

/*===========================================================================*/
/**
 *  @brief  Create a OSMesa rendering context
 *  @param  format [in] pixel format (OSMESA_*: COLOR_INDEX, RGBA, BGRA, ARGB, RGB, BGR)
 *  @param  depth_bits [in] desired bit size of depth buffer
 *  @param  stencil_bits [in] desired bit size of stencil buffer
 *  @param  accum_bits [in] desired bit size of accumulation buffer
 *  @return true if success, false if the context is invalid (error)
 */
/*===========================================================================*/
bool Context::create( GLenum format, GLint depth_bits, GLint stencil_bits, GLint accum_bits )
{
#if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
    m_handle = OSMesaCreateContextExt( format, depth_bits, stencil_bits, accum_bits, NULL );
#else
    kvs::IgnoreUnusedVariable( depth_bits );
    kvs::IgnoreUnusedVariable( stencil_bits );
    kvs::IgnoreUnusedVariable( accum_bits );
    m_handle = OSMesaCreateContext( format, NULL );
#endif
    return this->isValid();
}

/*===========================================================================*/
/**
 *  @brief  Destroy the Context class.
 */
/*===========================================================================*/
void Context::destroy()
{
    if ( m_handle )
    {
        OSMesaDestroyContext( m_handle );
        m_handle = NULL;
    }
}

/*===========================================================================*/
/**
 *  @brief  Bind the context to a buffer and make it current buffer for rendering.
 *  @param  buffer [in] rendering buffer
 *  @param  width [in] width of rendering buffer
 *  @param  height [in] height of rendering buffer
 *  @return true if success, false if error because of invalid ctx,
 *          invalid buffer address, type!=GL_UNSIGNED_BYTE, width<1, height<1,
 *          width>internal limit or height>internal limit
 */
/*===========================================================================*/
bool Context::makeCurrent( kvs::ValueArray<kvs::UInt8>& buffer, GLsizei width, GLsizei height )
{
    GLboolean status = OSMesaMakeCurrent( m_handle, buffer.data(), GL_UNSIGNED_BYTE, width, height);
    return status == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Release the current buffer.
 */
/*===========================================================================*/
void Context::releaseCurrent()
{
    OSMesaMakeCurrent( NULL, NULL, GL_UNSIGNED_BYTE, 0, 0 );
}

} // end of namespace osmesa

} // end of namespace kvs
