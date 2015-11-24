#include "MRT.h"

int main(int argc,char** argv)
{     
    MRT::WebClient wc;
    FILE* file = fopen( "D:\\test.jpg" , "wb" );
    wc.dl_file( "http://ww2.sinaimg.cn/bmiddle/701cac0cjw1eyc5h4dkmcj20hs0dcab5.jpg" , file , [ file ] ( uptr<MRT::HTTPResponse> rep )
    { 
        fclose( file );
    } );

    while(true)
    {
        MRT::Maraton::instance( )->loop( );
    }
    return 0;
}