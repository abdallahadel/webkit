description(
'This tests for caller property in functions. Only functions that are called from inside of other functions and have a parent should have this property set. Tests return true when caller is found and false when the caller is null.'
)       
function child()
{
    return (child.caller !== null);
}

function parent()
{
    return child();
}

var childHasCallerWhenExecutingGlobalCode = (child.caller !== null);
var childHasCallerWhenCalledWithoutParent = child();
var childHasCallerWhenCalledFromWithinParent = parent();

shouldBe('childHasCallerWhenExecutingGlobalCode', 'false');
shouldBe('childHasCallerWhenCalledWithoutParent', 'false');
shouldBe('childHasCallerWhenCalledFromWithinParent', 'true')

// The caller property should throw in strict mode, and a non-strict function cannot use caller to reach a strict caller (see ES5.1 15.3.5.4).
function nonStrictCallee() { return nonStrictCallee.caller; }
function strictCallee() { "use strict"; return strictCallee.caller; }
function nonStrictCaller(x) { return x(); }
// Tail calls leak and show our caller's caller, which is null here
function strictCaller(x) { "use strict"; var result = x(); return result; }
function strictTailCaller(x) { "use strict"; return x(); }
shouldBe("nonStrictCaller(nonStrictCallee)", "nonStrictCaller");
shouldThrow("nonStrictCaller(strictCallee)");
shouldThrow("strictCaller(nonStrictCallee)", '"TypeError: Function.caller used to retrieve strict caller"');
shouldThrow("strictCaller(strictCallee)");
shouldBe("strictTailCaller(nonStrictCallee)", "null");
shouldThrow("strictTailCaller(strictCallee)");

// .caller within a bound function reaches the caller, ignoring the binding.
var boundNonStrictCallee = nonStrictCallee.bind();
var boundStrictCallee = strictCallee.bind();
shouldBe("nonStrictCaller(boundNonStrictCallee)", "nonStrictCaller");
shouldThrow("nonStrictCaller(boundStrictCallee)");
shouldThrow("strictCaller(boundNonStrictCallee)", '"TypeError: Function.caller used to retrieve strict caller"');
shouldThrow("strictCaller(boundStrictCallee)");
shouldBe("strictTailCaller(boundNonStrictCallee)", "null");
shouldThrow("strictTailCaller(boundStrictCallee)");

// Check that .caller works (or throws) as expected, over an accessor call.
function getFooGetter(x) { return Object.getOwnPropertyDescriptor(x, 'foo').get; }
function getFooSetter(x) { return Object.getOwnPropertyDescriptor(x, 'foo').set; }
var nonStrictAccessor = {
    get foo() { return getFooGetter(nonStrictAccessor).caller; },
    set foo(x) { if (getFooSetter(nonStrictAccessor).caller !==x) throw false; }
};
var strictAccessor = {
    get foo() { "use strict"; return getFooGetter(strictAccessor).caller; },
    set foo(x) { "use strict"; if (getFooSetter(strictAccessor).caller !==x) throw false; }
};
function nonStrictGetter(x) { return x.foo; }
function nonStrictSetter(x) { x.foo = nonStrictSetter; return true; }
function strictGetter(x) { "use strict"; return x.foo; }
function strictSetter(x) { "use strict"; x.foo = nonStrictSetter; return true; }
shouldBe("nonStrictGetter(nonStrictAccessor)", "nonStrictGetter");
shouldBeTrue("nonStrictSetter(nonStrictAccessor)");
shouldThrow("nonStrictGetter(strictAccessor)");
shouldThrow("nonStrictSetter(strictAccessor)");
shouldThrow("strictGetter(nonStrictAccessor)", '"TypeError: Function.caller used to retrieve strict caller"');
shouldThrow("strictSetter(nonStrictAccessor)", '"TypeError: Function.caller used to retrieve strict caller"');
shouldThrow("strictGetter(strictAccessor)");
shouldThrow("strictSetter(strictAccessor)");
