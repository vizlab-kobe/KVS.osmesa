#pragma once

#include "OSMesa.h"
#include <kvs/ValueArray>
#include <kvs/Type>


namespace kvs
{

namespace osmesa
{

class Context
{
private:
    OSMesaContext m_handle;

public:
    Context();
    ~Context();

    void* handle() { return m_handle; }
    bool isValid() const { return m_handle != NULL; }

    bool create( GLenum format, GLint depth_bits, GLint stencil_bits, GLint accum_bits );
    void destroy();
    bool makeCurrent( kvs::ValueArray<kvs::UInt8>& buffer, GLsizei width, GLsizei height );
    void releaseCurrent();
};

} // end of namespace osmesa

} // end of namespace kvs
