/**
 * Bit Reader
 *
 * Created on October 23, 2010, 2:32 PM
 *
 * Copyright 2010 Yauhen Yakimovich eugeny(dot)yakimovitch(at)gmail(dot)com
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned long bitindex;

class BitReader {
private:
    bitindex length;
    ifstream in;

    long get_io_stream_length(ifstream &stream) {
        long currentPosition = stream.tellg();
        stream.seekg(0, ios::end);
        long result = stream.tellg();
        stream.seekg(currentPosition, ios::beg);

        return result;
    };

    long get_position_in_bytes() {
        // get position in bytes from position in bits
        return (long) (this->index / 8);
    }

public:
    bitindex index; // position in bits

    BitReader(const char* filename) {
        this->index = 0;
        this->in.open(filename, ios::in | ios::binary);
        if (!this->in.is_open()) {
            cout << "Failed to open : " << filename << endl;
            exit(EXIT_FAILURE);
        }
        this->length = (bitindex) this->get_io_stream_length(this->in) * 8;
    }

    /**
     * Obtain bit value at in.index
     *
     * @return bool
     */
    bool get_bit() {
        long position = (long) (this->index / 8); // bytes
        int state_index = this->index % 8;
        char byte = 0;
        this->assert_stream_position(position + 1);
        this->in.seekg(position, ios::beg);
        this->in.read(&byte, 1);
        return (byte & (1 << state_index));
    }

    bool has_ended() {
        return (this->index >= this->length);
    }

    void assert_stream_position(bitindex position) {
        if (position > get_io_stream_length(in)) {
            cout << "Input stream index is out of range." << endl;
            exit(EXIT_FAILURE);
        }
    }

    void static output(const char* filename) {
        BitReader input(filename);

        while (!input.has_ended()) {
            bool state = input.get_bit();
            const char *state_str = (state) ? "1" : "0";
            cout << state_str;
            input.index++;
        }
        cout << endl;
    }
};

/*
 * Program entry point
 *
 */
int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Bit Reader reads a file and outputs its data as a binary string of {0,1}." << endl;
        cout << "Copyright 2010 Yauhen Yakimovich. Redistribution under FreeBSD license." << endl;
        cout << "Usage: bitreader <filename>" << endl;
        exit(EXIT_SUCCESS);
    }
    BitReader::output(argv[1]);

    return (EXIT_SUCCESS);
}