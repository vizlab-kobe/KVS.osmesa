#pragma once

#include "OSMesa.h"
#include <kvs/ScreenBase>
#include <kvs/ValueArray>
#include <kvs/ColorImage>


namespace kvs
{

namespace osmesa
{

class ScreenBase : public kvs::ScreenBase
{
    typedef kvs::ScreenBase BaseClass;

private:

    OSMesaContext m_context; ///< OSMesa context
    kvs::ValueArray<kvs::UInt8> m_buffer; ///< frame buffer

public:

    ScreenBase();
    virtual ~ScreenBase();

    const kvs::ValueArray<kvs::UInt8>& buffer() const { return m_buffer; }
    kvs::ColorImage capture() const;
    void draw();

    virtual void create();
    virtual void show();
    virtual void redraw();

    virtual void initializeEvent() {}
    virtual void paintEvent() {}
};

} // end of namespace osmesa

} // end of namepsace kvs
