#include <errno.h>   // errno
#include <cstring>  // strerror

#include <FL/fl_utf8.h>

#include "mrvFl/mrvUtil.h"
#include "mrvFl/mrvIO.h"

namespace
{
  const char* kModule = "util";
}

namespace mrv
{
    Fl_SVG_Image* load_svg( const std::string& svg_name )
    {
        const char* root =  fl_getenv( "MRV_ROOT" );
        if ( !root ) root = "..";
    
        std::string svg_root = root;
        svg_root += "/icons/";
    
        std::string file = svg_root + svg_name;
        Fl_SVG_Image* svg = new Fl_SVG_Image( file.c_str() );
        if ( !svg ) return nullptr;
      
        switch (svg->fail()) {
        case Fl_Image::ERR_FILE_ACCESS:
            // File couldn't load? show path + os error to user
            LOG_ERROR( file << ": " << strerror(errno) );
            return nullptr;
        case Fl_Image::ERR_FORMAT:
            // Parsing error
            LOG_ERROR( file << ": couldn't decode image" );
            return nullptr;
        }
        return svg;
    }
}
