# Cwift

### Conditional clang attributes used for C-Swift interoperability. 

Macros for using Clang's Swift attributes for cross platform headers, will expand to nothing on non-Apple platforms. 

Install with Cocoapods using `pod 'Cwift'`.

Example project [here](https://github.com/dave234/CwiftDemo)

## How to use:

Cwift's intent is to be able to take advantage of clang's swift specific attributes without having to import Foundation. One common use case is when you want to use an enum in both your C++ code and in your Swift code. You cannot use NS_ENUM in your non-apple code, as there is no way to import Foundation. Cwift takes care of this by conditionally adding the attribute directly.

``` C
// MyCFile.h 
typedef enum CWIFT_ENUM Region {
    RegionNorth,
    RegionEast,
    RegionSouth,
    RegionWest
} Region;
```

In C++, `CWIFT_ENUM` will not expand to anything, making it a normal enum. However, when this is imported to Swift, you will be able to use dot syntax.

``` Swift
///////////////// MySwiftFile.swift /////////////////
var myregion = Region.north
myRegion = .west
```
\
This also works for option sets.
``` C
///////////////// MyCFile.h /////////////////
typedef enum CWIFT_OPTIONS Characteristic {
    CharacteristicCool =   1 << 0,
    CharacteristicSmart =  2 << 0,
    CharacteristicFunny =  3 << 0
} Characteristic;
```
\
Characteristic is a normal bit mask in C/C++, but is imported into Swift as an option set.
``` Swift
///////////////// MySwiftFile.swift /////////////////
var characteristics: Characteristic = [.smart, .funny]
if !characteristics.contains(.cool) {
    characteristics.insert(.cool)
}
```
One very useful attribute is swift_newtype(struct), which is conditionally added using CWIFT_TYPE. This allows you to create a strong type when using some of the more vague data types like void * in your C code base. Having a concrete type makes extension possible in Swift which is useful when you cannot import the type directly as in the case of C++.  A strong type also makes possible the use of CWIFT_NAME, which will associate functions with types in order to use dot syntax.
``` C++
///////////////// MyCppFile.hpp /////////////////
namespace census_model {
    class person {
    public:
        int age;
        Region region;
    };
}
```
\
`Person` will be used to point to `census_model::person`. In C/C++, this helps associate our wrapper functions with the type they are wrapping.
``` C
///////////////// MyCFile.h /////////////////

typedef void * Person CWIFT_TYPE; 
Person  PersonCreate(int age, Region region)    CWIFT_NAME(Person.init(age:region:));
int     PersonGetAge(Person person)             CWIFT_NAME(getter:Person.age(self:));
void    PersonSetAge(Person person, int age)    CWIFT_NAME(setter:Person.age(self:newValue:));
```
`Person` uses `CWIFT_TYPE` to conditionaly add Clang's swift_newtype(struct) attribute. In Swift it is imported as a discrete type that is not interchangeable with `void *`. Since it is a type it can be extended in Swift, and also from the C declaration using `CWIFT_NAME`. 
 \
 \
 This is how these three functions are called in Swift:
``` Swift
///////////////// MySwiftFile.swift /////////////////
guard var person = Person(age: 20, region: .north) else { fatalError() }
person.age += 10
```
\
Without CWIFT_NAME, calling C code looks like this:
``` Swift
guard var person = PersonCreate(age: 20, region: .north) else { fatalError() }
PersonSetAge(person, PersonGetAge(person) + 10)
```
\
In C/C++ Person is a `void *`. Cast to and from Person -> census_model::person In your wrapper implementations.
``` C
///////////////// MyCFile.c /////////////////
Person PersonCreate(int age, Region region) { 
    return (Person) new census_model::person {
        .age = age,
        .region = region
    };
}

int PersonGetAge(Person person) {
    return ((census_model::person *)person)->age;
}

void PersonSetAge(Person person, int age) {
    ((census_model::person *)person)->age = age;
}
```

---

Copyright (C) 2019 David O'Neill

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
