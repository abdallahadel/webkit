/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(WEB_TIMING)
#include "JSPerformanceEntry.h"

#include "JSDOMBinding.h"
#if ENABLE(USER_TIMING)
#include "JSPerformanceMark.h"
#include "JSPerformanceMeasure.h"
#endif
#include "JSPerformanceResourceTiming.h"
#include "PerformanceMark.h"
#include "PerformanceMeasure.h"
#include "PerformanceResourceTiming.h"

using namespace JSC;

namespace WebCore {

JSValue toJS(ExecState*, JSDOMGlobalObject* globalObject, PerformanceEntry* entry)
{
    if (!entry)
        return jsNull();

    if (is<PerformanceResourceTiming>(*entry))
        return wrap<JSPerformanceResourceTiming>(globalObject, downcast<PerformanceResourceTiming>(*entry));

#if ENABLE(USER_TIMING)
    if (is<PerformanceMark>(*entry))
        return wrap<JSPerformanceMark>(globalObject, downcast<PerformanceMark>(*entry));

    if (is<PerformanceMeasure>(*entry))
        return wrap<JSPerformanceMeasure>(globalObject, downcast<PerformanceMeasure>(*entry));
#endif

    return wrap<JSPerformanceEntry>(globalObject, *entry);
}

} // namespace WebCore
#endif // ENABLE(WEB_TIMING)
