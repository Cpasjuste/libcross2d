#include "shaders.h"

const char c2d_crt_fakelottes_shader[2816] = {
	0x38, 0xb4, 0xfa, 0x10, 0x7f, 0x7a, 0x2d, 0xa8, 0x00, 0x01, 0x05, 
	0x43, 0x02, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x0a, 0xe0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x19, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x2c, 0x00, 0x0a, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0x8a, 0x20, 0x50, 0x53, 0x55, 0x23, 
	0x00, 0x00, 0x00, 0x7e, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 
	0x38, 0x40, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0xd8, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x0a, 0x00, 0xf6, 0x01, 0xff, 0xff, 0x00, 0x00, 0x0a, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
	0x10, 0x00, 0x0a, 0x00, 0x80, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x09, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x0a, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0b, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x04, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x07, 0x00, 
	0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 
	0x02, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0b, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x0d, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0e, 
	0x00, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0f, 0x00, 0x0f, 0x00, 
	0x00, 0x00, 0xff, 0x03, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 
	0x00, 0x02, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x08, 0x83, 0x21, 0x15, 0x80, 0x38, 0x00, 
	0x01, 0x10, 0x83, 0x21, 0x05, 0x80, 0x38, 0x06, 0x82, 0x99, 0xaf, 
	0xbc, 0x0d, 0x80, 0x40, 0x7c, 0x02, 0x44, 0xcf, 0x84, 0x07, 0xa4, 
	0x08, 0x07, 0x11, 0x45, 0xcf, 0x80, 0x87, 0xb1, 0x18, 0x05, 0x11, 
	0x49, 0xcf, 0x80, 0x87, 0xb1, 0x18, 0x0b, 0x11, 0x0d, 0xc0, 0x81, 
	0x81, 0xb1, 0x18, 0x0c, 0xd1, 0x4e, 0xc0, 0x81, 0x81, 0xb1, 0x18, 
	0x08, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x43, 0x4f, 0x4c, 0x30, 0x00, 
	0x00, 0x00, 0x05, 0x05, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 
	0x52, 0x00, 0x00, 0x00, 0x05, 0x04, 0x03, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0x01, 0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x67, 0x6c, 
	0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x00, 
	0x01, 0x05, 0x05, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x4d, 0x56, 0x50, 0x4d, 0x61, 
	0x74, 0x72, 0x69, 0x78, 0x00, 0x00, 0x00, 0x16, 0x03, 0x03, 0x00, 
	0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0xff, 0xff, 0x00, 
	0x00, 0x56, 0x65, 0x72, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6f, 0x72, 
	0x64, 0x00, 0x00, 0x00, 0x05, 0x04, 0x03, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0x01, 0x00, 0x04, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x54, 0x45, 
	0x58, 0x30, 0x00, 0x00, 0x00, 0x05, 0x05, 0x03, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x01, 0x00, 0x04, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x54, 
	0x65, 0x78, 0x43, 0x6f, 0x6f, 0x72, 0x64, 0x00, 0x00, 0x00, 0x05, 
	0x04, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x08, 0x04, 
	0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x04, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 
	0x2c, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0c, 
	0x20, 0x50, 0x53, 0x55, 0x23, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 
	0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x77, 0x00, 0xff, 0xff, 
	0x00, 0x00, 0x03, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xfc, 0xff, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x20, 0x00, 0x36, 0x00, 0x00, 0x00, 0x80, 0x00, 
	0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 
	0x00, 0x04, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x36, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x06, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x07, 0x00, 0x04, 0x00, 
	0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x02, 
	0x00, 0x01, 0x00, 0x05, 0x00, 0x10, 0x00, 0x02, 0x00, 0x02, 0x00, 
	0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x06, 0x00, 0x10, 
	0x00, 0x05, 0x00, 0x89, 0x88, 0x88, 0x39, 0x07, 0x00, 0x00, 0x00, 
	0x05, 0x00, 0xab, 0xaa, 0x2a, 0x3b, 0x08, 0x00, 0x00, 0x00, 0x05, 
	0x00, 0xab, 0xaa, 0xaa, 0x3c, 0x09, 0x00, 0x00, 0x00, 0x05, 0x00, 
	0x00, 0x00, 0x00, 0x3e, 0x0a, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0x0b, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 
	0x80, 0x3f, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x0d, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x31, 0x00, 0x0e, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x1f, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 
	0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x11, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x39, 0x00, 0x12, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x3b, 0x00, 0x13, 0x00, 0x00, 0x00, 0x01, 0x00, 0x21, 0x00, 
	0x14, 0x00, 0x00, 0x00, 0x01, 0x00, 0x22, 0x00, 0x15, 0x00, 0x00, 
	0x00, 0x02, 0x00, 0x3e, 0x00, 0x16, 0x00, 0x00, 0x00, 0x02, 0x00, 
	0x3f, 0x00, 0x16, 0x00, 0x10, 0x00, 0x01, 0x00, 0x24, 0x00, 0x17, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x26, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x27, 0x00, 0x19, 0x00, 0x00, 0x00, 0x02, 0x00, 0x16, 
	0x00, 0x1a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x17, 0x00, 0x1a, 0x00, 
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x09, 0x00, 0xf8, 0x00, 
	0x00, 0x0c, 0xa0, 0x81, 0x02, 0x8b, 0x48, 0x00, 0x00, 0x00, 0xa0, 
	0x84, 0x00, 0xa3, 0xe9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x20, 
	0xf9, 0x00, 0x91, 0x80, 0xaf, 0x90, 0x0b, 0xc1, 0x40, 0x00, 0x2d, 
	0xa0, 0x6f, 0x84, 0x0b, 0x83, 0x40, 0xbc, 0x80, 0x01, 0x17, 0x82, 
	0x88, 0xe1, 0x18, 0x00, 0x87, 0x99, 0xaf, 0xbc, 0x0b, 0xc1, 0x40, 
	0x00, 0x2e, 0xa0, 0x6f, 0x84, 0x0b, 0xc3, 0x40, 0x3d, 0x80, 0x05, 
	0x07, 0x02, 0x89, 0xe1, 0x18, 0x00, 0x00, 0x6c, 0x41, 0x02, 0x0d, 
	0x82, 0x38, 0x82, 0x00, 0x60, 0x42, 0x4a, 0x0c, 0x82, 0x30, 0x0b, 
	0x04, 0x42, 0xa6, 0x8e, 0x0d, 0xc0, 0x40, 0x00, 0x00, 0x68, 0x41, 
	0x02, 0x0d, 0x82, 0x38, 0x01, 0x3e, 0xe0, 0x00, 0x12, 0x08, 0xa0, 
	0x30, 0x02, 0x3e, 0xe0, 0x00, 0x1a, 0x08, 0xa0, 0x30, 0xfc, 0x01, 
	0x04, 0x86, 0x86, 0x49, 0xa4, 0x10, 0x01, 0x3e, 0xe0, 0x00, 0x02, 
	0x08, 0xa0, 0x30, 0x02, 0x3e, 0xe0, 0x00, 0x0a, 0x00, 0xa0, 0x30, 
	0xfc, 0x01, 0xc4, 0x85, 0x86, 0xb1, 0xa4, 0x10, 0x40, 0x00, 0x44, 
	0x42, 0x0a, 0x05, 0x82, 0x38, 0x00, 0x03, 0x74, 0x43, 0x02, 0x05, 
	0x82, 0x38, 0x00, 0x03, 0x54, 0x41, 0x02, 0x05, 0x82, 0x38, 0x80, 
	0x03, 0x58, 0x81, 0x0a, 0x05, 0x80, 0x38, 0x00, 0x1d, 0x00, 0xa5, 
	0x0a, 0x00, 0x81, 0x50, 0x80, 0x03, 0x50, 0x82, 0x0a, 0x05, 0x80, 
	0x38, 0x00, 0x04, 0x4c, 0x81, 0x0a, 0x05, 0x80, 0x38, 0x00, 0x18, 
	0x81, 0xa4, 0x8e, 0x0b, 0x81, 0x40, 0x00, 0x17, 0xc1, 0xa1, 0x8e, 
	0x0b, 0x81, 0x40, 0x3a, 0x16, 0x83, 0xa7, 0x8e, 0x0d, 0x80, 0x40, 
	0x00, 0x15, 0xc0, 0xa7, 0x0a, 0x00, 0x81, 0x50, 0x80, 0x0b, 0xe0, 
	0xa6, 0x0a, 0x00, 0x81, 0x50, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
	0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x15, 0x00, 
	0x02, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x04, 0x00, 0x04, 
	0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x01, 0x40, 0xa0, 0x46, 0x0d, 
	0x81, 0x40, 0xc1, 0x13, 0x0d, 0xef, 0xa8, 0x00, 0xc0, 0x10, 0x12, 
	0x00, 0x24, 0xb0, 0x82, 0x41, 0x04, 0x08, 0x01, 0x01, 0x00, 0x50, 
	0x8c, 0x21, 0x23, 0x00, 0x01, 0x00, 0x00, 0x20, 0x8e, 0x0d, 0x01, 
	0x40, 0x00, 0x80, 0x00, 0x20, 0x8a, 0x41, 0x40, 0x08, 0x0c, 0x0f, 
	0x10, 0x3f, 0x88, 0x00, 0xc0, 0x08, 0x9d, 0x00, 0xac, 0xcf, 0x00, 
	0x5c, 0x84, 0x28, 0x0b, 0xbe, 0x03, 0x30, 0x81, 0x86, 0x88, 0x48, 
	0x00, 0x04, 0xf4, 0xc4, 0x00, 0x05, 0x80, 0x38, 0x0f, 0xbe, 0x93, 
	0x3f, 0xa0, 0x86, 0x80, 0x78, 0x00, 0x0f, 0xf0, 0x07, 0x00, 0x05, 
	0x80, 0x38, 0x2a, 0x94, 0xc1, 0xff, 0x9c, 0x0d, 0x80, 0x40, 0x7e, 
	0xcf, 0xf3, 0x07, 0x20, 0x45, 0x80, 0x3d, 0x3e, 0x9e, 0x81, 0xff, 
	0x9c, 0x0d, 0x80, 0x41, 0x0c, 0x80, 0x91, 0x40, 0x80, 0x81, 0xe3, 
	0x18, 0x0c, 0x0f, 0xcd, 0x10, 0x80, 0x00, 0x81, 0x08, 0x81, 0x01, 
	0x00, 0xd0, 0x86, 0x31, 0x21, 0x00, 0x00, 0x00, 0x04, 0x20, 0x86, 
	0x41, 0x24, 0x08, 0x0c, 0x03, 0x00, 0x50, 0x86, 0x01, 0x23, 0x00, 
	0x07, 0x00, 0x04, 0xb1, 0x84, 0x41, 0x24, 0x08, 0x05, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 
	0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x06, 0x01, 0x00, 
	0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0xd3, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xa0, 0x00, 0x04, 
	0x03, 0xe0, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x80, 0x01, 
	0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x01, 0x00, 0x0a, 
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x02, 0x00, 
	0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x42, 0x04, 0x80, 0x30, 0x02, 0x00, 0x00, 0x00, 0x4a, 0x04, 0x80, 
	0x30, 0x81, 0x00, 0x20, 0x00, 0x42, 0x04, 0x80, 0x30, 0x0d, 0x00, 
	0x04, 0xbf, 0x84, 0x03, 0xa4, 0x08, 0x01, 0x3e, 0xa0, 0x0f, 0x00, 
	0x06, 0x80, 0x30, 0x02, 0x3e, 0xa0, 0x0f, 0x08, 0x06, 0x80, 0x30, 
	0x04, 0x3e, 0xa0, 0x0f, 0x10, 0x06, 0x80, 0x30, 0x02, 0x81, 0x11, 
	0x1f, 0x80, 0x83, 0xe1, 0x18, 0x04, 0x88, 0x00, 0xc0, 0x91, 0xca, 
	0x8c, 0x48, 0x1c, 0x01, 0x04, 0x30, 0x86, 0x41, 0xa4, 0x08, 0x4c, 
	0x03, 0x10, 0xd0, 0x86, 0x01, 0xa1, 0x00, 0x00, 0x20, 0x04, 0xa0, 
	0x86, 0x41, 0xa4, 0x08, 0x0e, 0x06, 0x80, 0x70, 0x84, 0x01, 0xa2, 
	0x00, 0x82, 0x00, 0x84, 0x0f, 0x88, 0x41, 0xc0, 0x08, 0x00, 0x0e, 
	0x00, 0x60, 0x0a, 0x00, 0x83, 0x50, 0x61, 0x0b, 0x36, 0x00, 0xd2, 
	0xd1, 0xa0, 0xfc, 0x88, 0x0f, 0x00, 0x30, 0xa4, 0x81, 0xa0, 0x00, 
	0x88, 0x0f, 0x10, 0x30, 0xa6, 0x81, 0xa0, 0x00, 0x89, 0x0f, 0x00, 
	0x30, 0x88, 0x81, 0xa0, 0x00, 0x89, 0x0f, 0x50, 0x30, 0x8c, 0x81, 
	0xa0, 0x00, 0x80, 0x02, 0xf4, 0xc3, 0x20, 0x05, 0x80, 0x38, 0x00, 
	0x11, 0x76, 0x2f, 0x80, 0x87, 0xb1, 0x18, 0x42, 0x0f, 0x08, 0x00, 
	0x86, 0x41, 0xa4, 0x08, 0x40, 0x8f, 0x0c, 0x20, 0x8a, 0x41, 0xc0, 
	0x08, 0x00, 0x10, 0x04, 0xa0, 0x82, 0x41, 0x80, 0x08, 0xc0, 0x06, 
	0x64, 0xef, 0x80, 0x41, 0x84, 0x08, 0x41, 0xc2, 0x7c, 0x5f, 0x80, 
	0x80, 0x83, 0x18, 0x7d, 0x91, 0x50, 0xcf, 0x88, 0x00, 0xc0, 0x08, 
	0x84, 0x88, 0x00, 0xc0, 0x81, 0x86, 0x8c, 0x49, 0x13, 0x82, 0x03, 
	0x30, 0x91, 0xca, 0x88, 0x49, 0x11, 0x82, 0x03, 0x30, 0x91, 0xc6, 
	0x88, 0x49, 0x34, 0x99, 0x81, 0xff, 0x9c, 0x0d, 0x80, 0x45, 0x3c, 
	0x81, 0x01, 0x1f, 0x80, 0x83, 0xe1, 0x18, 0x01, 0x3e, 0xa0, 0x0f, 
	0x40, 0x04, 0x80, 0x30, 0x02, 0x3e, 0xa0, 0x0f, 0x48, 0x04, 0x80, 
	0x30, 0x04, 0x3e, 0xa0, 0x0f, 0x50, 0x04, 0x80, 0x30, 0xc0, 0x04, 
	0xf4, 0xc8, 0x08, 0x05, 0x80, 0x38, 0x80, 0x0c, 0x00, 0xe0, 0x08, 
	0x00, 0x81, 0x50, 0x80, 0x11, 0x60, 0xe0, 0x08, 0x00, 0x81, 0x50, 
	0x3d, 0x00, 0x00, 0x0f, 0x80, 0x47, 0xa4, 0x08, 0x01, 0x3e, 0x00, 
	0x00, 0x00, 0x06, 0x80, 0x30, 0x02, 0x3e, 0x00, 0x00, 0x08, 0x06, 
	0x80, 0x30, 0x01, 0x3e, 0x20, 0x00, 0x10, 0x06, 0x80, 0x30, 0x02, 
	0x3e, 0x20, 0x00, 0x18, 0x06, 0x80, 0x30, 0x02, 0x80, 0x1d, 0x00, 
	0x3e, 0x0c, 0x80, 0x40, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 
	0x00, 0x3f, 0x80, 0x00, 0x00, 0x3c, 0xfd, 0xf3, 0xb6, 0x3d, 0x27, 
	0xef, 0x9e, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 
	0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x40, 0x0c, 0xcc, 0xcd, 
	0x40, 0x0c, 0xcc, 0xcd, 0x40, 0x0c, 0xcc, 0xcd, 0x3f, 0x80, 0x00, 
	0x00, 0x3e, 0xcc, 0xcc, 0xcd, 0x3e, 0xcc, 0xcc, 0xcd, 0x3e, 0xcc, 
	0xcc, 0xcd, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x80, 0x03, 0x47, 0x40, 
	0x49, 0x0e, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x2a, 0xaa, 
	0xab, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x80, 
	0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3e, 0xaa, 0xaa, 0xab, 0x3e, 
	0xaa, 0x7e, 0xfa, 0x3f, 0xc0, 0x00, 0x00, 0x3f, 0x2a, 0x7e, 0xfa, 
	0x3f, 0xc0, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x3f, 0x80, 0x00, 
	0x00, 0x38, 0xd1, 0xb7, 0x17, 0x3f, 0x7f, 0xf9, 0x72, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x40, 0xc9, 0x0e, 0x56, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x3e, 0xcc, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xcc, 
	0xcc, 0xcd, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 
	0x73, 0x33, 0x33, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 
	0x3e, 0x22, 0xf9, 0x83, 0x40, 0xc9, 0x0f, 0xdb, 0xc0, 0x49, 0x0f, 
	0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x54, 0x65, 0x78, 
	0x74, 0x75, 0x72, 0x65, 0x53, 0x69, 0x7a, 0x65, 0x00, 0x00, 0x00, 
	0x03, 0x03, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 
	0x02, 0x00, 0x03, 0x00, 0x00, 0x49, 0x6e, 0x70, 0x75, 0x74, 0x53, 
	0x69, 0x7a, 0x65, 0x00, 0x00, 0x00, 0x03, 0x03, 0x02, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x00, 0x03, 0x00, 0x00, 
	0x54, 0x45, 0x58, 0x30, 0x00, 0x00, 0x00, 0x05, 0x06, 0x03, 0x00, 
	0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 
	0x00, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x00, 0x00, 0x00, 
	0x18, 0x03, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x00, 0x4f, 0x75, 0x74, 0x70, 0x75, 0x74, 
	0x53, 0x69, 0x7a, 0x65, 0x00, 0x00, 0x00, 0x03, 0x03, 0x02, 0x00, 
	0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x16, 0x02, 0x00, 0x03, 0x00, 
	0x00, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 
	0x6f, 0x72, 0x00, 0x00, 0x06, 0x05, 0x07, 0x02, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x67, 
	0x6c, 0x5f, 0x50, 0x4d, 0x58, 0x50, 0x6f, 0x73, 0x41, 0x64, 0x6a, 
	0x75, 0x73, 0x74, 0x00, 0x00, 0x57, 0x05, 0x03, 0x02, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x01, 0x00, 0x3c, 0x04, 0x00, 0x0f, 0x00, 0x00, 
	0x00, 0x03, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 
	0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 
	0x6f, 0x72, 0x00, 0x00, 0x00, 0x00, 0x01, 0x61, 0x5f, 0x74, 0x65, 
	0x78, 0x43, 0x6f, 0x6f, 0x72, 0x64, 0x00, 0x00, 0x00, 0x00, 0x02
};

