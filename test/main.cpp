
#include <cstdint>
#include <rapidcheck.h>

#include <base64.h>

namespace {
    bool prop_encode_then_decode_should_match () {
        auto condition = [] (const std::vector<uint8_t> &input) {
            RC_CLASSIFY (input.size () % 3 == 0, "Full blocks") ;
            RC_CLASSIFY (input.size () % 3 == 1, "1 octet remaining") ;
            RC_CLASSIFY (input.size () % 3 == 2, "2 octets remaining") ;
            auto actual = base64_decode (base64_encode (input)) ;
            RC_ASSERT (actual == input) ;
        } ;
        return rc::check ("Encode then decode should return to original", condition) ;
    }
}

int main (int argc, char **argv) {
    if (! prop_encode_then_decode_should_match ()) {
        return -1 ;
    }
    return 0 ;
}
