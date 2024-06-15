#ifndef REQUEST_H
#define REQUEST_H

namespace Api {

class Request {

    public:

        explicit Request();
        virtual ~Request();

        // inhibit copy
        Request(Request& other) = delete;
        Request& operator=(Request& other) = delete;

};


}


#endif // REQUEST_H