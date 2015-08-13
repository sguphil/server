#!/bin/sh

../../3rd/protobuf-3.0.0-alpha-3.1/src/protoc -I. --cpp_out=../ *.proto

