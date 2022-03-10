#include "shaders.h"

const char c2d_handheld_lcd1x_shader[1799] = {
	0x38, 0xb4, 0xfa, 0x10, 0x30, 0xe4, 0x02, 0x3d, 0x00, 0x01, 0x05, 
	0x43, 0x02, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x06, 0xe7, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x19, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x21, 0x1e, 
	0x0c, 0x00, 0x00, 0x00, 0x01, 0x9c, 0x20, 0x50, 0x53, 0x55, 0x23, 
	0x00, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 
	0x38, 0x40, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0xd8, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x0a, 0x00, 0xf6, 0x01, 0xff, 0xff, 0x00, 0x00, 0x0c, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
	0x11, 0x00, 0x0c, 0x00, 0x80, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x0d, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x0e, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0f, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x09, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 
	0x00, 0x0a, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0b, 0x00, 
	0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 
	0x00, 0x01, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 
	0x06, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x07, 0x00, 0x0b, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x11, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x01, 0x00, 0x12, 
	0x00, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x13, 0x00, 0x0f, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xff, 
	0x0f, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0x02, 0x00, 
	0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x08, 0x83, 0x21, 0x15, 0x80, 0x38, 0x8d, 0x00, 
	0x04, 0xb1, 0x85, 0x01, 0xa4, 0x08, 0xcd, 0x00, 0x44, 0xb1, 0x89, 
	0x01, 0xc0, 0x08, 0x0a, 0x84, 0x99, 0xaf, 0xbc, 0x0d, 0x80, 0x40, 
	0x7c, 0x02, 0x44, 0xcf, 0x84, 0x07, 0xa4, 0x08, 0x07, 0x11, 0x45, 
	0xcf, 0x80, 0x87, 0xb1, 0x18, 0x05, 0x11, 0x49, 0xcf, 0x80, 0x87, 
	0xb1, 0x18, 0x0b, 0x11, 0x0d, 0xc0, 0x81, 0x81, 0xb1, 0x18, 0x0c, 
	0xd1, 0x4e, 0xc0, 0x81, 0x81, 0xb1, 0x18, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x14, 0x3f, 0x80, 0x03, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x43, 0x4f, 0x4c, 0x30, 
	0x00, 0x00, 0x00, 0x05, 0x05, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x43, 0x4f, 0x4c, 
	0x4f, 0x52, 0x00, 0x00, 0x00, 0x05, 0x04, 0x03, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x54, 
	0x45, 0x58, 0x30, 0x00, 0x00, 0x00, 0x05, 0x05, 0x03, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x01, 0x00, 0x04, 0x04, 0x00, 0x0f, 0x00, 0x00, 
	0x54, 0x65, 0x78, 0x43, 0x6f, 0x6f, 0x72, 0x64, 0x00, 0x00, 0x00, 
	0x05, 0x04, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x04, 
	0x04, 0x00, 0x0f, 0x00, 0x00, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 
	0x69, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x00, 0x01, 0x05, 0x05, 0x03, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x04, 0x00, 0x0f, 
	0x00, 0x00, 0x4d, 0x56, 0x50, 0x4d, 0x61, 0x74, 0x72, 0x69, 0x78, 
	0x00, 0x00, 0x00, 0x16, 0x03, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 
	0x01, 0x00, 0x04, 0x10, 0xff, 0xff, 0x00, 0x00, 0x56, 0x65, 0x72, 
	0x74, 0x65, 0x78, 0x43, 0x6f, 0x6f, 0x72, 0x64, 0x00, 0x00, 0x00, 
	0x05, 0x04, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x08, 
	0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 
	0x00, 0x00, 0x00, 0x00, 0x21, 0x1e, 0x0c, 0x00, 0x00, 0x00, 0x03, 
	0x0c, 0x20, 0x50, 0x53, 0x55, 0x23, 0x00, 0x00, 0x00, 0x00, 0x03, 
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0x00, 0x00, 0x00, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x77, 0x00, 0xff, 
	0xff, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xfc, 0xff, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x08, 0x00, 0x12, 0x00, 0x00, 0x00, 0x80, 
	0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 
	0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x80, 0x41, 0x00, 0x00, 
	0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x80, 0x40, 0x01, 0x00, 0x00, 
	0x00, 0x01, 0x00, 0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x04, 
	0x00, 0x00, 0x00, 0x05, 0x00, 0x89, 0x88, 0x88, 0x39, 0x05, 0x00, 
	0x00, 0x00, 0x05, 0x00, 0xab, 0xaa, 0x2a, 0x3b, 0x06, 0x00, 0x00, 
	0x00, 0x05, 0x00, 0x01, 0x0d, 0xd0, 0x35, 0x07, 0x00, 0x00, 0x00, 
	0x05, 0x00, 0x61, 0x0b, 0xb6, 0x37, 0x08, 0x00, 0x00, 0x00, 0x05, 
	0x00, 0xab, 0xaa, 0xaa, 0x3c, 0x09, 0x00, 0x00, 0x00, 0x05, 0x00, 
	0x00, 0x00, 0x00, 0x3e, 0x0a, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0x0b, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 
	0x80, 0x3f, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0a, 0x00, 0x0d, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x05, 0x00, 0x0e, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x07, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x40, 0x09, 0x00, 0xf8, 0x00, 0x00, 0x0c, 0xa0, 0x81, 0x02, 
	0x8b, 0x48, 0x00, 0x00, 0x00, 0xa0, 0x84, 0x00, 0xa3, 0xe9, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xf9, 0x81, 0x05, 0xa0, 0x81, 
	0x0a, 0x00, 0x80, 0x30, 0x02, 0x06, 0x80, 0x81, 0x02, 0x00, 0x80, 
	0x30, 0x00, 0x01, 0xec, 0x62, 0x22, 0x0c, 0x83, 0x40, 0x00, 0x0d, 
	0x00, 0xa3, 0x0a, 0x00, 0x81, 0x50, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 
	0x00, 0x02, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8c, 0x01, 0x00, 0xd0, 
	0x8c, 0x31, 0xa1, 0x00, 0x00, 0x06, 0x00, 0x40, 0x80, 0x41, 0x82, 
	0x08, 0x01, 0x00, 0x00, 0x20, 0x8c, 0x0d, 0x81, 0x40, 0xcc, 0x02, 
	0x00, 0xd0, 0x80, 0x01, 0xa1, 0x00, 0x00, 0x20, 0x04, 0x00, 0x88, 
	0x41, 0xc0, 0x08, 0x05, 0x06, 0x40, 0x70, 0x80, 0x21, 0xa2, 0x00, 
	0x41, 0x00, 0x44, 0x0f, 0x88, 0x41, 0xc0, 0x08, 0xc0, 0x01, 0xf8, 
	0xc3, 0x20, 0x05, 0x80, 0x38, 0x08, 0x22, 0xb6, 0xdf, 0xc0, 0x87, 
	0xb1, 0x18, 0x89, 0x1b, 0x16, 0xc0, 0x34, 0x81, 0xd8, 0x28, 0x8a, 
	0x13, 0x16, 0xc0, 0x34, 0x01, 0xd8, 0x28, 0x01, 0x0f, 0x08, 0x00, 
	0x84, 0x41, 0xa4, 0x08, 0x00, 0x8f, 0x0c, 0x00, 0x88, 0x41, 0xc0, 
	0x08, 0x00, 0x10, 0x04, 0x00, 0x88, 0x41, 0xc0, 0x08, 0x00, 0x91, 
	0x28, 0xc0, 0x80, 0x41, 0x84, 0x08, 0x00, 0x03, 0x04, 0xc0, 0x88, 
	0x41, 0xc0, 0x08, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 
	0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x88, 0x06, 0x01, 0x00, 0x0e, 0x00, 0x02, 0x00, 0x02, 
	0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 
	0x00, 0x02, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x03, 0x00, 0x03, 0x00, 
	0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 
	0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x01, 
	0x00, 0x0e, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 
	0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 
	0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 
	0x0f, 0x88, 0x10, 0xc0, 0x08, 0x01, 0x0f, 0x00, 0x2f, 0x80, 0x43, 
	0x80, 0x10, 0x00, 0xbc, 0x05, 0x20, 0x1e, 0x0c, 0x81, 0x40, 0x17, 
	0x00, 0x00, 0xb0, 0x06, 0x00, 0x80, 0x81, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x40, 0xc9, 0x0f, 0xdb, 
	0x41, 0x80, 0x00, 0x00, 0x41, 0x88, 0x00, 0x00, 0x40, 0x80, 0x00, 
	0x00, 0x40, 0xa0, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x3f, 0x00, 
	0x00, 0x00, 0x3e, 0x22, 0xf9, 0x83, 0x40, 0xc9, 0x0f, 0xdb, 0xc0, 
	0x49, 0x0f, 0xdb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x54, 0x45, 0x58, 0x30, 0x00, 
	0x00, 0x00, 0x05, 0x06, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 
	0x00, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x54, 0x65, 0x78, 0x74, 
	0x75, 0x72, 0x65, 0x53, 0x69, 0x7a, 0x65, 0x00, 0x00, 0x00, 0x03, 
	0x03, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 
	0x00, 0x03, 0x00, 0x00, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 
	0x00, 0x00, 0x00, 0x18, 0x03, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 
	0x02, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x67, 0x6c, 0x5f, 
	0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x00, 
	0x06, 0x05, 0x07, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 
	0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x03, 0x61, 0x5f, 0x70, 
	0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6f, 0x72, 
	0x64, 0x00, 0x00, 0x00, 0x00, 0x02
};
