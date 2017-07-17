/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

const {
  ConsoleAPIListener
} = require("devtools/server/actors/utils/webconsole-listeners");
const { on, once, off, emit, count } = require("devtools/shared/event-emitter");

const pass = (message) => ok(true, message);
const fail = (message) => ok(false, message);

const TESTS = {
  "test add a listener"() {
    let events = [ { name: "event#1" }, "event#2" ];
    let target = { name: "target" };

    on(target, "message", function (message) {
      equal(this, target, "this is a target object");
      equal(message, events.shift(), "message is emitted event");
    });

    emit(target, "message", events[0]);
    emit(target, "message", events[0]);
  },

  "test that listener is unique per type"() {
    let actual = [];
    let target = {};
    listener = () => actual.push(1);

    on(target, "message", listener);
    on(target, "message", listener);
    on(target, "message", listener);
    on(target, "foo", listener);
    on(target, "foo", listener);

    emit(target, "message");
    deepEqual([ 1 ], actual, "only one message listener added");

    emit(target, "foo");
    deepEqual([ 1, 1 ], actual, "same listener added for other event");
  },

  "test event type matters"() {
    let target = { name: "target" };
    on(target, "message", () => fail("no event is expected"));
    on(target, "done", () => pass("event is emitted"));

    emit(target, "foo");
    emit(target, "done");
  },

  "test all arguments are passed"() {
    let foo = { name: "foo" }, bar = "bar";
    let target = { name: "target" };

    on(target, "message", (a, b) => {
      equal(a, foo, "first argument passed");
      equal(b, bar, "second argument passed");
    });

    emit(target, "message", foo, bar);
  },

  "test no side-effects in emit"() {
    let target = { name: "target" };

    on(target, "message", () => {
      pass("first listener is called");

      on(target, "message", () => fail("second listener is called"));
    });
    emit(target, "message");
  },

  "test can remove next listener"() {
    let target = { name: "target"};

    on(target, "data", () => {
      pass("first listener called");
      off(target, "data", fail);
    });
    on(target, "data", fail);

    emit(target, "data", "Listener should be removed");
  },

  "test order of propagation"() {
    let actual = [];
    let target = { name: "target" };

    on(target, "message", () => actual.push(1));
    on(target, "message", () => actual.push(2));
    on(target, "message", () => actual.push(3));
    emit(target, "message");

    deepEqual([ 1, 2, 3 ], actual, "called in order they were added");
  },

  "test remove a listener"() {
    let target = { name: "target" };
    let actual = [];

    on(target, "message", function listener() {
      actual.push(1);
      on(target, "message", () => {
        off(target, "message", listener);
        actual.push(2);
      });
    });

    emit(target, "message");
    deepEqual([ 1 ], actual, "first listener called");

    emit(target, "message");
    deepEqual([ 1, 1, 2 ], actual, "second listener called");

    emit(target, "message");
    deepEqual([ 1, 1, 2, 2, 2 ], actual, "first listener removed");
  },

  "test remove all listeners for type"() {
    let actual = [];
    let target = { name: "target" };

    on(target, "message", () => actual.push(1));
    on(target, "message", () => actual.push(2));
    on(target, "message", () => actual.push(3));
    on(target, "bar", () => actual.push("b"));
    off(target, "message");

    emit(target, "message");
    emit(target, "bar");

    deepEqual([ "b" ], actual, "all message listeners were removed");
  },

  "test remove all listeners"() {
    let actual = [];
    let target = { name: "target" };

    on(target, "message", () => actual.push(1));
    on(target, "message", () => actual.push(2));
    on(target, "message", () => actual.push(3));
    on(target, "bar", () => actual.push("b"));

    off(target);

    emit(target, "message");
    emit(target, "bar");

    deepEqual([], actual, "all listeners events were removed");
  },

  "test falsy arguments are fine"() {
    let type, listener, actual = [];
    let target = { name: "target" };
    on(target, "bar", () => actual.push(0));

    off(target, "bar", listener);
    emit(target, "bar");
    deepEqual([ 0 ], actual, "3rd bad arg will keep listener");

    off(target, type);
    emit(target, "bar");
    deepEqual([ 0, 0 ], actual, "2nd bad arg will keep listener");

    off(target, type, listener);
    emit(target, "bar");
    deepEqual([ 0, 0, 0 ], actual, "2nd & 3rd bad args will keep listener");
  },

  "test unhandled exceptions"(done) {
    let listener = new ConsoleAPIListener(null, {
      onConsoleAPICall(message) {
        equal(message.level, "error", "Got the first exception");
        ok(message.arguments[0].startsWith("Error: Boom!"),
          "unhandled exception is logged");

        listener.destroy();
        done();
      }
    });

    listener.init();

    let target = {};

    on(target, "message", () => {
      throw Error("Boom!");
    });

    emit(target, "message");
  },

  "test count"() {
    let target = {};

    equal(count(target, "foo"), 0, "no listeners for 'foo' events");
    on(target, "foo", () => {});
    equal(count(target, "foo"), 1, "listener registered");
    on(target, "foo", () => {});
    equal(count(target, "foo"), 2, "another listener registered");
    off(target);
    equal(count(target, "foo"), 0, "listeners unregistered");
  },

  async "test once"() {
    let target = {};
    let called = false;

    let pFoo = once(target, "foo", value => {
      ok(!called, "listener called only once");
      equal(value, "bar", "correct argument was passed");
    });
    let pDone = once(target, "done");

    emit(target, "foo", "bar");
    emit(target, "foo", "baz");
    emit(target, "done", "");

    await Promise.all([pFoo, pDone]);
  },

  "test removing once"(done) {
    let target = {};

    once(target, "foo", fail);
    once(target, "done", done);

    off(target, "foo", fail);

    emit(target, "foo", "listener was called");
    emit(target, "done", "");
  }
};

const run = (tests) => (async function () {
  for (let name of Object.keys(tests)) {
    do_print(name);
    if (tests[name].length === 1) {
      await (new Promise(resolve => tests[name](resolve)));
    } else {
      await tests[name]();
    }
  }
});

add_task(run(TESTS));
