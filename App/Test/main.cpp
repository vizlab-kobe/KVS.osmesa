#include <iostream>
#include "OSMesa.h"
#include "ScreenBase.h"

#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/PolygonObject>
#include <kvs/PolygonRenderer>
#include <kvs/Isosurface>
#include <kvs/HydrogenVolumeData>
#include <kvs/RayCastingRenderer>
#include <kvs/PointObject>
#include <kvs/CellByCellUniformSampling>
#include <kvs/ParticleBasedRenderer>
#include <kvs/TransferFunction>


int main( int argc, char** argv )
{
    std::cout << "OSMesa Version: " << kvs::osmesa::Version() << std::endl;

    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::HydrogenVolumeData( kvs::Vector3ui( 64, 64, 64 ) );

/*
    const double i = ( volume->maxValue() + volume->minValue() ) * 0.5;
    const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
    const bool d = false;
    const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object = new kvs::Isosurface( volume, i, n, d, t );
    delete volume;

    // kvs::glsl::PolygonRenderer* renderer = new kvs::glsl::PolygonRenderer();
    kvs::PolygonRenderer* renderer = new kvs::PolygonRenderer();
*/
    kvs::StructuredVolumeObject* object = volume;
    kvs::glsl::RayCastingRenderer* renderer = new kvs::glsl::RayCastingRenderer();
/*
    kvs::TransferFunction tfunc( 256 );
    kvs::PointObject* object = new kvs::CellByCellUniformSampling( volume, 2, 0.5, tfunc );
//    kvs::ParticleBasedRenderer* renderer = new kvs::ParticleBasedRenderer();
    kvs::glsl::ParticleBasedRenderer* renderer = new kvs::glsl::ParticleBasedRenderer();
    renderer->setRepetitionLevel( 4 );
*/

    kvs::osmesa::ScreenBase screen;
    screen.scene()->registerObject( object, renderer );
    screen.setGeometry( 0, 0, 512, 512 );

    screen.create();
    screen.draw();
    screen.capture().write("output.bmp");

    return 0;
}
