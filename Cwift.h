//
//  Copyright (C) 2019 David O'Neill
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source distribution.


#ifndef Cwift_h
#define Cwift_h

#ifdef __OBJC__

#define CWIFT_ENUM __attribute__((enum_extensibility(open)))
#define CWIFT_NAME(_name) __attribute__((swift_name(#_name)))
#define CWIFT_TYPE __attribute((swift_newtype(struct)))
#define CWIFT_OPTIONS __attribute__((flag_enum,enum_extensibility(open)))
#define CWIFT_BOOL BOOL

#else 

//////////// Expand to nothing if ndef ObjC or Swift /////////////

#define CWIFT_ENUM
#define CWIFT_NAME(_name)
#define CWIFT_TYPE
#define CWIFT_OPTIONS
#define CWIFT_BOOL signed char

#endif 

#endif // Cwift_h
