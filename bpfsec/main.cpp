#include <iostream>
#include <string>

#include "bpfsec.h"
#include "config.h"

using namespace bpfsec;

int main(int argc, char const *argv[])
{
    const Config config = get_config("/home/humac/Projects/bpf_security/config.yaml");
    Bpfsec bpfsecd(config);
    bpfsecd.run();

    return 0;
}
