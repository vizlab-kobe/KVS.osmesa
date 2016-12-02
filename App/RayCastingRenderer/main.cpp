#include <iostream>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/RayCastingRenderer>
#include <kvs/HydrogenVolumeData>
#include <kvs/TransferFunction>
#include <KVS.osmesa/Lib/OSMesa.h>
#include <KVS.osmesa/Lib/Screen.h>


int main( int argc, char** argv )
{
    std::cout << "OSMesa version: " << kvs::osmesa::Version() << std::endl;

    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) { object = new kvs::StructuredVolumeImporter( std::string( argv[1] ) ); }
    else { object = new kvs::HydrogenVolumeData( kvs::Vec3u( 32, 32, 32 ) ); }

    kvs::Real32 sampling_step = 0.5f;
    kvs::Real32 opaque_value = 0.97f;
    kvs::TransferFunction transfer_function( 256 );
    kvs::glsl::RayCastingRenderer* renderer = new kvs::glsl::RayCastingRenderer();
    renderer->setSamplingStep( sampling_step );
    renderer->setOpaqueValue( opaque_value );
    renderer->setTransferFunction( transfer_function );

    kvs::osmesa::Screen screen;
    screen.setGeometry( 0, 0, 512, 512 );
    screen.registerObject( object, renderer );
    screen.draw();

    std::string filename = "output.bmp";
    screen.capture().write( filename );
    std::cout << "Rendering to " << filename << std::endl;

    return 0;
}
