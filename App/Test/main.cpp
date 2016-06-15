#include <iostream>
#include "OSMesa.h"
#include "ScreenBase.h"

#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/PolygonObject>
#include <kvs/PolygonRenderer>
#include <kvs/Isosurface>
#include <kvs/HydrogenVolumeData>

int main( int argc, char** argv )
{
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::HydrogenVolumeData( kvs::Vector3ui( 64, 64, 64 ) );

    const double i = ( volume->maxValue() + volume->minValue() ) * 0.5;
    const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
    const bool d = false;
    const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object = new kvs::Isosurface( volume, i, n, d, t );
    delete volume;

    //    kvs::glsl::PolygonRenderer* renderer = new kvs::glsl::PolygonRenderer();
    kvs::PolygonRenderer* renderer = new kvs::PolygonRenderer();

    kvs::osmesa::ScreenBase screen;
    screen.scene()->registerObject( object, renderer );
    screen.setGeometry( 0, 0, 512, 512 );

    screen.create();
    screen.draw();
    screen.capture().write("output.bmp");

    return 0;
}
