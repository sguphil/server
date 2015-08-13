#!/bin/sh

protoc -I. --cpp_out=../ *.proto

