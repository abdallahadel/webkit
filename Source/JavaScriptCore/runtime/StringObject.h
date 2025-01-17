/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2007-2008, 2016 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef StringObject_h
#define StringObject_h

#include "JSWrapperObject.h"
#include "JSString.h"

namespace JSC {

class StringObject : public JSWrapperObject {
public:
    typedef JSWrapperObject Base;
    static const unsigned StructureFlags = Base::StructureFlags | OverridesGetOwnPropertySlot | InterceptsGetOwnPropertySlotByIndexEvenWhenLengthIsNotZero | OverridesGetPropertyNames;

    static StringObject* create(VM& vm, Structure* structure)
    {
        JSString* string = jsEmptyString(&vm);
        StringObject* object = new (NotNull, allocateCell<StringObject>(vm.heap)) StringObject(vm, structure);
        object->finishCreation(vm, string);
        return object;
    }
    static StringObject* create(VM& vm, Structure* structure, JSString* string)
    {
        StringObject* object = new (NotNull, allocateCell<StringObject>(vm.heap)) StringObject(vm, structure);
        object->finishCreation(vm, string);
        return object;
    }
    static StringObject* create(VM&, JSGlobalObject*, JSString*);

    JS_EXPORT_PRIVATE static bool getOwnPropertySlot(JSObject*, ExecState*, PropertyName, PropertySlot&);
    JS_EXPORT_PRIVATE static bool getOwnPropertySlotByIndex(JSObject*, ExecState*, unsigned propertyName, PropertySlot&);

    JS_EXPORT_PRIVATE static bool put(JSCell*, ExecState*, PropertyName, JSValue, PutPropertySlot&);
    JS_EXPORT_PRIVATE static bool putByIndex(JSCell*, ExecState*, unsigned propertyName, JSValue, bool shouldThrow);

    JS_EXPORT_PRIVATE static bool deleteProperty(JSCell*, ExecState*, PropertyName);
    JS_EXPORT_PRIVATE static bool deletePropertyByIndex(JSCell*, ExecState*, unsigned propertyName);
    JS_EXPORT_PRIVATE static void getOwnPropertyNames(JSObject*, ExecState*, PropertyNameArray&, EnumerationMode);
    JS_EXPORT_PRIVATE static bool defineOwnProperty(JSObject*, ExecState*, PropertyName, const PropertyDescriptor&, bool shouldThrow);

    DECLARE_EXPORT_INFO;

    JSString* internalValue() const { return asString(JSWrapperObject::internalValue());}

    static Structure* createStructure(VM& vm, JSGlobalObject* globalObject, JSValue prototype)
    {
        return Structure::create(vm, globalObject, prototype, TypeInfo(ObjectType, StructureFlags), info());
    }

protected:
    JS_EXPORT_PRIVATE void finishCreation(VM&, JSString*);
    JS_EXPORT_PRIVATE StringObject(VM&, Structure*);
};

StringObject* asStringObject(JSValue);

inline StringObject* asStringObject(JSValue value)
{
    ASSERT(asObject(value)->inherits(StringObject::info()));
    return static_cast<StringObject*>(asObject(value));
}

JS_EXPORT_PRIVATE StringObject* constructString(VM&, JSGlobalObject*, JSValue);

// Helper for producing a JSString for 'string', where 'string' was been produced by
// calling ToString on 'originalValue'. In cases where 'originalValue' already was a
// string primitive we can just use this, otherwise we need to allocate a new JSString.
static inline JSString* jsStringWithReuse(ExecState* exec, JSValue originalValue, const String& string)
{
    if (originalValue.isString()) {
        ASSERT(asString(originalValue)->value(exec) == string);
        return asString(originalValue);
    }
    return jsString(exec, string);
}

// Helper that tries to use the JSString substring sharing mechanism if 'originalValue' is a JSString.
static inline JSString* jsSubstring(ExecState* exec, JSValue originalValue, const String& string, unsigned offset, unsigned length)
{
    if (originalValue.isString()) {
        ASSERT(asString(originalValue)->value(exec) == string);
        return jsSubstring(exec, asString(originalValue), offset, length);
    }
    return jsSubstring(exec, string, offset, length);
}


} // namespace JSC

#endif // StringObject_h
