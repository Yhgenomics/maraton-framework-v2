///* * * * * * * * * * * * * * * *
//* YHGenomics Inc.
//* Author     : yang shubo
//* Date       : 2015-11-20
//* Description:
//* * * * * * * * * * * * * * * */
//
//#ifndef HTTP_H_
//#define HTTP_H_
//
//#include <string>
//#include <map>
//#include <functional>
//
//#include "Macro.h"
//#include "Buffer.h"
//#include "Connector.h"
//#include "CircleBuffer.h"
//
//NS_MARATON_BEGIN
//
//class HTTPRequest;
//class HTTPResponse;
//
//typedef std::function<void( uptr<HTTPResponse>)>  callback_response_t;
//typedef std::function<uptr<Buffer>( HTTPRequest* req )> write_callback_t;
//typedef std::function<uptr<Buffer>( HTTPResponse* req )> read_callback_t;
//
//// HTTPRequest
//class HTTPRequest
//{
//public:
//     
//    HTTPRequest( std::string path ,
//                 std::string method );
//
//    void write_callback         ( write_callback_t callback );
//
//    void content                ( uptr<Buffer> content );
//    void content_length         ( size_t size );
//
//    void header                 ( std::string key ,
//                                  std::string value );
//
//    std::string host            ( ){ return this->ip_;};
//
//    void parse                  ( uptr<Buffer> data );
//
//    uptr<Buffer> build_header   ( );
//    uptr<Buffer> build_body     ( );
//
//    void* data                  ( );
//    void data                   ( void* value );
//     
//private:
//
//    std::string                         ip_             = "";
//    std::string                         url_            = "";
//    std::string                         method_         = "";
//    std::map<std::string , std::string> header_;
//    size_t                              content_length_ = 0;
//    uptr<Buffer>                        content_        = nullptr;
//    write_callback_t                    write_callback_ = nullptr;
//    void*                               data_           = nullptr;
//
//    // Parsing used
//    // ==================================================================
//    enum ParseState
//    {
//        kMethod = 1,
//        kUrl,
//        kProtocol,
//        kHeadKey,
//        kHeadValue,
//        kContent
//    };
//
//    ParseState  parse_state_ = ParseState::kMethod;
//    std::string tmp_key_     = "";
//    std::string tmp_value_   = "";
//    std::string protocol_    = "";
//};
//
//// HTTPResponse
//class HTTPResponse
//{
//public:
//     
//    HTTPResponse                 ( size_t status );
//    HTTPResponse                 ( );
//
//    void         read_callback   ( read_callback_t callback );
//                                 
//    void*        data            ( );
//    void         data            ( void* value );
//                                        
//    void         header          ( std::string key , 
//                                   std::string value );
//    std::string  header          ( std::string key );
//                 
//    void         content         ( uptr<Buffer> content );
//    uptr<Buffer> content         ( );
//    void         content_length  ( size_t size );
//    size_t       content_length  ( );
//
//    void         status          ( size_t code );
//    size_t       status          ( );
//
//    uptr<Buffer> build_header    ( );
//    uptr<Buffer> build_body      ( );
//
//    void         parse           ( uptr<Buffer> data );
//
//private: 
//
//    size_t          status_         = 0;
//    std::string     status_str_     = "";
//    size_t          content_length_ = 0;
//    std::string     protocol_       = "";
//    read_callback_t read_callback_  = nullptr;
//    void*           data_           = nullptr;
//    uptr<Buffer>    content_        = nullptr;
//    std::map<
//       std::string , 
//       std::string> header_;
//
//    // Parsing used
//    // ==================================================================
//    enum ParseState
//    {
//        kProtocol = 1,
//        kStatusCode,
//        kStatus,
//        kHeadKey,
//        kHeadValue,
//        kContent
//    };
//   
//    ParseState   parse_state_ = ParseState::kProtocol;
//    std::string  tmp_key_     = "";
//    std::string  tmp_value_   = "";
//};
//
//// WebSession
//class WebRequestSession :
//    public Session
//{
//public:
//
//    WebRequestSession( sptr<HTTPRequest> req ); 
//
//    uptr<HTTPResponse> response( )
//    {
//        return move_ptr( this->rep_ );
//    }
//
//protected:
//
//    virtual void on_connect ( );
//    virtual void on_read    ( uptr<Buffer> data );
//    virtual void on_write   ( uptr<Buffer> data ) { };
//    virtual void on_close   ( )                   { };
//
//private:
//
//    sptr<HTTPRequest>  req_ = nullptr;
//    uptr<HTTPResponse> rep_ = nullptr;
//
//    size_t             recived_length = 0;
//};
//
//// WebConnector
//class WebConnector :
//    public Connector
//{
//public:
//     
//    WebConnector ( std::string addr, 
//                   sptr<HTTPRequest> req ,
//                   callback_response_t cb);
//    ~WebConnector( ){ };
//protected:
//    
//    virtual Session* create_session ( );
//
//    virtual void on_new_session     ( Session * session ) override;
//     
//    virtual void on_close           ( ) override;
//
//private:
//
//    sptr<HTTPRequest>       req_;
//    callback_response_t     callback_ = nullptr;
//
//    // Í¨¹ý Connector ¼Ì³Ð
//    virtual void on_destroy_session( Session * session ) override;
//};
//
//NS_MARATON_END
//
//#endif // !HTTP_H_
