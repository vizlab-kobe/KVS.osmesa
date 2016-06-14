#pragma once

#include "OSMesa.h"
#include <kvs/ScreenBase>
#include <kvs/ValueArray>
#include <kvs/ColorImage>
#include <kvs/Scene>


namespace kvs
{

namespace osmesa
{

class ScreenBase : public kvs::ScreenBase
{
    typedef kvs::ScreenBase BaseClass;

private:

    OSMesaContext m_context;
    kvs::ValueArray<kvs::UInt8> m_buffer;
    kvs::Scene* m_scene; ///< default scene

public:

    kvs::Scene* scene() { return m_scene; }

    ScreenBase();
    virtual ~ScreenBase();

    virtual void create();

    void draw();
    kvs::ColorImage capture() const;
};

} // end of namespace osmesa

} // end of namepsace kvs
